using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Activation;
using Windows.Graphics.Printing.PrintSupport;
using Windows.Graphics.Printing.Workflow;

namespace PrintSupportApplication
{
    /// <summary>
    /// Provides application-specific behavior to supplement the default Application class.
    /// </summary>
    public partial class App : Application
    {
        private Window? m_window;

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
            // Get activation arguments using AppInstance to determine activation type
            var instanceArgs = AppInstance.GetActivatedEventArgs();

            if (instanceArgs.Kind == ActivationKind.PrintSupportSettingsUI)
            {
                // Settings UI activation
                HandleSettingsActivation(instanceArgs as PrintSupportSettingsActivatedEventArgs);
            }
            else if (instanceArgs.Kind == ActivationKind.PrintSupportJobUI)
            {
                // Job UI activation
                HandleJobActivation(instanceArgs as PrintWorkflowJobActivatedEventArgs);
            }
            else
            {
                // Normal launch - show main page
                HandleNormalLaunch(args);
            }
        }

        private void HandleNormalLaunch(Microsoft.UI.Xaml.LaunchActivatedEventArgs args)
        {
            m_window = new Window();
            var rootFrame = new Frame();

            // Navigate to UserLaunchMainPage
            rootFrame.Navigate(typeof(UserLaunchMainPage), args.Arguments);

            m_window.Content = rootFrame;
            m_window.Activate();
        }

        private void HandleSettingsActivation(PrintSupportSettingsActivatedEventArgs? settingsArgs)
        {
            if (settingsArgs == null) return;

            m_window = new Window();
            var rootFrame = new Frame();

            // Navigate to SettingsActivatedMainPage
            // Pass the activation args as navigation parameter
            rootFrame.Navigate(typeof(SettingsActivatedMainPage), settingsArgs);

            m_window.Content = rootFrame;
            m_window.Activate();
        }

        private void HandleJobActivation(PrintWorkflowJobActivatedEventArgs? jobArgs)
        {
            if (jobArgs == null) return;

            m_window = new Window();
            var rootFrame = new Frame();

            // Navigate to JobActivatedMainPage
            // Pass the activation args as navigation parameter
            rootFrame.Navigate(typeof(JobActivatedMainPage), jobArgs);

            m_window.Content = rootFrame;
            m_window.Activate();
        }
    }
}
