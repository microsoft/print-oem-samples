using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Navigation;
using Windows.Graphics.Printing.PrintSupport;

namespace PrintSupportApplication
{
    /// <summary>
    /// Main page shown when the app is activated for print settings UI.
    /// Displays navigation to print ticket manipulation and other settings.
    /// </summary>
    public sealed partial class SettingsActivatedMainPage : Page
    {
        public PrintSupportSettingsUISession? Session { get; set; }

        public SettingsActivatedMainPage()
        {
            InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            if (e.Parameter is PrintSupportSettingsActivatedEventArgs settingsActivatedEventArgs)
            {
                Session = settingsActivatedEventArgs.Session;

                string openedFrom = Session.LaunchKind.ToString();
                PrintTicketManipulationExampleNavigationItem.Content = $"Print Ticket Manipulation Example ({openedFrom})";
            }
        }

        private void NavViewItemInvoked(NavigationView sender, NavigationViewItemInvokedEventArgs args)
        {
            string? tag = args.InvokedItemContainer.Tag as string;
            if (tag == "PrintTicketManipulationExample")
            {
                contentFrame.Navigate(typeof(PrintTicketManipulationExample), Session);
            }
        }

        private void NavViewLoaded(object sender, RoutedEventArgs e)
        {
            // NavView doesn't load any page by default, so load the first page.
            NavigationViewControl.SelectedItem = NavigationViewControl.MenuItems[0];
            contentFrame.Navigate(typeof(PrintTicketManipulationExample), Session);
        }
    }
}
