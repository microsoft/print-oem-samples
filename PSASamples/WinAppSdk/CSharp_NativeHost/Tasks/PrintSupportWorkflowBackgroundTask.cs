using System;
using System.Collections.Generic;
using System.Linq;
using Windows.ApplicationModel.Background;
using Windows.Devices.Printers;
using Windows.Graphics.Printing.Workflow;
using Windows.Storage.Streams;
#if XPSUTIL_AVAILABLE
using XpsUtil;
#endif

namespace Tasks
{
    public sealed partial class PrintSupportWorkflowBackgroundTask : IBackgroundTask
    {
        // Can be completed after PrintWorkflowPdlTargetStream.CompleteStreamSubmission is complete.
        public BackgroundTaskDeferral? TaskInstanceDeferral { get; set; }

        public void Run(IBackgroundTaskInstance taskInstance)
        {
            TaskInstanceDeferral = taskInstance.GetDeferral();

            if (taskInstance.TriggerDetails is PrintWorkflowJobTriggerDetails jobDetails)
            {
                var session = jobDetails.PrintWorkflowJobSession;
                session.JobStarting += OnJobStarting;
                session.PdlModificationRequested += OnPdlModificationRequested;

                // Make sure to register all the event handlers before PrintWorkflowJobBackgroundSession.Start is called.
                session.Start();
            }
        }

        private void OnJobStarting(PrintWorkflowJobBackgroundSession sender, PrintWorkflowJobStartingEventArgs args)
        {
            // Not fully implemented yet.
            args.SetSkipSystemRendering();
            args.GetDeferral().Complete();
        }

        private bool HasPdlConverter(string documentFormat)
        {
            switch (documentFormat)
            {
                case "image/pwg-raster":
                case "application/PCLm":
                case "application/pdf":
                    return true;
            }
            return false;
        }

        private string GetDocumentFormat(IppPrintDevice printer)
        {
            //
            // Example: Getting IPP attributes from the printer.
            //

            // Create a list of IPP attributes that we are requesting.
            var requestedAttributes = new List<string>
                {
                    "document-format-default",
                    "document-format-supported"
                };
            var attributes = printer.GetPrinterAttributes(requestedAttributes);

            // Lookup the IPP attribute from the map.
            string defaultFormat = attributes["document-format-default"].GetKeywordArray().First();

            // If the default format does not have a PDL converter, pick the first supported format.
            if (!HasPdlConverter(defaultFormat))
            {
                foreach (string documentFormat in attributes["document-format-supported"].GetKeywordArray())
                {
                    if (HasPdlConverter(documentFormat))
                    {
                        return documentFormat;
                    }
                }
            }
            return defaultFormat;
        }

        private void OnPdlModificationRequested(PrintWorkflowJobBackgroundSession sender, PrintWorkflowPdlModificationRequestedEventArgs args)
        {
            var deferral = args.GetDeferral();
            try
            {
                var uiResult = args.UILauncher.LaunchAndCompleteUIAsync().AsTask().GetAwaiter().GetResult();
                if (uiResult != PrintWorkflowUICompletionStatus.Completed)
                {
                    if (uiResult == PrintWorkflowUICompletionStatus.UserCanceled)
                    {
                        args.Configuration.AbortPrintFlow(PrintWorkflowJobAbortReason.UserCanceled);
                    }
                    return;
                }

                var sourceContent = args.SourceContent;
                var inputStream = sourceContent.GetInputStream();

#if XPSUTIL_AVAILABLE
                if (string.Equals(sourceContent.ContentType, "application/OXPS", StringComparison.OrdinalIgnoreCase))
                {
                    // Source is OXPS - convert to printer's preferred format
                    string documentFormat = GetDocumentFormat(args.PrinterJob.Printer);

                    // Add custom job attributes.
                    var jobAttributes = new Dictionary<string, IppAttributeValue>();

                    if (!string.IsNullOrEmpty(LocalStorageUtil.GetJobPasswordEncryptionMethod()))
                    {
                        var operationAttributeCollection = new Dictionary<string, IppAttributeValue>
                        {
                            {"job-password",  IppAttributeValue.CreateOctetString(LocalStorageUtil.GetEncryptedJobPassword())},
                            {"job-password-encryption", IppAttributeValue.CreateKeyword(LocalStorageUtil.GetJobPasswordEncryptionMethod())}
                        };
                        jobAttributes.Add("msft-operation-attribute-col", IppAttributeValue.CreateCollection(operationAttributeCollection));
                        LocalStorageUtil.ClearJobPassword();
                    }

                    PrintWorkflowObjectModelSourceFileContent xpsContentObjectModel = new PrintWorkflowObjectModelSourceFileContent(inputStream);

                    XpsPageWatermarker watermarker = GetXpsPageWatermarker();
                    var document = new XpsSequentialDocument(xpsContentObjectModel);

                    document.XpsGenerationFailed += (doc, e) => {
                        args.Configuration.AbortPrintFlow(PrintWorkflowJobAbortReason.JobFailed);
                    };

                    IInputStream watermarkedStream = document.GetWatermarkedStream(watermarker);

                    // Create job on printer with the format we'll send after conversion
                    var targetStream = args.CreateJobOnPrinterWithAttributes(jobAttributes, documentFormat);

                    // Get the PDL converter for the target format
                    PrintWorkflowPdlConverter? pdlConverter = null;
                    switch (documentFormat)
                    {
                        case "image/pwg-raster":
                            pdlConverter = args.GetPdlConverter(PrintWorkflowPdlConversionType.XpsToPwgr);
                            break;
                        case "application/PCLm":
                            pdlConverter = args.GetPdlConverter(PrintWorkflowPdlConversionType.XpsToPclm);
                            break;
                        case "application/pdf":
                            pdlConverter = args.GetPdlConverter(PrintWorkflowPdlConversionType.XpsToPdf);
                            break;
                    }

                    if (pdlConverter != null)
                    {
                        pdlConverter.ConvertPdlAsync(args.PrinterJob.GetJobPrintTicket(),
                                                     watermarkedStream,
                                                     targetStream.GetOutputStream()
                                                     ).AsTask().GetAwaiter().GetResult();
                    }

                    targetStream.CompleteStreamSubmission(PrintWorkflowSubmittedStatus.Succeeded);
                }
                else
#endif
                {
                    // Source is not OXPS - pass it through using its actual format
                    var documentFormat = sourceContent.ContentType;
                    var targetStream = args.CreateJobOnPrinter(documentFormat);

                    RandomAccessStream.CopyAndCloseAsync(inputStream, targetStream.GetOutputStream()).AsTask().GetAwaiter().GetResult();
                    targetStream.CompleteStreamSubmission(PrintWorkflowSubmittedStatus.Succeeded);
                }
            }
            catch (Exception)
            {
                args.Configuration.AbortPrintFlow(PrintWorkflowJobAbortReason.JobFailed);
            }
            finally
            {
                deferral.Complete();
                TaskInstanceDeferral?.Complete();
            }
        }

#if XPSUTIL_AVAILABLE
        private XpsPageWatermarker GetXpsPageWatermarker()
        {
            XpsPageWatermarker watermarker = new XpsPageWatermarker();

            LocalStorageUtil.GetWatermarkTextPropertiesFromLocalStorage(out string watermarkText, out int fontSize, out double xOffset, out double yOffset);
            watermarker.SetWatermarkText(watermarkText, fontSize, xOffset, yOffset);

            bool usingImage = LocalStorageUtil.GetImagePropertiesFromLocalStorage(out string imageFile, out double dpiX, out double dpiY, out int imageWidth, out int imageHeight);

            watermarker.SetWatermarkImageEnabled(usingImage && imageFile != null);
            if (usingImage && imageFile != null)
            {
                watermarker.SetWatermarkImage(imageFile, dpiX, dpiY, imageWidth, imageHeight);
            }

            return watermarker;
        }
#endif
    }
}
