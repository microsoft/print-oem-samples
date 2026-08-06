# C# Print Support App — Native Host variant (Windows App SDK)

- [C# Print Support App — Native Host variant (Windows App SDK)](#c-print-support-app--native-host-variant-windows-app-sdk)
  - [Overview](#overview)
  - [Project Structure](#project-structure)
  - [Release History](#release-history)
  - [Remarks](#remarks)
  - [Resources](#resources)

## Overview

This sample demonstrates a Print Support App (PSA) implementation using C#/.NET 10 and the Windows App SDK (WinUI 3). The **Native Host** variant uses a native CoreCLR hosting shim (`NativeHost.dll`) and ships a **self-contained** .NET 10 runtime inside the MSIX. This gives partners a reviewable, customizable runtime bootstrap that runs inside `BackgroundTaskHost.exe` and removes any dependency on a system-installed .NET runtime.

The solution includes:

- **PrintSupportApplication**: The main WinUI 3 application that provides printer settings UI, job activation handling, watermark preview, virtual printer management, and workflow management. Owns the MSIX packaging that stages the self-contained runtime and `NativeHost.dll` into the package layout.
- **Tasks**: C# .NET 10 CsWinRT component containing background tasks for print support extension, print workflow processing, and virtual printer operations. Also contains `NativeHostBridge.cs`, the managed half of the native ⇄ managed activation contract that resolves activation factories from the CsWinRT-generated registry.
- **NativeHost**: A native (C) DLL registered as the WinRT `InProcessServer` for the background tasks. It exports `DllGetActivationFactory`, bootstraps the bundled CoreCLR via `hostfxr`, resolves the managed `NativeHostBridge` method, and forwards activation requests to it. See `Tasks\NativeHost\README.md` for a deep dive.
- **XpsUtil**: C++ WinRT native component for XPS document manipulation (watermarking, page processing), consumed through **XpsUtil.Projections**, a C# projections assembly. `XpsUtil.dll` is activated via registration-free WinRT (`XpsUtil.dll.manifest`). Shared with the C++ sample.

The app showcases modern print support capabilities including custom printer settings, XPS document manipulation, watermark application, print ticket manipulation, IPP attribute handling, virtual printers, and job notifications using the Windows App SDK framework.

## Project Structure

```
CSharp_NativeHost/
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
│   ├── Package.appxmanifest                    # App package manifest (InProcessServer → NativeHost.dll)
│   ├── XpsUtil.dll.manifest                    # Reg-free WinRT activation for native XpsUtil.dll
│   ├── PrintSupportApplication.csproj          # App project (self-contained; stages runtime + NativeHost.dll)
│   ├── PrintSupportApplication.sln             # Solution file
│   ├── Config/                                 # Printer configuration
│   │   ├── PrinterPdc.xml                      # Printer device capabilities
│   │   └── PrinterPdr.xml                      # Printer device resources
│   ├── Strings/                                # Localization resources
│   │   └── en-US/                              # English (US) strings
│   └── Assets/                                 # Application assets
│
├── Tasks/                                      # Background tasks (C# .NET 10 CsWinRT component)
│   ├── PrintSupportExtensionBackgroundTask.cs  # PSE background task
│   ├── PrintSupportWorkflowBackgroundTask.cs   # Workflow background task
│   ├── VirtualPrinterBackgroundTask.cs         # Virtual printer task
│   ├── LocalStorageUtil.cs                     # Local storage utilities
│   ├── NativeHostBridge.cs                     # Managed half of the native ⇄ managed activation contract
│   ├── Tasks.csproj                            # Self-contained; emits Tasks.runtimeconfig.json for hostfxr
│   └── NativeHost/                             # Native CoreCLR hosting shim (C)
│       ├── host.c                              # DllGetActivationFactory + CoreCLR bootstrap via hostfxr
│       ├── host.h                              # Internal declarations
│       ├── host.def                            # Exports DllGetActivationFactory (undecorated)
│       ├── NativeHost.vcxproj                  # Builds NativeHost.dll (x64-only)
│       └── README.md                           # Deep dive on the hosting shim
│
└── XpsUtil (external, ..\..\..\XpsUtil)        # XPS utilities (C++ WinRT native component)
    ├── XpsUtil.vcxproj                         # Native component (XpsUtil.dll)
    ├── XpsUtil.Projections/                    # C# projections assembly consumed by Tasks/PSA
    ├── XpsPageWatermarker.*                    # Watermark application
    ├── XpsPageWrapper.*                        # XPS page manipulation
    ├── XpsSequentialDocument.*                 # Document processing
    └── SynchronizedSequentialStream.*          # Stream handling
```

## Release History

<details>
<summary>Version 2 (Current)</summary>

- **Native hosting shim**: Replaced the CsWinRT-generated `WinRT.Host.dll` with a hand-rolled native CoreCLR host (`NativeHost.dll`) registered as the WinRT `InProcessServer` for the background tasks
- **Self-contained deployment**: The full .NET 10 runtime is bundled inside the MSIX (`hostfxr.dll`, `hostpolicy.dll`, `coreclr.dll`, framework assemblies) — no system .NET install required on partner machines
- **XpsUtil.Projections**: Watermarking consumed via a C# projections assembly over the native `XpsUtil.dll`, activated through registration-free WinRT (`XpsUtil.dll.manifest`)
- Upgraded to .NET 10 for improved performance and latest language features
- Uses CsWinRT 2.2.0+ and Windows App SDK 1.6 (self-contained)
- All UI pages migrated (UserLaunchMainPage, SettingsActivatedMainPage, JobActivatedMainPage, PrintTicketManipulationExample, WatermarkManipulationExample, JobNotificationExample, VirtualPrintersPage, GetIppPrinterUrl, AppInfo, PreviewPaginationControl)
- Background tasks: PrintSupportExtension, PrintSupportWorkflow, VirtualPrinter (three registered virtual printers: PDF, PostScript, Cloud)
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

- This sample is self-contained: it bundles the .NET 10 runtime and the Windows App SDK inside the MSIX, so a system-installed .NET 10 runtime is **not** required on the target machine. It is designed for Windows 11 24H2 (build 26010) and later.
- Unlike the baseline `CSharp` sample, the background tasks are hosted by a hand-rolled native CoreCLR shim (`NativeHost.dll`) rather than the CsWinRT-generated `WinRT.Host.dll`. `NativeHost.dll` is the registered `InProcessServer`; it bootstraps CoreCLR via `hostfxr` and forwards WinRT activation to the managed `NativeHostBridge` in `Tasks.dll`. See `Tasks\NativeHost\README.md` for the full contract.
- Uses CsWinRT 2.2.0+ and Windows App SDK 1.6 to project WinRT types for OS callbacks during PSA operations.
- **CRITICAL: Must use MSBuild or Visual Studio to build (not dotnet CLI).** Both the `NativeHost` (C) and `XpsUtil` (C++ WinRT) native components require MSBuild; dotnet CLI cannot compile them. Activation and watermarking features will not work without them.

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
1. XpsUtil.vcxproj (native C++ WinRT component; generates XpsUtil.winmd and XpsUtil.dll)
2. XpsUtil.Projections.csproj (C# projections over XpsUtil.dll)
3. Tasks.csproj (CsWinRT component; self-contained publish, emits Tasks.runtimeconfig.json for hostfxr)
4. NativeHost.vcxproj (native C shim; generates NativeHost.dll)
5. PrintSupportApplication.csproj (stages the self-contained runtime + NativeHost.dll into the MSIX)

**Supported Platforms:**
- x64 (Recommended — the `NativeHost` shim is x64-only)
- x86 (for x86 systems)
- ARM64 (for ARM devices)

**CSProj Configuration:**

- **Tasks project:**
  - Requires the CsWinRT NuGet package
  - Must be marked as a CsWinRT component and publish self-contained so the .NET runtime is bundled:
    ```xml
    <PropertyGroup>
      <CsWinRTComponent>true</CsWinRTComponent>
      <CsWinRTWindowsMetadata>10.0.26100.0</CsWinRTWindowsMetadata>
      <!-- Exclude the unmanaged bridge from the CsWinRT source generator -->
      <CsWinRTExcludeFromSourceGenerator>Tasks.NativeHost.NativeHostBridge</CsWinRTExcludeFromSourceGenerator>
      <SelfContained>true</SelfContained>
      <EnableDynamicLoading>true</EnableDynamicLoading>
    </PropertyGroup>
    ```
  - `NativeHostBridge` exposes an `[UnmanagedCallersOnly]` `GetActivationFactory` method that resolves factories from the CsWinRT-generated `WinRT.Module` registry. It is **not** an activatable WinRT class, so it is excluded from the source generator.
  - `Tasks.csproj` also emits `Tasks.runtimeconfig.json` (a class library does not normally get one); `hostfxr_initialize_for_dotnet_command_line` requires it next to `Tasks.dll` or CoreCLR never starts.

- **PrintSupportApplication project:**
  - Requires a ProjectReference on the Tasks project and on `XpsUtil.Projections`
  - Requires a build-only ProjectReference on `NativeHost.vcxproj` (`ReferenceOutputAssembly=false`) so the shim is built and staged into the MSIX
  - Must set: `<WindowsAppSDKSelfContained>true</WindowsAppSDKSelfContained>` and `<SelfContained>true</SelfContained>`
  - Owns the packaging targets that copy the self-contained runtime, `NativeHost.dll`, `Tasks.deps.json`, `Tasks.runtimeconfig.json`, and `Tasks.winmd` into the MSIX root
  - Required NuGet packages: CsWinRT, SDK build tools, WindowsAppSDK

**Manifest Requirements:**
- The `Package.appxmanifest` must declare the following extensions in the Application section:
  ```xml
  <Extensions>
    <printsupport:Extension Category="windows.printSupportExtension" EntryPoint="Tasks.PrintSupportExtensionBackgroundTask"/>
    <printsupport:Extension Category="windows.printSupportWorkflow" EntryPoint="Tasks.PrintSupportWorkflowBackgroundTask"/>
    <printsupport:Extension Category="windows.printSupportSettingsUI" EntryPoint="PrintSupportApplication.App"/>
    <printsupport:Extension Category="windows.printSupportJobUI" EntryPoint="PrintSupportApplication.App"/>
  </Extensions>
  ```
- The following extension information (outside of the Application section) is required for task activation. Note the `<Path>` points at **`NativeHost.dll`**, not `WinRT.Host.dll`:
  ```xml
  <Extensions>
    <Extension Category="windows.activatableClass.inProcessServer">
      <InProcessServer>
        <Path>NativeHost.dll</Path>
        <ActivatableClass ActivatableClassId="Tasks.PrintSupportWorkflowBackgroundTask" ThreadingModel="both" />
        <ActivatableClass ActivatableClassId="Tasks.PrintSupportExtensionBackgroundTask" ThreadingModel="both" />
        <ActivatableClass ActivatableClassId="Tasks.VirtualPrinterBackgroundTask" ThreadingModel="both" />
      </InProcessServer>
    </Extension>
    <Extension Category="windows.activatableClass.inProcessServer">
      <InProcessServer>
        <Path>XpsUtil.dll</Path>
        <ActivatableClass ActivatableClassId="XpsUtil.XpsPageWatermarker" ThreadingModel="both" />
        <ActivatableClass ActivatableClassId="XpsUtil.XpsSequentialDocument" ThreadingModel="both" />
      </InProcessServer>
    </Extension>
  </Extensions>
  ```
- Required capabilities:
  - `runFullTrust` — required for the PSA / self-contained packaging model
  - `privateNetworkClientServer` — For IPP communication
  - PrintSupport namespaces and other standard PSA adjustments

**Known Issues:**
- Virtual Printer APIs (`Windows.Devices.Printers.VirtualPrinter*`) exist in Windows SDK 10.0.26100.0 but are not yet projected in the `Microsoft.Windows.SDK.NET.Ref` package used here, so that path is compiled out via the `VIRTUAL_PRINTER_DISABLED` constant. It will be enabled once the SDK projections ship.
- Unable to retrieve the instance arguments as `PrintWorkflowJobActivatedEventArgs` when handling an activation event for PrintSupportJobUI (this may be resolved with .NET 10 and CsWinRT 2.2+ — to be tested)
- The self-contained runtime must be staged completely into the MSIX. If `Tasks.deps.json`, `Tasks.runtimeconfig.json`, or the bundled `hostfxr.dll`/`coreclr.dll` are missing, `BackgroundTaskHost.exe` terminates with `ExitReason=UnexpectedAbort` and printer install fails — always fix the MSIX bundling.

</details>

## Resources

- [Windows App SDK Documentation](https://docs.microsoft.com/windows/apps/windows-app-sdk/)
- [WinUI 3 Documentation](https://docs.microsoft.com/windows/apps/winui/winui3/)
- [CsWinRT Documentation](https://github.com/microsoft/CsWinRT/tree/master/docs)
- [Print Support App Overview](https://docs.microsoft.com/windows/uwp/devices-sensors/print-support-app)
- [Windows.Graphics.Printing.PrintTicket](https://docs.microsoft.com/uwp/api/windows.graphics.printing.printticket)
- [IPP Attributes](https://www.iana.org/assignments/ipp-registrations/ipp-registrations.xhtml)
