using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using Windows.ApplicationModel;
#if !VIRTUAL_PRINTER_DISABLED
using Windows.Devices.Printers;
#endif
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Navigation;

namespace PrintSupportApplication
{
    public sealed partial class VirtualPrintersPage : Page
    {
        public ObservableCollection<VirtualPrinterInfo> VirtualPrinters { get; set; }

        public VirtualPrintersPage()
        {
            InitializeComponent();
            VirtualPrinters = new ObservableCollection<VirtualPrinterInfo>();
            VirtualPrintersListView.ItemsSource = VirtualPrinters;
            this.Loaded += VirtualPrintersPage_Loaded;
        }

        private void VirtualPrintersPage_Loaded(object sender, RoutedEventArgs e)
        {
#if VIRTUAL_PRINTER_DISABLED
            ShowFeatureNotAvailable();
#else
            LoadVirtualPrinters();
#endif
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
        }

#if VIRTUAL_PRINTER_DISABLED
        private async void ShowFeatureNotAvailable()
        {
            await ShowMessageDialog("Feature Not Available",
                "Virtual Printer management is not available in this version.\n\n" +
                "The Windows.Devices.Printers.VirtualPrinter APIs exist in Windows SDK 10.0.26100.0 " +
                "but are not yet projected in the Microsoft.Windows.SDK.NET package.\n\n" +
                "This feature will be enabled in a future update when Microsoft updates the SDK projections.");
        }

        private async void OnAddVirtualPrinterClicked(object sender, RoutedEventArgs e)
        {
            await ShowFeatureNotAvailableDialog();
        }

        private void OnRefreshClicked(object sender, RoutedEventArgs e)
        {
            // Do nothing
        }

        private async void VirtualPrintersListView_ItemClick(object sender, ItemClickEventArgs e)
        {
            await ShowFeatureNotAvailableDialog();
        }

        private async void OnPrinterItemClicked(object sender, ItemClickEventArgs e)
        {
            await ShowFeatureNotAvailableDialog();
        }

        private async void OnRemovePrinterClicked(object sender, RoutedEventArgs e)
        {
            await ShowFeatureNotAvailableDialog();
        }

        private async System.Threading.Tasks.Task ShowFeatureNotAvailableDialog()
        {
            await ShowMessageDialog("Feature Not Available",
                "Virtual Printer APIs are not available. See migration documentation for details.");
        }

#else
        // Full implementation when VirtualPrinter APIs are available
        private async void LoadVirtualPrinters()
        {
            try
            {
                LoadingProgressRing.IsActive = true;
                LoadingProgressRing.Visibility = Visibility.Visible;
                VirtualPrintersListView.Visibility = Visibility.Collapsed;

                VirtualPrinters.Clear();

                string currentAppPackageFamilyName = Package.Current.Id.FamilyName;

                var printers = VirtualPrinterManager.FindAllVirtualPrinters(currentAppPackageFamilyName);

                foreach (var printer in printers)
                {
                    var ippDevice = IppPrintDevice.FromPrinterName(printer);
                    VirtualPrinters.Add(new VirtualPrinterInfo
                    {
                        PrinterName = printer,
                        PrinterUri = ippDevice.PrinterUri.ToString()
                    });
                }

                if (VirtualPrinters.Count == 0)
                {
                    await ShowMessageDialog("No Virtual Printers", "No virtual printers found for this app package.");
                }
            }
            catch (Exception ex)
            {
                await ShowMessageDialog("Error", $"Failed to load virtual printers: {ex.Message}");
            }
            finally
            {
                LoadingProgressRing.IsActive = false;
                LoadingProgressRing.Visibility = Visibility.Collapsed;
                VirtualPrintersListView.Visibility = Visibility.Visible;
            }
        }

        private async void OnAddVirtualPrinterClicked(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrWhiteSpace(printerNameTextBox.Text))
            {
                await ShowMessageDialog("Invalid Input", "Please enter a printer name.");
                return;
            }

            if (string.IsNullOrWhiteSpace(outputExtensionsTextBox.Text))
            {
                await ShowMessageDialog("Invalid Input", "Please enter at least one output file extension.");
                return;
            }

            IList<string> outputFileExtensions;
            try
            {
                outputFileExtensions = outputExtensionsTextBox.Text
                    .Split(',')
                    .Select(ext => ext.Trim())
                    .Where(ext => !string.IsNullOrWhiteSpace(ext))
                    .ToList();

                if (outputFileExtensions.Count == 0)
                {
                    await ShowMessageDialog("Invalid Input", "Please enter at least one valid output file extension.");
                    return;
                }

                foreach (var ext in outputFileExtensions)
                {
                    if (ext.Length > 10)
                    {
                        await ShowMessageDialog("Invalid Input", $"Output file extension '{ext}' is too long (max 10 characters).");
                        return;
                    }
                }
            }
            catch (Exception ex)
            {
                await ShowMessageDialog("Invalid Input", $"Failed to parse output file extensions: {ex.Message}");
                return;
            }

            var supportedFormats = new List<VirtualPrinterSupportedFormat>();
            if (addPassthroughFormatsCheckBox.IsChecked == true)
            {
                var formatVersionPairs = supportedFormatsTextBox.Text
                    .Split(';')
                    .Select(pair => pair.Trim())
                    .Where(pair => !string.IsNullOrWhiteSpace(pair))
                    .ToList();

                foreach (var pair in formatVersionPairs)
                {
                    var parts = pair.Split(',');
                    if (parts.Length == 2)
                    {
                        var format = parts[0].Trim();
                        var version = parts[1].Trim();
                        if (!string.IsNullOrWhiteSpace(format) && !string.IsNullOrWhiteSpace(version))
                        {
                            supportedFormats.Add(new VirtualPrinterSupportedFormat(format, version));
                        }
                    }
                }

                if (supportedFormats.Count == 0)
                {
                    await ShowMessageDialog("Invalid Input", "Please provide at least one valid format,version pair when enabling passthrough formats.");
                    return;
                }
            }

            Uri? printerUri = null;

            if (!string.IsNullOrWhiteSpace(printerUriTextBox.Text))
            {
                if (!Uri.IsWellFormedUriString(printerUriTextBox.Text, UriKind.Absolute))
                {
                    await ShowMessageDialog("Invalid Input", "Printer URI is not a valid URI.");
                    return;
                }
                printerUri = new Uri(printerUriTextBox.Text);
            }

            await AddVirtualPrinter(
                printerNameTextBox.Text,
                printerUri,
                formatComboBox.SelectedItem?.ToString() ?? "OpenXps",
                outputFileExtensions,
                supportedFormats);
        }

        private async System.Threading.Tasks.Task AddVirtualPrinter(string printerName, Uri? printerUri, string format, IList<string> outputFileExtensions, List<VirtualPrinterSupportedFormat> supportedFormats)
        {
            try
            {
                LoadingProgressRing.IsActive = true;
                LoadingProgressRing.Visibility = Visibility.Visible;

                var parameters = new VirtualPrinterInstallationParameters();
                parameters.PrinterName = printerName;
                parameters.PrinterUri = printerUri;
                parameters.PrintDeviceCapabilitiesPackageRelativeFilePath = "Config\\PrinterPdc.xml";
                parameters.PrintDeviceResourcesPackageRelativeFilePath = "Config\\PrinterPdr.xml";
                parameters.PreferredInputFormat = (format == "OpenXps") ? VirtualPrinterPreferredInputFormat.OpenXps : VirtualPrinterPreferredInputFormat.PostScript;
                parameters.SupportedInputFormats.Concat(supportedFormats);
                parameters.EntryPoint = "Tasks.VirtualPrinterBackgroundTask";

                var extensions = parameters.OutputFileExtensions;
                for (int i = 0; i < outputFileExtensions.Count; i++)
                {
                    extensions.Add(outputFileExtensions[i]);
                }

                await ShowMessageDialog("Debug", $"supportedFormats.Count: {supportedFormats.Count}");

                var installResult = await VirtualPrinterManager.InstallVirtualPrinterAsync(parameters);

                if (installResult.Status == VirtualPrinterInstallationStatus.InstallationSucceeded)
                {
                    await ShowMessageDialog("Success", $"Virtual printer '{printerName}' added successfully.");
                    LoadVirtualPrinters();
                }
                else
                {
                    await ShowMessageDialog("Error", $"Failed to add virtual printer: {installResult.Status}");
                }
            }
            catch (Exception ex)
            {
                await ShowMessageDialog("Error", $"Failed to add virtual printer: {ex.Message}");
            }
            finally
            {
                LoadingProgressRing.IsActive = false;
                LoadingProgressRing.Visibility = Visibility.Collapsed;
            }
        }

        private void OnRefreshClicked(object sender, RoutedEventArgs e)
        {
            LoadVirtualPrinters();
        }

        private async void VirtualPrintersListView_ItemClick(object sender, ItemClickEventArgs e)
        {
            var printer = e.ClickedItem as VirtualPrinterInfo;
            if (printer != null)
            {
                ContentDialog dialog = new ContentDialog
                {
                    Title = $"Remove Virtual Printer",
                    Content = $"Are you sure you want to remove the virtual printer '{printer.PrinterName}'?",
                    PrimaryButtonText = "Remove",
                    CloseButtonText = "Cancel",
                    XamlRoot = this.XamlRoot
                };

                var result = await dialog.ShowAsync();

                if (result == ContentDialogResult.Primary)
                {
                    await RemoveVirtualPrinter(printer.PrinterName);
                }
            }
        }

        private async System.Threading.Tasks.Task RemoveVirtualPrinter(string printerName)
        {
            try
            {
                LoadingProgressRing.IsActive = true;
                LoadingProgressRing.Visibility = Visibility.Visible;

                var removeSucccesfull = await VirtualPrinterManager.RemoveVirtualPrinterAsync(printerName);
                if (removeSucccesfull)
                {
                    await ShowMessageDialog("Success", $"Virtual printer '{printerName}' removed successfully.");
                    LoadVirtualPrinters();
                }
                else
                {
                    await ShowMessageDialog("Error", $"Failed to remove virtual printer '{printerName}'.");
                }
            }
            catch (Exception ex)
            {
                await ShowMessageDialog("Error", $"Failed to remove virtual printer: {ex.Message}");
            }
            finally
            {
                LoadingProgressRing.IsActive = false;
                LoadingProgressRing.Visibility = Visibility.Collapsed;
            }
        }
#endif

        private async System.Threading.Tasks.Task ShowMessageDialog(string title, string message)
        {
            if (this.XamlRoot == null)
            {
                // XamlRoot not available yet - skip dialog
                return;
            }

            ContentDialog dialog = new ContentDialog
            {
                Title = title,
                Content = message,
                CloseButtonText = "OK",
                XamlRoot = this.XamlRoot
            };

            await dialog.ShowAsync();
        }

        public class VirtualPrinterInfo
        {
            public string? PrinterName { get; set; }
            public string? PrinterUri { get; set; }
        }
    }
}
