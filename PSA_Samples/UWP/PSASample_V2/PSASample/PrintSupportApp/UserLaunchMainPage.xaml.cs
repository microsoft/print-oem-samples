using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace PrintSupportApp
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class UserLaunchMainPage : Page
    {
        private String printerName;

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

        private void NavViewItemInvoked(Microsoft.UI.Xaml.Controls.NavigationView sender, Microsoft.UI.Xaml.Controls.NavigationViewItemInvokedEventArgs args)
        {
            string tag = args.InvokedItemContainer.Tag as string;
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
            contentFrame.Navigate(typeof(AppInfo), printerName);
        }
    }
}
