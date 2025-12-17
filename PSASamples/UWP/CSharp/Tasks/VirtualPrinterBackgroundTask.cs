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
                    // Check if we need to launch UI for XPS to PDF conversion
                    bool shouldLaunchUI = targetFile.FileType == ".pdf" && 
                                          sourceContent.ContentType.ToLower() == "application/oxps";

                    if (shouldLaunchUI)
                    {
                        // Launch UI to allow user to apply watermarks or other modifications before conversion
                        PrintWorkflowUILauncher uiLauncher = args.UILauncher;
                        if (uiLauncher != null)
                        {
                            // Launch the UI and wait for it to complete
                            // The UI will be handled by JobActivatedMainPage.OnVirtualSessionPdlDataAvailable
                            // which navigates to WatermarkManipulationExample
                            uiLauncher.LaunchAndCompleteUIAsync().AsTask().Wait();
                        }
                    }

                    IRandomAccessStream outputStream = targetFile.OpenAsync(FileAccessMode.ReadWrite).AsTask().Result;
                    var inputStream = sourceContent.GetInputStream();
                    using (var outStream = outputStream.GetOutputStreamAt(0))
                    {

                        if (targetFile.FileType == ".pdf")
                        {
                            if (sourceContent.ContentType.ToLower() == "application/oxps")
                            {
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
    }
}
