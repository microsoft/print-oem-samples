using Microsoft.UI.Xaml;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Activation;
using Windows.Graphics.Printing.PrintSupport;
using Windows.Graphics.Printing.Workflow;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace PrintSupportApplication
{
    /// <summary>
    /// Provides application-specific behavior to supplement the default Application class.
    /// </summary>
    public partial class App : Application
    {
        /// <summary>
        /// Initializes the singleton application object.  This is the first line of authored code
        /// executed, and as such is the logical equivalent of main() or WinMain().
        /// </summary>
        public App()
        {
            this.InitializeComponent();
        }

        /// <summary>
        /// Invoked when the application is launched.
        /// </summary>
        /// <param name="args">Details about the launch request and process.</param>
        protected override void OnLaunched(Microsoft.UI.Xaml.LaunchActivatedEventArgs args)
        {
            m_window = new MainWindow();
            m_window.Activate();

            var instanceArgs = AppInstance.GetActivatedEventArgs();
            if (instanceArgs.Kind == ActivationKind.PrintSupportSettingsUI)
            {
                var activatedArgs = instanceArgs as PrintSupportSettingsActivatedEventArgs;
            }
            else if (instanceArgs.Kind == ActivationKind.PrintSupportJobUI)
            {
                var activatedArgs = instanceArgs as PrintWorkflowJobActivatedEventArgs;
            }
        }

        private Window? m_window;
    }
}
