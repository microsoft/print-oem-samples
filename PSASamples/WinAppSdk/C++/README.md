 # C++ Print Support App (Windows App SDK)

- [C++ Print Support App (Windows App SDK)](#c-print-support-app-windows-app-sdk)
  - [Overview](#overview)
  - [Project Structure](#project-structure)
  - [Release History](#release-history)
  - [Remarks](#remarks)
  - [Resources](#resources)

## Overview

This sample demonstrates a Print Support App (PSA) implementation using C++/WinRT and the Windows App SDK (WinUI 3). The solution includes:

- **PSAAppSdk**: The main WinUI 3 application that provides printer settings UI, job activation handling, watermark preview, and workflow management.
- **BackgroundTasks**: Windows Runtime component containing background tasks for print workflow processing, print support extension, and virtual printer operations.
- **XpsUtil**: Utility library for XPS document manipulation, including watermarking and page processing.

The app showcases modern print support capabilities including custom printer settings, XPS document manipulation, watermark application, and IPP attribute handling using the Windows App SDK framework.

## Project Structure

```
C++/
├── PSAAppSdk/               # Main WinUI 3 application
│   ├── App.xaml.*                      # Application entry point
│   ├── MainWindow.xaml.*               # Main application window
│   ├── UserLaunchMainPage.xaml.*       # User-launched settings UI
│   ├── JobActivationMainPage.xaml.*    # Job-specific activation UI
│   ├── PrinterSettings.xaml.*          # Printer configuration UI
│   ├── WorkflowWatermarkSample.xaml.*  # Watermark workflow UI
│   ├── WatermarkPreviewControl.xaml.*  # Watermark preview control
│   ├── PreviewPaginationControl.xaml.* # Page navigation control
│   ├── AppInfo.xaml.*                  # Application information page
│   ├── ModalWindowHandler.*            # Modal window management
│   ├── Package.appxmanifest            # App package manifest
│   └── Assets/                         # Application assets
│
├── BackgroundTasks/                    # Windows Runtime component
│   ├── PrintSupportExtension.*         # PSE background task
│   ├── WorkflowBgTask.*                # Workflow background task
│   ├── VirtualPrinterBgTask.*          # Virtual printer task
│   ├── LocalStorageHelper.*            # Local storage utilities
│   └── BackgroundTasks.def             # Module definition
│
└── XpsUtil/                            # XPS utilities
    ├── XpsPageWatermarker.*            # Watermark application
    ├── XpsPageWrapper.*                # XPS page manipulation
    ├── XpsSequentialDocument.*         # Document processing
    └── SynchronizedSequentialStream.*  # Stream handling
```

## Release History

<details>
<summary>Current Version (as of November 2025)</summary>

Initial Windows App SDK (WinUI 3) C++ implementation including:

- Migration from UWP to Windows App SDK framework
- WinUI 3 XAML UI implementation with modern controls
- C++/WinRT implementation of Print Support App functionality
- Background tasks for print workflow, PSE, and virtual printer operations
- XPS document manipulation and watermarking capabilities
- Preview rendering with pagination controls
- Custom IPP attribute support
- Multi-instance app support
- Modal window handling for print dialog scenarios
- Local storage helper for app data persistence

</details>

## Remarks

<details>

- This sample requires the Windows App SDK and is designed for Windows 10, version 2004 (build 19041) and later.
- The solution uses C++/WinRT projections for Windows Runtime APIs.
- IDL files define Windows Runtime classes, and the XAML compiler generates implementation headers.
- Background tasks run out-of-process to support multi-instance scenarios.
- The app demonstrates modern print support capabilities including:
  - Custom printer settings UI
  - Print job workflow interception and modification
  - XPS document watermarking
  - IPP attribute manipulation
  - Preview rendering with real-time updates
- Build the project at least once to generate implementation templates under the "Generated Files" folder.
- The PSAAppSdk project includes NuGet packages for Windows App SDK and Windows Implementation Library (WIL).

**Building the Solution:**
1. Open `PSAAppSdk.sln` (`print-oem-samples\PSASamples\WinAppSdk\C++\PSAAppSdk\PSAAppSdk.sln`) in Visual Studio 2022 or later
2. Restore NuGet packages
  - In the Solution Explorer, right click on the solution header at the top
  - Select "Restore NuGet packages"
3. Clean, then build the solution (this generates C++/WinRT headers from IDL files)
  - In the Solution Explorer, right click on the solution header at the top
  - Select "Clean Solution"
  - Wait for the Output panel to show the cleanup was completed successfully
  - Right click the solution header again and select "Build Solution"
  - Wait for the Output panel to show the build is completed
4. Also make sure that PSAAppSdk is selected as the startup project
  - In the Solution Explorer, right click on the PSAAppSdk project header
  - Select "Set as Starup Project"
5. Deploy and run the application
  - Make sure the settings are configured as "Debug" and "x64" at the top of the Visual Studio menu
  - Click the green triangle button to run the application on your local machine
6. PSA Popup Appears
  - A popup titled "WinUI" should appear, displaying the package family name of the printer
7. Close the popup window and stop the application by clicking the red square in visual studio

**Manifest Requirements:**
- The `Package.appxmanifest` must declare the following extensions:
  - `windows.printSupportExtension` - For printer capabilities management
  - `windows.printSupportWorkflow` - For print job workflow handling
  - `windows.printSupportSettingsUI` - For printer settings UI
  - `windows.printSupportJobUI` - For job-specific UI
- Required capabilities:
  - `privateNetworkClientServer` - For IPP communication
  - Multi-instance support via `uap10:SupportsMultipleInstances="true"`

</details>

## Resources

- [Windows App SDK Documentation](https://docs.microsoft.com/windows/apps/windows-app-sdk/)
- [WinUI 3 Documentation](https://docs.microsoft.com/windows/apps/winui/winui3/)
- [C++/WinRT](https://docs.microsoft.com/windows/uwp/cpp-and-winrt-apis/)
- [Print Support App Overview](https://docs.microsoft.com/windows/uwp/devices-sensors/print-support-app)
- [Windows.Graphics.Printing.PrintTicket](https://docs.microsoft.com/uwp/api/windows.graphics.printing.printticket)
- [Windows Implementation Library (WIL)](https://github.com/microsoft/wil)
- [Using an Extension INF File](https://docs.microsoft.com/windows-hardware/drivers/install/using-an-extension-inf-file)
- [IPP Attributes](https://www.iana.org/assignments/ipp-registrations/ipp-registrations.xhtml)
