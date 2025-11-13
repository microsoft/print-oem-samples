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
using Windows.Devices.Enumeration;
using System.Threading.Tasks;
using Windows.ApplicationModel;

namespace PrintSupportApp
{
    /// <summary>
    /// The very basic sample of getting the IPP Printer Url by
    /// quering the System.Devices.Aep.DeviceAddress device property with
    /// Windows.Devices.Enumeration's DeviceInformation.FindAllAsync.
    /// </summary>
    public sealed partial class GetIppPrinterUrl : Page
    {

        private string printerNameFromSettings;
        public GetIppPrinterUrl()
        {
            InitializeComponent();
            DataContext = this;
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            if (e.Parameter != null && !String.IsNullOrEmpty((string)e.Parameter))
            {
                printerNameFromSettings = (string)e.Parameter;
            }
        }
        public string Manufacturer { get; set; }

        private bool QueryIsStarted
        {
            set
            {
                if (value)
                {
                    this.StartQueryButton.IsEnabled = false;
                    QueryProgressBar.Visibility = Visibility.Visible;
                    QueryResultListView.Items.Clear();
                    IppPrinters.Clear();
                }
                else
                {
                    this.StartQueryButton.IsEnabled = true;
                    QueryProgressBar.Visibility = Visibility.Collapsed;
                }
            }
        }

        /// <summary>
        /// Helper class containing PrinterName to PrinterUrl association
        /// </summary>
        private class PrinterInfo
        {
            public string PrinterName { get; set; }
            public string PrinterUrl { get; set; }
        }
        Dictionary<string, PrinterInfo> IppPrinters = new Dictionary<string, PrinterInfo>();

        // PSA APIv2 is offering superior ways to communicate with IPP Printers and is available on the latest versions of Windows
        static bool UsePsaV2Api { get { return true; } }
        
        /// <summary>
        /// This method is utilizing the Windows.Device.Enumeration API, by querying for
        /// the Installed Printer Devices, supporting the IPP (IPP Printers)
        /// Note it does not include the IPP Printers shared from the Print Server.
        /// </summary>
        /// <returns></returns>
        private void QueryLocalIppPrinters()
        {
            const string printerDeviceClassGuid = "{4d36e979-e325-11ce-bfc1-08002be10318}";
            const string ippMicrosoftClassDriverId = "1284_CID_MS_IPP";
            const string universalPrintClassDriverId = "1284_CID_MS_MPS";
            const string PSA_AUMID_PROPERTY = "{A925764B-88E0-426D-AFC5-B39768BE59EB} 1";
            string currAppPFN = Package.Current.Id.FamilyName;


            // Get IPP Printer Devices
            string aqsIppPrinterDeviceFilter =
                  $"System.Devices.ClassGuid:=\"{printerDeviceClassGuid}\""
                + $" AND System.Devices.CompatibleIds:~~\"{ippMicrosoftClassDriverId}\""
                + $" OR System.Devices.CompatibleIds:~~\"{universalPrintClassDriverId}\"";

            DeviceInformation.FindAllAsync(
                aqsIppPrinterDeviceFilter,
                new List<string> { PSA_AUMID_PROPERTY, "{f01fac5d-e5f6-485f-a8c6-27446425998c} 5" },
                DeviceInformationKind.Device).AsTask().ContinueWith(queryTask =>
                {
                    if (queryTask.Status == TaskStatus.RanToCompletion)
                    {
                        foreach (var deviceInfo in queryTask.Result)
                        {
                            if (deviceInfo != null && deviceInfo.Properties.ContainsKey("{f01fac5d-e5f6-485f-a8c6-27446425998c} 5"))
                            {
                                var printerName = GetPropertyAsString(deviceInfo, "{f01fac5d-e5f6-485f-a8c6-27446425998c} 5");
                                var psaPFN = GetPropertyAsString(deviceInfo, PSA_AUMID_PROPERTY);
                                var psaMatch = (psaPFN == null) ? false : psaPFN.Contains(currAppPFN);

                                // Note! Windows.Devices.Printers.IppPrintDevice.FromPrinterName is only available in PSAv2
                                // if you need to get Uri for IPP printers for PSAv1, please use QuerySoftwarePrinterDevice, instead of IppPrintDevice
                                if (UsePsaV2Api)
                                {
                                    var ippPrintDevice = Windows.Devices.Printers.IppPrintDevice.FromPrinterName(printerName);
                                    var printerUrl = ippPrintDevice.PrinterUri.ToString();

                                    if (IppPrinters.TryAdd(printerName, new PrinterInfo { PrinterName = printerName, PrinterUrl = printerUrl }))
                                    {
                                        AddPrinterQueryResultView(printerName, printerUrl, "True" /* isPaired */, psaMatch);
                                    }
                                }
                                else
                                {
                                    IppPrinters.TryAdd(printerName, new PrinterInfo { PrinterName = printerName });
                                }
                            }
                        }

                        if (!UsePsaV2Api)
                        {
                            // Please note, Windows.Devices.Printers.IppPrintDevice.FromPrinterName is preferred method starting PSAv2
                            // You should only resort to approach implemented in QuerySoftwarePrinterDevice when PSAv2 is not availalable
                            QuerySoftwarePrinterDevice();
                        }
                    }
                    else
                    {
                        throw new Exception("Ipp Printer query has failed");
                    }
                }, System.Threading.Tasks.TaskScheduler.FromCurrentSynchronizationContext());
        }

        /// <summary>
        /// Quiring the software printer devices, filtering the results 
        /// by the known printer names
        /// </summary>
        private void QuerySoftwarePrinterDevice()
        {
            // Print Software Device Query
            string aqsPrinterSoftwareDeviceFilter = "System.Devices.ClassGuid:=\"{62f9c741-b25a-46ce-b54c-9bccce08b6f2}\"";

            DeviceInformation.FindAllAsync(
                aqsPrinterSoftwareDeviceFilter,
                new List<string> { "System.ItemNameDisplay", "System.Devices.Aep.DeviceAddress" },
                DeviceInformationKind.Device).AsTask().ContinueWith(queryTask =>
                {
                    if (queryTask.Status == TaskStatus.RanToCompletion)
                    {
                        ShowIppPrinterUrls(queryTask.Result);
                    }
                    else
                    {
                        QueryResultListView.Items.Add("Query has failed");
                    }
                    QueryResultListView.Items.Add("Software printer device query completed.");
                }, System.Threading.Tasks.TaskScheduler.FromCurrentSynchronizationContext());
        }
        private void QueryIppPrintersAssociationEndpointInfo()
        {
            string aqsPrintAepFilter = "System.Devices.Aep.Category:~~\"PrintFax.Printer\"" +
                " AND System.Devices.Aep.ProtocolId:=\"{9f73fafd-2343-4bf9-a532-ad20e2674bea}\"" + // filter by IPP protocol id
                (!String.IsNullOrEmpty(Manufacturer) ? $" AND System.Devices.Aep.Manufacturer:~~\"{Manufacturer}\"" : String.Empty);

            DeviceInformation.FindAllAsync(
                aqsPrintAepFilter,
                new List<string> { "System.Devices.Aep.DeviceAddress", "System.Devices.Aep.Category",
                    "System.Devices.Aep.Manufacturer", "System.Devices.Aep.ProtocolId" },
                DeviceInformationKind.AssociationEndpoint).AsTask().ContinueWith(queryTask =>
                {
                    if (queryTask.Status == TaskStatus.RanToCompletion)
                    {
                        ShowIppPrinterUrls(queryTask.Result);
                    }
                    else
                    {
                        QueryResultListView.Items.Add("Query has failed");
                    }
                    QueryIsStarted = false;
                    QueryResultListView.Items.Add("AssociationEndpoint query completed.");
                }, System.Threading.Tasks.TaskScheduler.FromCurrentSynchronizationContext());
        }
        private void OnStartQueryButtonClicked(object sender, RoutedEventArgs e)
        {
            QueryIsStarted = true;
            // Query installed printers, which will include 'Directed Discovered' IPP  Printers
            QueryLocalIppPrinters();

            // Query known IPP Printers' URIs via Association Endpoint Service
            QueryIppPrintersAssociationEndpointInfo();
        }

        private string GetPropertyAsString(DeviceInformation deviceInfo, string propertyName)
        {
            try
            {
                object objectPropertyValue = null;
                if (deviceInfo.Properties.TryGetValue(propertyName, out objectPropertyValue))
                {
                    return objectPropertyValue?.ToString();
                }
            }
            catch (Exception) { }
            return null;
        }

        private void AddPrinterQueryResultView(string printerName, string printerUrl, string isPaired, bool psaMatch = false)
        {
            ListViewItem item = new ListViewItem();
            item.Content = $"Printer: '{printerName}', Url: '{printerUrl}', IsPaired '{isPaired}', PSAMatched '{psaMatch}'";

            // Highlighting Printer that was launched from settings
            if (printerNameFromSettings != null && printerName.Contains(printerNameFromSettings))
            {
                item.Background = new SolidColorBrush(Windows.UI.Colors.LightBlue);
            }

            // Highlighting Printer that match with the current PSA
            if (psaMatch)
            {
                item.Background = new SolidColorBrush(Windows.UI.Colors.Azure);
            }

            QueryResultListView.Items.Add(item);
        }

        // Parse query result and update UI
        private void ShowIppPrinterUrls(DeviceInformationCollection deviceInfos)
        {
            foreach (var deviceInfo in deviceInfos)
            {
                // Selecting only the records containing both printer name and DeviceAddress
                if (deviceInfo.Properties.ContainsKey("System.ItemNameDisplay") &&
                        deviceInfo.Properties.ContainsKey("System.Devices.Aep.DeviceAddress"))
                {
                    var printerName = GetPropertyAsString(deviceInfo, "System.ItemNameDisplay");
                    var printerUri = GetPropertyAsString(deviceInfo, "System.Devices.Aep.DeviceAddress");
                    // Note isPaired will be always false for 10X by design
                    var isPaired = GetPropertyAsString(deviceInfo, "System.Devices.Aep.IsPaired");
                    PrinterInfo printerInfo = null;
                    if (IppPrinters.TryGetValue(printerName, out printerInfo))
                    {
                        if (String.IsNullOrEmpty(printerInfo.PrinterUrl))
                        {
                            printerInfo.PrinterUrl = printerUri;
                            AddPrinterQueryResultView(printerName, printerUri, isPaired);
                        }
                    }
                }
            }
        }
    }
}
