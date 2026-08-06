using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using System;
using System.IO;
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

        // Keeps the modal "More settings" window host alive for the duration of the
        // settings activation. The handler also self-roots via a GCHandle, but holding
        // a reference here keeps the ownership obvious.
        private ModalWindowHandler? m_settingsModalWindow;

        /// <summary>
        /// Stores the settings activation args so other pages can access the printer context.
        /// </summary>
        public static PrintSupportSettingsActivatedEventArgs? SettingsActivationArgs { get; private set; }

        /// <summary>
        /// Initializes the singleton application object.  This is the first line of authored code
        /// executed, and as such is the logical equivalent of main() or WinMain().
        /// </summary>
        public App()
        {
            this.InitializeComponent();

            // Handle re-activation when the app is already running
            // (e.g., L3 settings page activates the PSA while it's open)
            Microsoft.Windows.AppLifecycle.AppInstance.GetCurrent().Activated += OnAppActivated;
        }

        private void OnAppActivated(object? sender, Microsoft.Windows.AppLifecycle.AppActivationArguments args)
        {
            if (args.Kind == Microsoft.Windows.AppLifecycle.ExtendedActivationKind.PrintSupportSettingsUI)
            {
                var settingsArgs = args.Data as PrintSupportSettingsActivatedEventArgs;
                if (settingsArgs != null)
                {
                    SettingsActivationArgs = settingsArgs;
                }
            }
        }

        /// <summary>
        /// Invoked when the application is launched.
        /// </summary>
        /// <param name="args">Details about the launch request and process.</param>
        protected override void OnLaunched(Microsoft.UI.Xaml.LaunchActivatedEventArgs args)
        {
            // Use the Windows App SDK API for activation detection (recommended for WinUI 3)
            var appActivationArgs = Microsoft.Windows.AppLifecycle.AppInstance.GetCurrent().GetActivatedEventArgs();
            var activationKind = appActivationArgs.Kind;

            // DIAGNOSTIC: log every activation to disk and OutputDebugString so we can see what
            // kind the OS actually sends for printer install / PDC regeneration.
            try
            {
                var localState = Windows.Storage.ApplicationData.Current.LocalFolder.Path;
                var logPath = System.IO.Path.Combine(localState, "app-activation.log");
                var dataType = appActivationArgs.Data?.GetType().FullName ?? "(null)";
                var msg = $"[{DateTime.UtcNow:o}] OnLaunched: Kind={activationKind}, DataType={dataType}\r\n";
                System.IO.File.AppendAllText(logPath, msg);
                System.Diagnostics.Debug.WriteLine($"[PSA.App] {msg.TrimEnd()}");
            }
            catch { /* never let logging crash the app */ }

            if (activationKind == Microsoft.Windows.AppLifecycle.ExtendedActivationKind.PrintSupportSettingsUI)
            {
                // Settings UI activation
                HandleSettingsActivation(appActivationArgs.Data as PrintSupportSettingsActivatedEventArgs);
            }
            else if (activationKind == Microsoft.Windows.AppLifecycle.ExtendedActivationKind.PrintSupportJobUI)
            {
                // Job UI activation
                HandleJobActivation(appActivationArgs.Data as PrintWorkflowJobActivatedEventArgs);
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
            SettingsActivationArgs = settingsArgs;

            var rootFrame = new Frame();

            // Navigate to SettingsActivatedMainPage
            // Pass the activation args as navigation parameter
            rootFrame.Navigate(typeof(SettingsActivatedMainPage), settingsArgs);

            // Host the settings UI in a modal window owned by the print dialog's owner
            // window, mirroring the C++/WinRT sample's ModalWindowHandler. A plain
            // WinUI Window is neither owned by nor modal to the caller, so the "More
            // settings" UI would otherwise appear as an independent top-level window.
            // OwnerWindowId is a Windows.UI.WindowId; Win32Interop works with
            // Microsoft.UI.WindowId. Both wrap the same raw HWND value, so convert
            // through it (mirrors the C++ sample's { OwnerWindowId().Value } init).
            var ownerWindowId = new Microsoft.UI.WindowId { Value = settingsArgs.OwnerWindowId.Value };
            m_settingsModalWindow = new ModalWindowHandler(ownerWindowId, rootFrame);
            m_settingsModalWindow.Activate();
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
