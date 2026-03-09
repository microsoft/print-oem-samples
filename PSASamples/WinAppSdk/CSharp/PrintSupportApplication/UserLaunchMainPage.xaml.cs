using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Navigation;

namespace PrintSupportApplication
{
    /// <summary>
    /// Main page shown when user launches the application normally.
    /// Contains navigation to App Info, IPP Printer URL configuration, and Virtual Printers.
    /// </summary>
    public sealed partial class UserLaunchMainPage : Page
    {
        private string? printerName;

        public UserLaunchMainPage()
        {
            InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            if (e.Parameter != null && !string.IsNullOrEmpty((string)e.Parameter))
            {
                printerName = (string)e.Parameter;
            }
        }

        private void NavViewItemInvoked(NavigationView sender, NavigationViewItemInvokedEventArgs args)
        {
            string? tag = args.InvokedItemContainer.Tag as string;
            if (tag == "GetIppPrinterUrl")
            {
                contentFrame.Navigate(typeof(GetIppPrinterUrl), printerName);
            }
            else if (tag == "VirtualPrinters")
            {
                contentFrame.Navigate(typeof(VirtualPrintersPage), printerName);
            }
            else
            {
                contentFrame.Navigate(typeof(AppInfo), printerName);
            }
        }

        private void NavViewLoaded(object sender, RoutedEventArgs e)
        {
            // NavView doesn't load any page by default, so load the first page.
            NavigationViewControl.SelectedItem = NavigationViewControl.MenuItems[0];

            // Navigate to AppInfo by default
            contentFrame.Navigate(typeof(AppInfo), printerName);
        }
    }
}
