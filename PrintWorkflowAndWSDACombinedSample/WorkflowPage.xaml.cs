// Copyright(c) Microsoft Corporation.All rights reserved.
//
//   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//   PARTICULAR PURPOSE.
//
//   This is a sample implementation of both a Print Workflow, and a WSDA application. Both are invoked in OnActivated() when something is printed to
//   a printer that has been associated with a Workflow or WSDA application. The application looks at the OnActivated(IActivatedEventArgs args) argument
//   and if it is ActivationKind.PrintTaskSettings it loads a xaml page appropriate to that, which is hosted by the Modern Print Dialog "More Settings",
//   or if it is ActivationKind.PrintWorkflowForegroundTask then it is being activated as a Print Workflow application and a different page is loaded.
//   
//   The WSDA part of the app uses the activation args as PrintTaskSettingsActivatedEventArgs, then uses the  PrinterExtensionEventsSinks
//   Windows Runtime component to obtain a IPrinterExtensionContext which can be used in the usual way to get, for example, an IPrinterQueue or IPrintSchemaTicket.
//
//   The Workflow background task uses the XPS Object Model Windows Runtime component to create a IPrintWorkflowXpsReceiver implementation,
//   called on a different thread to the App. The generator receives the document object model parts, and we use the XPS Object Model to add watermark text
//   and a logo image before being sent on to the printer.
//
//   The Workflow sample demonstrates the Workflow UI callbacks which are, in the order called, OnSetupRequested() and OnXpsDataAvailable()
//   which are set in App.xaml.cs as the taskSessionManager.SetupRequested and taskSessionManager.XpsDataAvailable properties.
//   The UI foreground task cannot change the XPS data, and this must be done by the background task. Hence any parameters gathered
//   by the UI are saved in local storage for use by the background task, which actually makes the changes. The UI does not exit the
//   OnXpsDataAvailable() function until the required data has been collected
//
using PrinterContextNativeRuntimeComponent.Printing.PrinterExtension;
using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using Windows.ApplicationModel.Core;
using Windows.Foundation;
using Windows.Graphics.Imaging;
using Windows.Graphics.Printing.Workflow;
using Windows.Storage;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media.Imaging;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace WorkflowAndWSDACombinedSample
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class WorkflowPage : Page, INotifyPropertyChanged, IDisposable
    {
        public enum WorkflowPageLaunchType
        {
            Standalone,
            ForegroundTask
        }

        #region Public member functions
        public WorkflowPage()
        {
            InitializeComponent();

            // Allow for multiple "main" pages
            rootPage = currentPage = this;

            // Set data context to this page for picking up labels and properties
            DataContext = this;

            // Initialize workflow app heading with unknown printer
            WorkflowHeadingLabel = string.Format(workflowHeadingFormat, workflowHeadingLabel, unknownPrinterLabel);

            // Activate window at required size
            Windows.UI.ViewManagement.ApplicationView.GetForCurrentView().SetPreferredMinSize(new Windows.Foundation.Size(320, 150));
            Windows.UI.ViewManagement.ApplicationView.GetForCurrentView().TryResizeView(new Windows.Foundation.Size(300, 230));

            // Handle the HostedViewCLosing event to buy extra time before the background is cancelled
            var currentView = CoreApplication.GetCurrentView();
            if (!currentView.IsMain)
            {
                // Register for hosted view closing to revert lockdown  
                currentView.HostedViewClosing += OnHostedViewClosing;
            }

            // Create event which signals UI has all required information
            uiComplete = new ManualResetEvent(false);

            // Retrieve last standalone run settings, if any
            localStorage = new LocalStorageUtilities();
            if (localStorage.GetUseStandaloneSettings())
            {
                WatermarkTextBox.Text = localStorage.GetWatermarkTextFromLocalStorage();

                localStorage.GetImagePropertiesFromLocalStorage(out string imageFile, out double dpiX, out double dpiY, out int imageWidth, out int imageHeight);
                if (imageFile != null)
                {
                    SetLogoCheckBoxImage(imageFile);
                    ImageCheckBox.IsChecked = true;
                }

                // Set the "use standlaone settings" check box without triggering the callback
                UseStandaloneSettingsCheckBox.Checked -= UseStandaloneSettingsCheckBox_Checked;
                UseStandaloneSettingsCheckBox.IsChecked = true;
                UseStandaloneSettingsCheckBox.Checked += UseStandaloneSettingsCheckBox_Checked;
            }

            // Default to standalone, as the foreground task always will overwrite this value shortly after construction
            this.LaunchType = WorkflowPageLaunchType.Standalone;
        }

        public WorkflowPageLaunchType LaunchType
        {
            get
            {
                return launchType;
            }
            set
            {
                launchType = value;

                // For the "standalone" app make the check box visible, otherwise the "Set watermark" button
                thisApplication = Application.Current as App;
                SetWatermarkButton.Visibility = (value == WorkflowPageLaunchType.Standalone) ? Visibility.Collapsed : Visibility.Visible;
                UseStandaloneSettingsCheckBox.Visibility = (value == WorkflowPageLaunchType.Standalone) ? Visibility.Visible : Visibility.Collapsed;
            }
        }

        /// <summary>
        /// Standard IDisposable Dispose method to ensure ManualResetEvent uiComplete is correctly disposed of
        /// </summary>
        /// <param name="disposing"></param>
        public void Dispose()
        {
            // dispose managed resources
            uiComplete.Dispose();
        }
        #endregion

        #region Internal member functions
        /// <summary>
        /// Handle the Print Task Setup Event
        /// </summary>
        /// <param name="sessionManager">Session manager</param>
        /// <param name="printTaskSetupArgs">Has the Configuration and Controller</param>
        internal void OnSetupRequested(PrintWorkflowForegroundSession sessionManager, PrintWorkflowForegroundSetupRequestedEventArgs printTaskSetupArgs)
        {
            // If anything asynchronous is going to be done, you need to take out a deferral here,
            // since otherwise the next callback happens once this one exits, which may be premature
            Deferral setupRequestedDeferral = printTaskSetupArgs.GetDeferral();

            // Get information about the source application, print job title, and session ID
            string sourceApplicationName = printTaskSetupArgs.Configuration.SourceAppDisplayName;
            string jobTitle = printTaskSetupArgs.Configuration.JobTitle;
            string sessionId = printTaskSetupArgs.Configuration.SessionId;
            string localStorageVariablePrefix = string.Format("{0}::", sessionId);
            localStorage.SetStorageKeyPrefix(localStorageVariablePrefix);

            // Get the PrinterExtensionContextNative from the activation arguments
            IntPtr ptr = GetPrintWorkflowConfigurationNativeAsIntPtr(printTaskSetupArgs);

            // Create the Print Helper
            PrintHelperClass printHelper = new PrintHelperClass(InitializationType.PrinterExtensionContextNativeType, (ulong)ptr);
            printerName = printHelper.GetPrinterName();
            WorkflowHeadingLabel = string.Format(workflowHeadingFormat, sourceApplicationName, printerName);

            // Add callback handler on main page
            try
            {
                printHelper.OnInkLevelReceived += OnQueryResultReceived;
                // Send the BiDi query
                printHelper.SendInkLevelQuery();
            }
            catch (Exception ex)
            {
                string errorMessage = ex.Message;
                Debug.WriteLine(errorMessage);
            }
            finally
            {
                // Complete the deferral taken out at the start of OnSetupRequested
                setupRequestedDeferral.Complete();
            }
        }

        /// <summary>
        /// Called when the BiDi query gets a respons, e.g.for the Ink Level query
        /// </summary>
        /// <param name="sender">Sender</param>
        /// <param name="e">string</param>
        public void OnQueryResultReceived(object sender, string e)
        {
            string str = e;
        }

        /// <summary>
        /// Handle the Print Task XPS Data Available Event
        /// </summary>
        /// <param name="sessionManager">Session manager</param>
        /// <param name="printTaskXpsAvailableEventArgs">Has the Configuration and Controller</param>
        internal async void OnXpsDataAvailable(PrintWorkflowForegroundSession sessionManager, PrintWorkflowXpsDataAvailableEventArgs printTaskXpsAvailableEventArgs)
        {
            // Take out a deferral whilst getting the information
            Deferral xpsDataAvailableDeferral = printTaskXpsAvailableEventArgs.GetDeferral();

            if (sessionManager != null && printTaskXpsAvailableEventArgs != null)
            {
                // See if we already have the information required and it
                // were desireable to only have to enter this information once then
                // you could determine that here and not do WaitUntilTheUIHasAllRequiredData();
            }

            // Don't actually exit until all UI stuff is done
            await WaitUntilTheUIHasAllRequiredData();

            // Complete the deferral taken out at the start of OnSetupRequested
            xpsDataAvailableDeferral.Complete();
        }
        #endregion

        #region Private member functions
        /// <summary>
        /// If valid, add the user-provided Watermark and logo image details to local
        /// storage to be read by the background task
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">Event arguments</param>
        private async void SetWatermarkButton_Click(object sender, RoutedEventArgs e)
        {
            if (IsValidWatermark(Watermark))
            {
                // Save required values to local storage
                localStorage.SaveWatermarkTextToLocalStorage(Watermark);

                // If the check box for a logo image has been selected then add that too
                await AddLogoImageIfRequiredAsync();

                // Let the OnXpsDataAvailable finish by signalling that all the required parameters are now available
                SignalThatAllRequiredInformationIsAvailable();
            }
            else
            {
                // If bad number switch focus to the text box
                WatermarkTextBox.Focus(FocusState.Programmatic);
                // Highlight the text
                WatermarkTextBox.SelectAll();
            }

        }

        /// <summary>
        /// Make sure the watermark complies with some policy
        /// </summary>
        /// <param name="watermark">A valid Watermark text string</param>
        /// <returns></returns>
        private bool IsValidWatermark(string watermark)
        {
            bool watermarkIsValid = false;

            // Do some validation on the watermark text
            if (watermark != null && watermark.Length > 0)
            {
                watermarkIsValid = true;
            }

            return watermarkIsValid;
        }

        /// <summary>
        /// This takes the PrintWorkflowForegroundSetupRequestedEventArgs and gets the IPrinterExtensionContextNative from it,
        /// via the PrintWorkflowConfiguration property.
        /// </summary>
        /// <param name="args">IActivatedEventArgs passed to OnActivated()</param>
        /// <returns>Pointer to interface as IntPtr</returns>
        private static IntPtr GetPrintWorkflowConfigurationNativeAsIntPtr(PrintWorkflowForegroundSetupRequestedEventArgs args)
        {
            Object printerExtensionContextNative = args.Configuration;

            IntPtr ptr = IntPtr.Zero;
            Guid iidIPrinterExtensionContextNative = new Guid("C056BE0A-9EE2-450A-9823-964F0006F2BB");
            int hr = Marshal.QueryInterface(Marshal.GetIUnknownForObject(printerExtensionContextNative), ref iidIPrinterExtensionContextNative, out ptr);
            if (hr < 0)
            {
                throw new InvalidCastException(iidIPrinterExtensionContextNative.ToString());
            }

            if (ptr != IntPtr.Zero)
            {
                // This is good
            }
            else
            {
                throw new InvalidCastException(iidIPrinterExtensionContextNative.ToString());
            }

            return ptr;
        }

        /// <summary>
        /// Wait until all required input has been provided before exiting the foreground application
        /// </summary>
        /// <returns></returns>
        private async Task<bool> WaitUntilTheUIHasAllRequiredData()
        {
            await Task.Run(() =>
            {
                uiComplete.WaitOne();
            });

            // And return the bool indicating whether we've finished
            return true;
        }

        /// <summary>
        /// Signal that the UI has now provided all required information and can exit
        /// </summary>
        private void SignalThatAllRequiredInformationIsAvailable()
        {
            uiComplete.Set();
        }

        /// <summary>
        /// Allow user to add an image to be used a logo on top left of each page
        /// </summary>
        private async Task AddLogoImageIfRequiredAsync()
        {
            SoftwareBitmap softwareBitmap = null;

            if (ImageCheckBox.IsChecked == true)
            {
                var imageUri = new Uri("ms-appx:///" + ImageFilePath);
                StorageFile imageFile = await StorageFile.GetFileFromApplicationUriAsync(imageUri);
                softwareBitmap = await localStorage.GetSoftwareBitmapFromFileAsync(imageFile);
                if (softwareBitmap != null)
                {
                    // Save image properties to local storage
                    localStorage.SaveImagePropertiesToLocalStorage(ImageFilePath, softwareBitmap.DpiX, softwareBitmap.DpiY, softwareBitmap.PixelWidth, softwareBitmap.PixelHeight);
                }
            }
            else
            {
                localStorage.DeleteImagePropertiesFromLocalStorage();
            }
        }

        /// <summary>
        /// When launched in "Standalone" mode save the settings to local storage and save the check box state to indicate that that the app should not launch UI
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void UseStandaloneSettingsCheckBox_Checked(object sender, RoutedEventArgs e)
        {
            // Save unprefixed variables to local storage
            localStorage.SetStorageKeyPrefix("");
            // Use the smiley image instead of Contoso
            ImageFilePath = @"Assets\smiley.jpg";
            SetWatermarkButton_Click(sender, e);
            localStorage.SetUseStandaloneSettings(true);
            SetLogoCheckBoxImage(ImageFilePath);
        }

        /// <summary>
        /// When launched in "Standalone" mode save the check box state to indicate that the app should launch UI to get the information it needs
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void UseStandaloneSettingsCheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            localStorage.SetUseStandaloneSettings(false);
            SetLogoCheckBoxImage(@"Assets\contoso_logo.jpg");
        }

        /// <summary>
        /// Set the image displayed next to the logo image ceheck box
        /// </summary>
        /// <param name="imageFile"></param>
        private void SetLogoCheckBoxImage(string imageFile)
        {
            var imageUri = new Uri(@"ms-appx:///" + imageFile);
            BitmapImage newImage = new BitmapImage(imageUri);
            if (newImage != null)
            {
                LogoImage.Source = newImage;
            }
        }

        /// <summary>
        /// Gain extra time before background task is cancelled
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        private void OnHostedViewClosing(CoreApplicationView sender, HostedViewClosingEventArgs args)
        {
            // Get a deferral before we allow consumers to request deferral  
            var deferral = args.GetDeferral();

            // Do whatever you need to do before the view closes. In this case we wait to give the background task time to finish
            WFBackgroundTasks.WfBackgroundTask.WaitForTasks();

            // Complete the deferral before exiting the app  
            deferral.Complete();

            App.Current.Exit();
        }

        /// <summary>
        /// Standard INotifyPropertyChanged function
        /// </summary>
        private void OnPropertyChanged(string name)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
        #endregion

        #region Protected member functions
        /// <summary>
        /// Standard override of OnNavigatedTo, used to set data context and rootpage for finding main UI thread
        /// </summary>
        /// <param name="e"></param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            rootPage = WorkflowPage.currentPage;
            DataContext = this;
        }

        protected override void OnNavigatingFrom(NavigatingCancelEventArgs e)
        {
            // If this is standalone, and the checkbox to save settings is set... save them
            if (LaunchType == WorkflowPageLaunchType.Standalone)
            {
                LocalStorageUtilities localStorage = new LocalStorageUtilities();
                if (localStorage.GetUseStandaloneSettings())
                {
                    // If the "use standalone settings" check box has been checked then also save watermark text to local storage
                    localStorage.SaveWatermarkTextToLocalStorage(Watermark);
                }
            }
            base.OnNavigatingFrom(e);
        }
        #endregion

        #region Public properties
        public event PropertyChangedEventHandler PropertyChanged;
        // Accessors for strings and other resources required by XAML or within the code
        public string ActionButtonLabel { get => actionButtonLabel; }
        public string ImageCheckBoxLabel { get => imageCheckBoxLabel; }
        public string ImageFilePath { get => imageFilePath; set => imageFilePath = value; }
        public string UseStandaloneSettingsCheckBoxLabel { get => useStandaloneSettingsCheckBoxLabel; }
        public string Watermark { get => watermark; set => watermark = value; }
        public string WatermarkEntryPlaceholder { get => watermarkEntryPlaceholder; }
        public string WatermarkLabel { get => watermarkLabel; }
        public string WorkflowHeadingLabel
        {
            get {return workflowHeadingLabel;}
            set
            {
                workflowHeadingLabel = value;

                // Must UI thread to update XAML controls
#pragma warning disable 4014
                rootPage.Dispatcher.RunAsync(CoreDispatcherPriority.Low, () =>
                {
                    OnPropertyChanged("WorkflowHeadingLabel");
                });
#pragma warning restore 4014
            }
        }
        #endregion

        #region Private member variables
        private static WorkflowPage currentPage;
        // This is the image file which needs to be accessible to all threads/parts of the application
        private string imageFilePath = @"Assets\contoso_logo.jpg";
        private LocalStorageUtilities localStorage;
        private string printerName = "";
        private WorkflowPage rootPage;
        App thisApplication;
        private ManualResetEvent uiComplete;
        private string watermark = "";
        private WorkflowPageLaunchType launchType;

        // START: These would be localized strings in a production app
        private string actionButtonLabel = "Set the Watermark text";
        private string imageCheckBoxLabel = "Add a logo at the top left";
        private string unknownPrinterLabel = "Unknown Printer";
        private string useStandaloneSettingsCheckBoxLabel = "Use these settings and do not display UI";
        private string watermarkLabel = "Watermark text";
        private string watermarkEntryPlaceholder = "Enter Watermark text";
        private string workflowHeadingFormat = "{0} - {1}";
        private string workflowHeadingLabel = "Workflow Demo";
        // END: These would be localized strings in a production app
        #endregion
    }

    /// <summary>
    /// A standard XAML binding converter class used to allow controls to be enabled when
    /// the text entered is none-zero length
    /// </summary>
    public class LengthToBoolConverter : Windows.UI.Xaml.Data.IValueConverter
    {
        // This converts the int Text.Length of a TextBox to a bool which is true if it is greater than zero
        public object Convert(object value, Type targetType,
            object parameter, string language)
        {
            // Get the text and see if the string contains something
            int textLength = (int)value;
            bool isNotEmpty = (textLength > 0);

            return isNotEmpty;
        }

        // No need to implement converting back on a one-way binding 
        public object ConvertBack(object value, Type targetType,
            object parameter, string language)
        {
            throw new NotImplementedException();
        }
    }

}
