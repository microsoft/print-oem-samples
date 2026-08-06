using System;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Navigation;
using Windows.Foundation;
using Windows.Graphics.Printing.Workflow;

namespace PrintSupportApplication
{
    /// <summary>
    /// Main page shown when the app is activated for print job UI.
    /// Handles job notifications and PDL data availability events.
    /// </summary>
    public sealed partial class JobActivatedMainPage : Page
    {
        public PrintWorkflowJobUISession? Session { get; set; }

        private static Deferral? PdlDataAvailableDeferral { get; set; }

        private static Deferral? SessionJobNotificationDeferral { get; set; }

        public JobActivatedMainPage()
        {
            InitializeComponent();
        }

        public static void CloseDialog()
        {
            if (SessionJobNotificationDeferral != null)
            {
                SessionJobNotificationDeferral.Complete();
            }

            if (PdlDataAvailableDeferral != null)
            {
                PdlDataAvailableDeferral.Complete();
            }

            Application.Current.Exit();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            if (e.Parameter is PrintWorkflowJobActivatedEventArgs jobActivatedEventArgs)
            {
                Session = jobActivatedEventArgs.Session;
                Session.JobNotification += OnSessionJobNotification;
                Session.PdlDataAvailable += OnSessionPdlDataAvailable;
                Session.VirtualPrinterUIDataAvailable += OnVirtualSessionPdlDataAvailable;
                Session.Start();
            }
        }

        private void OnSessionJobNotification(PrintWorkflowJobUISession sender, PrintWorkflowJobNotificationEventArgs args)
        {
            SessionJobNotificationDeferral = args.GetDeferral();

            // Note: OnSessionJobNotification is not called on the UI thread, so we must use DispatcherQueue to run any code that updates the UI.
            DispatcherQueue.TryEnqueue(() =>
            {
                SetNavigationViewSelectedItem("JobNotificationExample");
                contentFrame.Navigate(typeof(JobNotificationExample), args);
            });
        }

        private void OnSessionPdlDataAvailable(PrintWorkflowJobUISession sender, PrintWorkflowPdlDataAvailableEventArgs args)
        {
            PdlDataAvailableDeferral = args.GetDeferral();

            // Note: OnSessionPdlDataAvailable is not called on the UI thread, so we must use DispatcherQueue to run any code that updates the UI.
            DispatcherQueue.TryEnqueue(() =>
            {
                SetNavigationViewSelectedItem("WatermarkManipulationExample");
                contentFrame.Navigate(typeof(WatermarkManipulationExample), args);
            });
        }

        private void OnVirtualSessionPdlDataAvailable(PrintWorkflowJobUISession sender, PrintWorkflowVirtualPrinterUIEventArgs args)
        {
            PdlDataAvailableDeferral = args.GetDeferral();

            // Note: OnVirtualSessionPdlDataAvailable is not called on the UI thread, so we must use DispatcherQueue to run any code that updates the UI.
            DispatcherQueue.TryEnqueue(() =>
            {
                SetNavigationViewSelectedItem("WatermarkManipulationExample");
                contentFrame.Navigate(typeof(WatermarkManipulationExample), args);
            });
        }

        // Helper method to update the selected menu item.
        private void SetNavigationViewSelectedItem(string tag)
        {
            foreach (FrameworkElement item in NavigationViewControl.MenuItems)
            {
                if (item.Tag?.ToString() == tag)
                {
                    NavigationViewControl.SelectedItem = item;
                    break;
                }
            }
        }
    }
}
