using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.ApplicationModel.Background;
using Windows.Data.Xml.Dom;
using Windows.Foundation.Metadata;
using Windows.Graphics.Printing.PrintSupport;
using System.Collections.Generic;
using Windows.Graphics.Printing.Workflow;
using Windows.Graphics.Printing.PrintTicket;


using Windows.Devices.Printers;
using System.IO;
using Windows.Storage.Streams;
using Windows.Storage;
using Windows.Devices.Input;

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
                PrintWorkflowPdlSourceContent soruceContent = args.SourceContent;
                StorageFile targetFile = args.GetTargetFileAsync().AsTask().Result;
                IRandomAccessStream outputStream = targetFile.OpenAsync(FileAccessMode.ReadWrite).AsTask().Result;
                var inputStream = soruceContent.GetInputStream();
                using (var outStream = outputStream.GetOutputStreamAt(0))
                {
                    if (printDevice.PrinterUri.AbsolutePath == "print-to-pdf")
                    {
                        // Get XPS to PDF PDL converter.
                        PrintWorkflowPdlConverter converter = args.GetPdlConverter(PrintWorkflowPdlConversionType.XpsToPdf);
                        // Convert XPS to PDF and write contents to outputStream.
                        var printTicket = args.GetJobPrintTicket();

                        converter.ConvertPdlAsync(printTicket, inputStream, outStream).AsTask().Wait();
                    }
                    else if (printDevice.PrinterUri.AbsolutePath == "print-to-ps")
                    {
                        RandomAccessStream.CopyAndCloseAsync(inputStream, outStream).AsTask().Wait();
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
    }
}
