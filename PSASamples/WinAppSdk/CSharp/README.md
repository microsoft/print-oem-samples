# C# Print Support App (Windows App SDK)

- [C# Print Support App (Windows App SDK)](#c-print-support-app-windows-app-sdk)
  - [Overview](#overview)
  - [Project Structure](#project-structure)
  - [Release History](#release-history)
  - [Remarks](#remarks)
  - [Resources](#resources)

## Overview

This sample demonstrates a Print Support App (PSA) implementation using C#/.NET 10 and the Windows App SDK (WinUI 3). The solution includes:

- **PrintSupportApplication**: The main WinUI 3 application that provides printer settings UI, job activation handling, watermark preview, virtual printer management, and workflow management.
- **Tasks**: C# .NET 10 project containing background tasks for print support extension, print workflow processing, and virtual printer operations. Uses CsWinRT 2.2.0+ to host WinRT projections for OS callbacks.
- **XpsUtil**: C++ WinRT utility component for XPS document manipulation, including watermarking and page processing (shared with the C++ sample).

The app showcases modern print support capabilities including custom printer settings, XPS document manipulation, watermark application, print ticket manipulation, IPP attribute handling, and job notifications using the Windows App SDK framework.

## Project Structure

```
CSharp/
├── PrintSupportApplication/                    # Main WinUI 3 application (C# .NET 10)
│   ├── App.xaml(.cs)                           # Application entry point
│   ├── MainWindow.xaml(.cs)                    # Main application window
│   ├── UserLaunchMainPage.xaml(.cs)            # User-launched settings UI
│   ├── SettingsActivatedMainPage.xaml(.cs)     # Settings activation UI
│   ├── JobActivatedMainPage.xaml(.cs)          # Job-specific activation UI
│   ├── PrintTicketManipulationExample.xaml(.cs)# Print ticket manipulation UI
│   ├── WatermarkManipulationExample.xaml(.cs)  # Watermark workflow UI
│   ├── JobNotificationExample.xaml(.cs)        # Job notification UI
│   ├── VirtualPrintersPage.xaml(.cs)           # Virtual printer management UI
│   ├── GetIppPrinterUrl.xaml(.cs)              # IPP printer URL retrieval
│   ├── AppInfo.xaml(.cs)                       # Application information page
│   ├── PreviewPaginationControl.xaml(.cs)      # Page navigation control
│   ├── WatermarkPreviewControl.xaml(.cs)       # Watermark preview control
│   ├── WatermarkOptions.cs                     # Watermark configuration
│   ├── Package.appxmanifest                    # App package manifest
│   ├── PrintSupportApplication.sln             # Solution file
│   ├── Config/                                 # Printer configuration
│   │   ├── PrinterPdc.xml                      # Printer device capabilities
│   │   └── PrinterPdr.xml                      # Printer device resources
│   ├── Strings/                                # Localization resources
│   │   └── en-US/                              # English (US) strings
│   └── Assets/                                 # Application assets
│
├── Tasks/                                      # Background tasks (C# .NET 10)
│   ├── PrintSupportExtensionBackgroundTask.cs  # PSE background task
│   ├── PrintSupportWorkflowBackgroundTask.cs   # Workflow background task
│   ├── VirtualPrinterBackgroundTask.cs         # Virtual printer task
│   └── LocalStorageUtil.cs                     # Local storage utilities
│
└── XpsUtil (external)                          # XPS utilities (C++ WinRT)
    ├── XpsPageWatermarker.*                    # Watermark application
    ├── XpsPageWrapper.*                        # XPS page manipulation
    ├── XpsSequentialDocument.*                 # Document processing
    └── SynchronizedSequentialStream.*          # Stream handling
```

## Release History

<details>
<summary>Version 2 (Current)</summary>

- Upgraded to .NET 10 for improved performance and latest language features
- Updated to CsWinRT 2.2.0+ for better .NET 8+ support
- Complete migration from UWP C# sample with full UI functionality
- All 10 UI pages migrated (UserLaunchMainPage, SettingsActivatedMainPage, JobActivatedMainPage, PrintTicketManipulationExample, WatermarkManipulationExample, JobNotificationExample, VirtualPrintersPage, GetIppPrinterUrl, AppInfo, PreviewPaginationControl)
- Full watermarking support via XpsUtil C++ WinRT component
- Background tasks: PrintSupportExtension, PrintSupportWorkflow, VirtualPrinter
- Configuration files: PrinterPdc.xml, PrinterPdr.xml
- Localization resources: en-US string resources for display names and media types

</details>

<details>
<summary>Version 1</summary>

- Basic C#/.NET WinAppSdk PSA application that showcases setting up registration for PrintSupportExtension and PrintSupportWorkflow operations
- Minimal UX
- Can be installed and used for printing (callbacks are hit)
- Preferences can be launched however no information is displayed

</details>

## Remarks

<details>

- This sample requires .NET 10 and the Windows App SDK installed on the machine, and is designed for Windows 11 24H2 (build 26010) and later.
- Uses CsWinRT 2.2.0+ to host WinRT projections for OS callbacks during PSA operations.
- **CRITICAL: Must use MSBuild or Visual Studio to build (not dotnet CLI).** The XpsUtil C++ WinRT component requires MSBuild; dotnet CLI cannot compile C++ projects. Watermarking features will not work without XpsUtil.

**Building the Solution:**
1. Open `PrintSupportApplication.sln` (`PSASamples\WinAppSdk\CSharp\PrintSupportApplication\PrintSupportApplication.sln`) in Visual Studio 2026
2. Restore NuGet packages
    - In the Solution Explorer, right click on the solution header at the top
    - Select "Restore NuGet packages"
3. Clean, then build the solution
    - In the Solution Explorer, right click on the solution header at the top
    - Select "Clean Solution"
    - Wait for the Output panel to show the cleanup was completed successfully
    - Right click the solution header again and select "Build Solution"
    - Wait for the Output panel to show the build is completed
4. Also make sure that PrintSupportApplication is selected as the startup project
    - In the Solution Explorer, right click on the PrintSupportApplication project header
    - Select "Set as Startup Project"
5. Deploy and run the application
    - Make sure the settings are configured as "Debug" and "x64" at the top of the Visual Studio menu
    - Click the green triangle button to run the application on your local machine
6. If you encounter build issues, install the **Single-project MSIX Packaging Tools for VS 2022-26** extension
    - Go to **Extensions > Manage Extensions** in Visual Studio
    - Search for "Single-project MSIX Packaging Tools" and install it
    - Restart Visual Studio and rebuild the solution

**Build Options:**

| Method | Supported | Notes |
|--------|-----------|-------|
| Visual Studio 2026 | Yes (Recommended) | Opens `.sln`, builds all projects in correct order |
| MSBuild Command Line | Yes | `msbuild PrintSupportApplication.sln /p:Platform=x64 /p:Configuration=Debug` |
| dotnet CLI | **No** | Cannot build C++ projects (XpsUtil); do NOT use |

**Build Order** (automatically handled by Visual Studio/MSBuild):
1. XpsUtil.vcxproj (generates XpsUtil.winmd and XpsUtil.dll)
2. Tasks.csproj (consumes XpsUtil.winmd via CsWinRT, generates WinRT.Host.dll)
3. PrintSupportApplication.csproj (consumes both Tasks and XpsUtil)

**Supported Platforms:**
- x64 (Recommended for development/testing)
- x86 (for x86 systems)
- ARM64 (for ARM devices)

**CSProj Configuration:**

- **Tasks project:**
  - Requires CsWinRT NuGet package
  - Must include the following PropertyGroup:
    ```xml
    <PropertyGroup>
      <CsWinRTComponent>true</CsWinRTComponent>
      <CsWinRTWindowsMetadata>10.0.26100.0</CsWinRTWindowsMetadata>
    </PropertyGroup>
    ```
  - This is required to bring in `WinRT.Host.dll` and supporting files for hosting the projection.

- **PrintSupportApplication project:**
  - Requires a ProjectReference on the Tasks project
  - Must set: `<WindowsAppSDKSelfContained>true</WindowsAppSDKSelfContained>`
  - Required NuGet packages: CsWinRT, SDK build tools, WindowsAppSDK

**Manifest Requirements:**
- The `Package.appxmanifest` must declare the following extensions in the Application section:
  ```xml
  <Extensions>
    <psa:Extension Category="windows.printSupportExtension" EntryPoint="Tasks.PrintSupportExtensionBackgroundTask"/>
    <psa:Extension Category="windows.printSupportWorkflow" EntryPoint="Tasks.PrintSupportWorkflowBackgroundTask"/>
    <psa:Extension Category="windows.printSupportSettingsUI" EntryPoint="PrintSupportApplication.App"/>
    <psa:Extension Category="windows.printSupportJobUI" EntryPoint="PrintSupportApplication.App"/>
  </Extensions>
  ```
- The following extension information (outside of the Application section) is required for task activation:
  ```xml
  <Extensions>
    <Extension Category="windows.activatableClass.inProcessServer">
      <InProcessServer>
        <Path>WinRT.Host.dll</Path>
        <ActivatableClass ActivatableClassId="Tasks.PrintSupportExtensionBackgroundTask" ThreadingModel="both" />
        <ActivatableClass ActivatableClassId="Tasks.PrintSupportWorkflowBackgroundTask" ThreadingModel="both" />
      </InProcessServer>
    </Extension>
  </Extensions>
  ```
- Required capabilities:
  - `privateNetworkClientServer` — For IPP communication
  - PrintSupport namespaces and other standard PSA adjustments

**Known Issues:**
- Unable to retrieve the instance arguments as `PrintWorkflowJobActivatedEventArgs` when handling an activation event for PrintSupportJobUI (this may be resolved with .NET 10 and CsWinRT 2.2+ — to be tested)
- RSP files support CsWinRT and have paths specific to the machine; they will likely need to be adjusted or recreated for the machine building the application

</details>

## Resources

- [Windows App SDK Documentation](https://docs.microsoft.com/windows/apps/windows-app-sdk/)
- [WinUI 3 Documentation](https://docs.microsoft.com/windows/apps/winui/winui3/)
- [CsWinRT Documentation](https://github.com/microsoft/CsWinRT/tree/master/docs)
- [Print Support App Overview](https://docs.microsoft.com/windows/uwp/devices-sensors/print-support-app)
- [Windows.Graphics.Printing.PrintTicket](https://docs.microsoft.com/uwp/api/windows.graphics.printing.printticket)
- [IPP Attributes](https://www.iana.org/assignments/ipp-registrations/ipp-registrations.xhtml)
