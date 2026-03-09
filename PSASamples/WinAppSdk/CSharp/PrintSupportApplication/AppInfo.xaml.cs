using System;
using Windows.ApplicationModel;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Navigation;

namespace PrintSupportApplication
{
    public sealed partial class AppInfo : Page
    {

        public string? printerName;

        public AppInfo()
        {
            InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            PackageFamilyNameTextBox.Text = Package.Current.Id.FamilyName;

            // This is the Printer Name received from Settings Launch.
            // Proceed to connect to this device via New PDL Passthrough / IPPDevice APIs
            if (e.Parameter != null && !string.IsNullOrEmpty((string)e.Parameter))
            {
                printerName = (string)e.Parameter;
                PrinterNameTextBox.Text = printerName;
                PrinterNameTextBox.Visibility = Microsoft.UI.Xaml.Visibility.Visible;
            }
        }
    }
}
