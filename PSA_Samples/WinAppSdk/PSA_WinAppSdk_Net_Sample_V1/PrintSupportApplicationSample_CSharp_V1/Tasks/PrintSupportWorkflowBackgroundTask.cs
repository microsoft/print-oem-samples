using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.ApplicationModel.Background;
using Windows.Devices.Printers;
using Windows.Graphics.Printing.PrintSupport;
using Windows.Graphics.Printing.Workflow;
using Windows.Storage.Streams;

namespace Tasks
{
    public sealed partial class PrintSupportWorkflowBackgroundTask : IBackgroundTask
    {
        private BackgroundTaskDeferral Deferral;

        public void Run(IBackgroundTaskInstance taskInstance)
        {
            Deferral = taskInstance.GetDeferral();
            if (taskInstance.TriggerDetails is PrintWorkflowJobTriggerDetails jobDetails)
            {
                var session = jobDetails.PrintWorkflowJobSession;
                session.JobStarting += OnJobStarting;
                session.PdlModificationRequested += OnPdlModificationRequested;
                session.Start();
            }
        }

        private void OnJobStarting(PrintWorkflowJobBackgroundSession sender, PrintWorkflowJobStartingEventArgs args)
        {
            args.DisableIppCompressionForJob();
            args.GetDeferral().Complete();
        }

        private async void OnPdlModificationRequested(PrintWorkflowJobBackgroundSession sender, PrintWorkflowPdlModificationRequestedEventArgs args)
        {
            /*
            var uiResult = await args.UILauncher.LaunchAndCompleteUIAsync();
            if (uiResult != PrintWorkflowUICompletionStatus.Completed)
            {
                if (uiResult == PrintWorkflowUICompletionStatus.UserCanceled)
                {
                    args.Configuration.AbortPrintFlow(PrintWorkflowJobAbortReason.UserCanceled);
                }
            }
            */

            var documentFormat = GetDocumentFormat(args.PrinterJob.Printer);
            var targetStream = args.CreateJobOnPrinter(documentFormat);
            var inputStream = args.SourceContent.GetInputStream();
            if (args.SourceContent.ToString() == "application/oxps")
            {
                var pdlConverter = args.GetPdlConverter(PrintWorkflowPdlConversionType.XpsToPdf);
                await pdlConverter.ConvertPdlAsync(args.PrinterJob.GetJobPrintTicket(), inputStream, targetStream.GetOutputStream());
                targetStream.CompleteStreamSubmission(PrintWorkflowSubmittedStatus.Succeeded);
            }
            else
            {
                await RandomAccessStream.CopyAndCloseAsync(inputStream, targetStream.GetOutputStream());
                targetStream.CompleteStreamSubmission(PrintWorkflowSubmittedStatus.Succeeded);
            }

            args.GetDeferral().Complete();
        }

        private string GetDocumentFormat(IppPrintDevice printer)
        {
            var requestedAttributes = new List<string> { "document-format-default", "document-format-supported" };
            var attributes = printer.GetPrinterAttributes(requestedAttributes);
            var documentFormat = attributes["document-format-default"].GetKeywordArray().First().ToString();
            if (!HasPdlConverter(documentFormat))
            {
                var documentFormats = attributes["document-format-supported"];
                foreach (var format in documentFormats.GetKeywordArray())
                {
                    if (HasPdlConverter(format.ToString()))
                    {
                        documentFormat = format.ToString();
                        break;
                    }
                }
            }

            return documentFormat;
        }

        private bool HasPdlConverter(string documentFormat)
        {
            return (documentFormat == "image/pwg-raster" || documentFormat == "application/PCLm" || documentFormat == "application/pdf" || documentFormat == "image/tiff");
        }
    }
}
