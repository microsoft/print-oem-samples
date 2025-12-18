using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.ApplicationModel.Background;
using Windows.Data.Xml.Dom;
using Windows.Foundation.Metadata;
using Windows.Graphics.Printing.PrintSupport;
using Windows.Graphics.Printing.Workflow;
using Windows.Graphics.Printing.PrintTicket;


using Windows.Devices.Printers;
using System.IO;
using Windows.Storage.Streams;
using Windows.Storage;
using Windows.Devices.Input;
using XpsUtil;

namespace Tasks
{
    public sealed class VirtualPrinterBackgroundTask : IBackgroundTask
    {
        private BackgroundTaskDeferral taskDeferral;
        private IppPrintDevice printDevice;

        public void Run(IBackgroundTaskInstance taskInstance)
        {
            var virtualPrinterDetails = taskInstance.TriggerDetails as PrintWorkflowVirtualPrinterTriggerDetails;
            taskDeferral = taskInstance.GetDeferral();

            PrintWorkflowVirtualPrinterSession session = virtualPrinterDetails.VirtualPrinterSession;
            session.VirtualPrinterDataAvailable += Session_VirtualPrinterDataAvailable;
            // Get print device for the session
            printDevice = session.Printer;

            // Make sure to register all the event handlers before PrintWorkflowVirtualPrinterSession.Start is called.
            session.Start();
        }

        private void Session_VirtualPrinterDataAvailable(PrintWorkflowVirtualPrinterSession sender, PrintWorkflowVirtualPrinterDataAvailableEventArgs args)
        {
            PrintWorkflowSubmittedStatus jobStatus = PrintWorkflowSubmittedStatus.Failed;
            try
            {
                PrintWorkflowPdlSourceContent sourceContent = args.SourceContent;
                StorageFile targetFile = args.GetTargetFileAsync().AsTask().Result;
                if (targetFile != null)
                {
                    IRandomAccessStream outputStream = targetFile.OpenAsync(FileAccessMode.ReadWrite).AsTask().Result;
                    IInputStream inputStream = sourceContent.GetInputStream();
                    
                    using (var outStream = outputStream.GetOutputStreamAt(0))
                    {
                        if (targetFile.FileType == ".pdf")
                        {
                            if (sourceContent.ContentType.ToLower() == "application/oxps")
                            {
                                // Launch UI to allow user to configure watermarks before conversion
                                PrintWorkflowUILauncher uiLauncher = args.UILauncher;

                                // Launch the UI and wait for it to complete
                                // The UI will be handled by JobActivatedMainPage.OnVirtualSessionPdlDataAvailable
                                // which navigates to WatermarkManipulationExample
                                // User's settings will be saved to LocalStorage
                                var uiResult = uiLauncher.LaunchAndCompleteUIAsync().AsTask().Result;

                                // Check if user canceled the operation
                                if (uiResult == PrintWorkflowUICompletionStatus.UserCanceled)
                                {
                                    // User canceled, abort the print job
                                    jobStatus = PrintWorkflowSubmittedStatus.Canceled;
                                    return;
                                }
                                else if (uiResult != PrintWorkflowUICompletionStatus.Completed)
                                {
                                    // UI failed for some other reason
                                    jobStatus = PrintWorkflowSubmittedStatus.Failed;
                                    return;
                                }

                                // Get the XPS document data stream from the source content.
                                PrintWorkflowObjectModelSourceFileContent xpsContentObjectModel = new PrintWorkflowObjectModelSourceFileContent(inputStream);

                                XpsPageWatermarker watermarker = ConfigureWatermarker();

                                // Adds the watermark to the XPS document.
                                var document = new XpsSequentialDocument(xpsContentObjectModel);

                                // Get the watermarked stream to use for conversion or direct output
                                inputStream = document.GetWatermarkedStream(watermarker);

                                // Get XPS to PDF PDL converter.
                                PrintWorkflowPdlConverter converter = args.GetPdlConverter(PrintWorkflowPdlConversionType.XpsToPdf);
                                // Convert XPS to PDF and write contents to outputStream.
                                var printTicket = args.GetJobPrintTicket();

                                converter.ConvertPdlAsync(printTicket, inputStream, outStream).AsTask().Wait();
                            }
                            else if (sourceContent.ContentType.ToLower() == "application/pdf")
                            {
                                RandomAccessStream.CopyAndCloseAsync(inputStream, outStream).AsTask().Wait();
                            }
                            else
                            {
                                throw new Exception();
                            }
                        }
                        else
                        {
                            RandomAccessStream.CopyAndCloseAsync(inputStream, outStream).AsTask().Wait();
                        }
                    }
                }

                // Complete Endpoint print job.
                jobStatus = PrintWorkflowSubmittedStatus.Succeeded;

            }
            finally
            {
                args.CompleteJob(jobStatus);
                taskDeferral.Complete();
            }
        }

        /// <summary>
        /// Configure the watermarker with settings from LocalStorage (saved by the UI)
        /// </summary>
        private XpsPageWatermarker ConfigureWatermarker()
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
    }
}
