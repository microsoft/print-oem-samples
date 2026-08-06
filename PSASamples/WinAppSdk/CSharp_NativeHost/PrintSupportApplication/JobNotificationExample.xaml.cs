using Windows.Graphics.Printing.Workflow;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Navigation;

namespace PrintSupportApplication
{
    public sealed partial class JobNotificationExample : Page
    {
        public JobNotificationExample()
        {
            InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            if (e.Parameter is PrintWorkflowJobNotificationEventArgs args)
            {
                OnSessionJobNotification(args);
            }
        }

        private void OnSessionJobNotification(PrintWorkflowJobNotificationEventArgs args)
        {
            var printJob = args.PrinterJob;
            var status = printJob.GetJobStatus();
            JobStatus.Text = status.ToString();
        }
    }
}
