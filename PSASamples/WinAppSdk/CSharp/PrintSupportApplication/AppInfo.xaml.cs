using System;
using System.Collections.Generic;
using Windows.ApplicationModel;
using Windows.Devices.Enumeration;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Navigation;

namespace PrintSupportApplication
{
    public sealed partial class AppInfo : Page
    {
        public AppInfo()
        {
            InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            PackageFamilyNameTextBox.Text = Package.Current.Id.FamilyName;
            FindAssociatedPrinter();
        }

        /// <summary>
        /// Queries all installed printer devices and displays their names.
        /// </summary>
        private async void FindAssociatedPrinter()
        {
            const string printerDeviceClassGuid = "{4d36e979-e325-11ce-bfc1-08002be10318}";
            const string PRINTER_NAME_PROPERTY = "{f01fac5d-e5f6-485f-a8c6-27446425998c} 5";

            string aqsFilter = $"System.Devices.ClassGuid:=\"{printerDeviceClassGuid}\"";

            try
            {
                var devices = await DeviceInformation.FindAllAsync(
                    aqsFilter,
                    new List<string> { PRINTER_NAME_PROPERTY },
                    DeviceInformationKind.Device);

                var printerNames = new List<string>();

                foreach (var deviceInfo in devices)
                {
                    string? name = null;

                    if (deviceInfo?.Properties.ContainsKey(PRINTER_NAME_PROPERTY) == true)
                    {
                        name = deviceInfo.Properties[PRINTER_NAME_PROPERTY]?.ToString();
                    }

                    if (string.IsNullOrEmpty(name))
                    {
                        name = deviceInfo?.Name;
                    }

                    if (!string.IsNullOrEmpty(name))
                    {
                        printerNames.Add(name);
                    }
                }

                PrinterNameTextBox.Text = printerNames.Count > 0
                    ? string.Join("\r\n", printerNames)
                    : "No printers found";
            }
            catch
            {
                PrinterNameTextBox.Text = "Printer query failed";
            }
        }
    }
}
