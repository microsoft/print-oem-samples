using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using Windows.ApplicationModel;
using Windows.Devices.Printers;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

namespace PrintSupportApp
{
    public sealed partial class VirtualPrintersPage : Page
    {
        public ObservableCollection<VirtualPrinterInfo> VirtualPrinters { get; set; }

        public VirtualPrintersPage()
        {
            InitializeComponent();
            VirtualPrinters = new ObservableCollection<VirtualPrinterInfo>();
            VirtualPrintersListView.ItemsSource = VirtualPrinters;
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            LoadVirtualPrinters();
        }

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
            var dialog = new ContentDialog
            {
                Title = "Add Virtual Printer",
                PrimaryButtonText = "Add",
                CloseButtonText = "Cancel",
                DefaultButton = ContentDialogButton.Primary
            };

            var panel = new StackPanel { Spacing = 10 };

            var printerNameTextBox = new TextBox
            {
                Header = "Printer Name",
                PlaceholderText = "Enter printer name"
            };

            var printerUriTextBox = new TextBox
            {
                Header = "Printer URI",
                PlaceholderText = "e.g., contoso:print-to-pdf or contoso:print-to-file"
            };

            var formatComboBox = new ComboBox
            {
                Header = "Preferred Input Format",
                PlaceholderText = "Select format",
                ItemsSource = new[] { "OpenXps", "Postscript" },
                SelectedIndex = 0
            };

            var enableFilePrinterCheckBox = new CheckBox
            {
                Content = "Enable File Printer",
                IsChecked = false
            };

            var outputExtensionsTextBox = new TextBox
            {
                Header = "Output File Extensions (comma-separated)",
                PlaceholderText = "e.g., pdf, xps",
                Text = "pdf",
                Visibility = Visibility.Collapsed
            };

            var addPassthroughFormatsCheckBox = new CheckBox
            {
                Content = "Add passthrough supported formats",
                IsChecked = false
            };

            var supportedFormatsTextBox = new TextBox
            {
                Header = "Supported Formats (format,version; format,version)",
                PlaceholderText = "e.g., application/pdf,1.7;application/postscript,3.0",
                Text = "application/pdf,1.7",
                Visibility = Visibility.Collapsed
            };

            enableFilePrinterCheckBox.Checked += (s, args) =>
            {
                outputExtensionsTextBox.Visibility = Visibility.Visible;
            };

            enableFilePrinterCheckBox.Unchecked += (s, args) =>
            {
                outputExtensionsTextBox.Visibility = Visibility.Collapsed;
            };

            addPassthroughFormatsCheckBox.Checked += (s, args) =>
            {
                supportedFormatsTextBox.Visibility = Visibility.Visible;
            };

            addPassthroughFormatsCheckBox.Unchecked += (s, args) =>
            {
                supportedFormatsTextBox.Visibility = Visibility.Collapsed;
            };

            panel.Children.Add(printerNameTextBox);
            panel.Children.Add(printerUriTextBox);
            panel.Children.Add(formatComboBox);
            panel.Children.Add(enableFilePrinterCheckBox);
            panel.Children.Add(outputExtensionsTextBox);
            panel.Children.Add(addPassthroughFormatsCheckBox);
            panel.Children.Add(supportedFormatsTextBox);

            dialog.Content = panel;

            var result = await dialog.ShowAsync();

            if (result == ContentDialogResult.Primary)
            {
                if (string.IsNullOrWhiteSpace(printerNameTextBox.Text))
                {
                    await ShowMessageDialog("Invalid Input", "Printer name is required.");
                    return;
                }

                var outputFileExtensions = new List<string>();
                if (enableFilePrinterCheckBox.IsChecked == true)
                {
                    outputFileExtensions = outputExtensionsTextBox.Text
                        .Split(',')
                        .Select(ext => ext.Trim())
                        .Where(ext => !string.IsNullOrWhiteSpace(ext))
                        .ToList();
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

                Uri printerUri = null;

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
                    formatComboBox.SelectedItem.ToString(),
                    outputFileExtensions,
                    supportedFormats);
            }
        }

        private async System.Threading.Tasks.Task AddVirtualPrinter(string printerName, Uri printerUri, string format, IList<string> outputFileExtensions, List<VirtualPrinterSupportedFormat> supportedFormats)
        {
            try
            {
                LoadingProgressRing.IsActive = true;
                LoadingProgressRing.Visibility = Visibility.Visible;

                var parameters = new VirtualPrinterInstallationParameters();
                parameters.PrinterName = printerName;
                parameters.PrinterUri = printerUri;
                parameters.PrintDeviceCapabilitiesPackageRelativeFilePath = "Config\\PrinterPdc.xml";
                //parameters.PrintDeviceResourcesPackageRelativeFilePath = "Config\\PrinterPdr.xml";
                parameters.PreferredInputFormat = (format == "OpenXps") ? VirtualPrinterPreferredInputFormat.OpenXps : VirtualPrinterPreferredInputFormat.PostScript;
                parameters.SupportedInputFormats.Concat(supportedFormats);
                parameters.EntryPoint = "Tasks.VirtualPrinterBackgroundTask";

                var extensions = parameters.OutputFileExtensions;
                for (int i = 0; i < outputFileExtensions.Count; i++)
                {
                    extensions.Add(outputFileExtensions[i]);
                }

                var formatVersionPairs = parameters.SupportedInputFormats;
                for (int i = 0; i < supportedFormats.Count; i++)
                {
                    formatVersionPairs.Add(supportedFormats[i]);
                }

                var installResult = await VirtualPrinterManager.InstallVirtualPrinterAsync(parameters);

                if (installResult.Status == VirtualPrinterInstallationStatus.InstallationSucceeded)
                {
                    await ShowMessageDialog("Success", $"Virtual printer '{printerName}' added successfully.");
                    LoadVirtualPrinters();
                }
                else
                {
                    await ShowMessageDialog("Error", $"Failed to add virtual printer. Error: {installResult.ExtendedError.ToString()}");
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

        private void OnPrinterItemClicked(object sender, ItemClickEventArgs e)
        {
            var printer = e.ClickedItem as VirtualPrinterInfo;
            if (printer != null)
            {
                // You can implement additional actions here when a printer is clicked
            }
        }

        private async void OnRemovePrinterClicked(object sender, RoutedEventArgs e)
        {
            var button = sender as Button;
            if (button?.Tag is string printerName)
            {
                var confirmDialog = new ContentDialog
                {
                    Title = "Confirm Removal",
                    Content = $"Are you sure you want to remove '{printerName}'?",
                    PrimaryButtonText = "Remove",
                    CloseButtonText = "Cancel",
                    DefaultButton = ContentDialogButton.Close
                };

                var result = await confirmDialog.ShowAsync();

                if (result == ContentDialogResult.Primary)
                {
                    await RemoveVirtualPrinter(printerName);
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
                    await ShowMessageDialog("Error", $"Failed to remove virtual printer:");
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

        private async System.Threading.Tasks.Task ShowMessageDialog(string title, string content)
        {
            var dialog = new ContentDialog
            {
                Title = title,
                Content = content,
                CloseButtonText = "OK"
            };

            await dialog.ShowAsync();
        }

        public class VirtualPrinterInfo
        {
            public string PrinterName { get; set; }
            public string PrinterUri { get; set; }
        }
    }
}