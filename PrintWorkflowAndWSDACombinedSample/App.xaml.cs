// Copyright(c) Microsoft Corporation.All rights reserved.
//
//   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//   PARTICULAR PURPOSE.
//
using System;
using System.Runtime.InteropServices;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Activation;
using Windows.Graphics.Printing.Workflow;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using PrinterContextNativeRuntimeComponent.Printing.PrinterExtension;
using Windows.UI.ViewManagement;
using Windows.Foundation;

namespace WorkflowAndWSDACombinedSample
{
    /// <summary>
    /// Provides application-specific behavior to supplement the default Application class.
    /// </summary>
    sealed partial class App : Application
    {
        #region Public properties and enums
        public Object Context;
        #endregion

        #region Public member functions
        /// <summary>
        /// Initializes the singleton application object.  This is the first line of authored code
        /// executed, and as such is the logical equivalent of main() or WinMain().
        /// </summary>
        public App()
        {
            this.InitializeComponent();
            this.Suspending += OnSuspending;
        }
        #endregion

        #region Protected member functions
        /// <summary>
        /// Invoked when the application is launched normally by the end user.  Other entry points
        /// will be used such as when the application is launched to open a specific file.
        /// </summary>
        /// <param name="e">Details about the launch request and process.</param>
        protected override void OnLaunched(LaunchActivatedEventArgs e)
        {

#if DO_NOT_SHOW_THIS_DEBUG
            if (System.Diagnostics.Debugger.IsAttached)
            {
                this.DebugSettings.EnableFrameRateCounter = true;
            }
#endif

            Frame rootFrame = Window.Current.Content as Frame;

            // Do not repeat app initialization when the Window already has content,
            // just ensure that the window is active
            if (rootFrame == null)
            {
                // Create a Frame to act as the navigation context and navigate to the first page
                rootFrame = new Frame();

                rootFrame.NavigationFailed += OnNavigationFailed;

                if (e.PreviousExecutionState == ApplicationExecutionState.Terminated)
                {
                    // Load state from previously suspended application
                    // Not really applicable to the "More settings" dialog
                }

                // Place the frame in the current Window
                Window.Current.Content = rootFrame;
            }

            if (rootFrame.Content == null)
            {
                // When the navigation stack isn't restored navigate to the first page,
                // configuring the new page by passing required information as a navigation
                // parameter
                rootFrame.Navigate(typeof(WorkflowPage), e.Arguments);
            }

            // Ensure the current window is active
            Window.Current.Activate();
        }

        /// <summary>
        /// Set the preferred size for the application view window
        /// </summary>
        public static void ActivateWindowAndSetDesiredSize()
        {
            const float desiredAppWidth = 320;
            const float desiredAppHeight = 230;
            float DPI = Windows.Graphics.Display.DisplayInformation.GetForCurrentView().LogicalDpi;
            ApplicationView.GetForCurrentView().SetPreferredMinSize(new Size(desiredAppWidth, desiredAppHeight));
            ApplicationView.PreferredLaunchWindowingMode = ApplicationViewWindowingMode.PreferredLaunchViewSize;
            var desiredSize = new Size((desiredAppWidth * 96.0f / DPI), (desiredAppHeight * 96.0f / DPI));
            ApplicationView.PreferredLaunchViewSize = desiredSize;
            bool result = ApplicationView.GetForCurrentView().TryResizeView(desiredSize);
            Window.Current.Activate();
        }

        /// <summary>
        /// Invoked when the app is launched via an activation contract. In this case we care about ActivationKind.PrintTaskSettings 
        /// </summary>
        /// <param name="args">Information about why this was invoked</param>
        protected override void OnActivated(IActivatedEventArgs args)
        {
            if (args.Kind == ActivationKind.PrintTaskSettings)
            {
                Frame rootFrame = new Frame();
                if (null == Window.Current.Content)
                {
                    rootFrame.Navigate(typeof(MainPage));
                    Window.Current.Content = rootFrame;
                }
                Window.Current.Activate();

                MainPage mainPage = (MainPage)rootFrame.Content;

                // Get the PrinterExtensionContext from the activation arguments
                IntPtr ptr = GetPrinterExtensionContextAsIntPtr(args);
                // Create the Print Helper
                PrintHelperClass printHelper = new PrintHelperClass(InitializationType.PrinterExtensionContextType, (ulong)ptr);
                // Can use the Windows Runtime Component above to GetPrinterName() and do BiDi queries etc.
            }

            if (args.Kind == ActivationKind.PrintWorkflowForegroundTask)
            {
                Frame rootFrame = new Frame();
                if (null == Window.Current.Content)
                {
                    rootFrame.Navigate(typeof(WorkflowPage));
                    Window.Current.Content = rootFrame;
                }

                // Ensure the current window is active
                ActivateWindowAndSetDesiredSize();

                // Get the main page
                WorkflowPage workflowPage = (WorkflowPage)rootFrame.Content;

                // Workflow stuff here

                // Make sure the page knows it's handling a foreground task activation
                workflowPage.LaunchType = WorkflowPage.WorkflowPageLaunchType.ForegroundTask;

                // Get the activation arguments
                PrintWorkflowUIActivatedEventArgs printTaskUIEventArgs = args as PrintWorkflowUIActivatedEventArgs;

                // Get the session manager
                PrintWorkflowForegroundSession taskSessionManager = printTaskUIEventArgs.PrintWorkflowSession;

                // Add the callback handlers - these are on the main page
                taskSessionManager.SetupRequested += workflowPage.OnSetupRequested;
                taskSessionManager.XpsDataAvailable += workflowPage.OnXpsDataAvailable;

                taskSessionManager.Start();
            }
        }

        /// <summary>
        /// This takes the IActivatedEventArgs and gets the IPrinterExtensionContext from it, via the PrintTaskConfiguration
        /// </summary>
        /// <param name="args">IActivatedEventArgs passed to OnActivated()</param>
        /// <returns></returns>
        private static IntPtr GetPrinterExtensionContextAsIntPtr(IActivatedEventArgs args)
        {
            PrintTaskSettingsActivatedEventArgs printTaskSettingsActivatedEventArgs = args as PrintTaskSettingsActivatedEventArgs;
            Windows.Devices.Printers.Extensions.PrintTaskConfiguration configuration = printTaskSettingsActivatedEventArgs.Configuration;
            Object printerExtensionContext = configuration.PrinterExtensionContext;

            IntPtr ptr = IntPtr.Zero;
            Guid iidIPrinterExtensionContext = new Guid("39843BF2-C4D2-41FD-B4B2-AEDBEE5E1900");
            int hr = Marshal.QueryInterface(Marshal.GetIUnknownForObject(printerExtensionContext), ref iidIPrinterExtensionContext, out ptr);
            if (hr < 0)
            {
                throw new InvalidCastException(iidIPrinterExtensionContext.ToString());
            }

            if (ptr != IntPtr.Zero)
            {
                // This is good
            }
            else
            {
                throw new InvalidCastException(iidIPrinterExtensionContext.ToString());
            }

            return ptr;
        }
        #endregion

        #region Private member functions
        /// <summary>
        /// Invoked when Navigation to a certain page fails
        /// </summary>
        /// <param name="sender">The Frame which failed navigation</param>
        /// <param name="e">Details about the navigation failure</param>
        private void OnNavigationFailed(object sender, NavigationFailedEventArgs e)
        {
            throw new Exception("Failed to load Page " + e.SourcePageType.FullName);
        }

        /// <summary>
        /// Invoked when application execution is being suspended.  Application state is saved
        /// without knowing whether the application will be terminated or resumed with the contents
        /// of memory still intact.
        /// 
        /// This particular implementation also helps make sure that all background tasks complete
        /// before the application is suspended.  This is important in cases where the background
        /// task runs in the same process as the UI views, as the tasks will be cancelled
        /// if the application is suspended.  In this implementation, the suspending handler
        /// simply calls into a static method in the background task class that will wait
        /// on an event until all the background tasks complete.
        /// 
        /// When the background tasks are run in a separate process (not recommended), this call
        /// will still call into the background task assembly, but the event that signals
        /// that the background tasks are all completed will already be signaled, so this handler
        /// will exit quickly.
        /// 
        /// Note that if the OnSuspending handler takes more than 30 minutes, the application
        /// will be forcibly closed.
        /// 
        /// One other "gotcha" is that to debug this code in Visual Studio, you must disable PLM debugging
        /// for the current processes.  To do that, once you have attached to the workflow process (typically
        /// by setting a breakpoint in WfBackgroundTask::Run), run plmdebug.exe /disableDebug and pass in
        /// the full package name for this application.  plmdebug can be found as part of the Windows 10 SDK.
        /// </summary>
        /// <param name="sender">The source of the suspend request.</param>
        /// <param name="e">Details about the suspend request.</param>
        private void OnSuspending(object sender, SuspendingEventArgs e)
        {
            var deferral = e.SuspendingOperation.GetDeferral();

            WFBackgroundTasks.WfBackgroundTask.WaitForTasks();

            deferral.Complete();
        }
        #endregion
    }
}
