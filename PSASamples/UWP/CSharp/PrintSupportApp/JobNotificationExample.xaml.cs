using Windows.Graphics.Printing.Workflow;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

namespace PrintSupportApp
{
    public sealed partial class JobNotificationExample : Page
    {
        public JobNotificationExample()
        {
            InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            if (e.Parameter is PrintWorkflowPrinterJobStatus status)
            {
                DisplayJobStatus(status);
            }
        }

        private void DisplayJobStatus(PrintWorkflowPrinterJobStatus status)
        {
            JobStatus.Text = status.ToString();
        }
    }
}