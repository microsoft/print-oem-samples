using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.ApplicationModel.Background;
using Windows.Graphics.Printing.PrintSupport;

namespace Tasks
{
    public sealed partial class PrintSupportExtensionBackgroundTask : IBackgroundTask
    {
        private BackgroundTaskDeferral Deferral;

        public void Run(IBackgroundTaskInstance taskInstance)
        {
            Deferral = taskInstance.GetDeferral();
            taskInstance.Canceled += (sender, reason) => { Deferral.Complete(); };

            if (taskInstance.TriggerDetails is PrintSupportExtensionTriggerDetails extensionDetails)
            {
                var session = extensionDetails.Session;
                session.PrintDeviceCapabilitiesChanged += OnSessionPrintDeviceCapabilitiesChanged;
                session.PrintTicketValidationRequested += OnSessionPrintTicketValidationRequested;
                session.CommunicationErrorDetected += OnSessionCommincationErrorDetected;
                session.Start();
            }
        }

        private void OnSessionPrintTicketValidationRequested(PrintSupportExtensionSession sender, PrintSupportPrintTicketValidationRequestedEventArgs args)
        {
            args.SetPrintTicketValidationStatus(WorkflowPrintTicketValidationStatus.Resolved);
            args.GetDeferral().Complete();
        }

        private void OnSessionPrintDeviceCapabilitiesChanged(PrintSupportExtensionSession sender, PrintSupportPrintDeviceCapabilitiesChangedEventArgs args)
        {
            var pdc = args.GetCurrentPrintDeviceCapabilities();
            var xml = pdc.GetXml();
            args.GetDeferral().Complete();
        }

        private void OnSessionCommincationErrorDetected(PrintSupportExtensionSession sender, PrintSupportCommunicationErrorDetectedEventArgs args)
        {
            args.GetDeferral().Complete();
        }
    }
}
