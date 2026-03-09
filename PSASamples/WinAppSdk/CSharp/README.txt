README for C#/.Net Print Support Application Sample for WinAppSdk

Version 1:
  - Basic C#/.Net WinAppSdk PSA application that showcases setting up registration for PrintSupportExtension and PrintSupportWorkflow operations.
  - Minimal UX
  - Can be installed and used for printing (callbacks are hit).
  - Preferences can be launched however no information is displayed.

Version 2 (Current):
  - Upgraded to .NET 10 for improved performance and latest language features
  - Updated to CsWinRT 2.2.0+ for better .NET 8+ support
  - Complete migration from UWP C# sample with full UI functionality
  - All 10 UI pages migrated (UserLaunchMainPage, SettingsActivatedMainPage, JobActivatedMainPage,
    PrintTicketManipulationExample, WatermarkManipulationExample, JobNotificationExample,
    VirtualPrintersPage, GetIppPrinterUrl, AppInfo, PreviewPaginationControl, WatermarkPreviewControl)
  - Full watermarking support via XpsUtil C++ WinRT component
  - Background tasks: PrintSupportExtension, PrintSupportWorkflow, VirtualPrinter
  - Configuration files: PrinterPdc.xml, PrinterPdr.xml
  - Localization resources: en-US string resources for display names and media types

Requirements:
  - Must be using .NET 10
    - Upgraded from .NET 6 for better performance, security, and modern C# features
    - Previous requirement was .NET 6; .NET 8 investigation completed, now on .NET 10
  - Needs to use CsWinRT to support hosting the projections for callback from the OS for PSA operations.
    - Using CsWinRT version 2.2.0+ (upgraded from 2.0.8)
  - Should be using the latest WinAppSdk and build tools
  - CRITICAL: Must use MSBuild or Visual Studio to build (not dotnet CLI)
    - XpsUtil C++ WinRT component requires MSBuild
    - dotnet CLI cannot compile C++ projects
    - Watermarking features will not work without XpsUtil

Adjustments to CSProj files:
  - Tasks project:
    - Needs to have CsWinRT Nuget package.
    - Needs to have the following PropertyGroup:
        <PropertyGroup>
          <CsWinRTComponent>true</CsWinRTComponent>
          <CsWinRTWindowsMetadata>10.0.26100.0</CsWinRTWindowsMetadata>
        </PropertyGroup>
      - This is required to bring in WinRT.Host.dll and supporting files. WinRT.Host.dll hosts the projection. 
  - PrintSupportApplication project:
    - Needs to have a dependency/ProjectReference on the Tasks project.
    - Needs to have the following property set:
      <WindowsAppSDKSelfContained>true</WindowsAppSDKSelfContained>
    - Needs to have the following Nuget packages:
      - CsWinRT
      - SDK build tools
      - WindowsAppSDK
  - Appx Manifest:
    - Needs to have the following extension information set for the Application section:
      <Extensions>
        <psa:Extension Category="windows.printSupportExtension" EntryPoint="Tasks.PrintSupportExtensionBackgroundTask"/>
        <psa:Extension Category="windows.printSupportWorkflow" EntryPoint="Tasks.PrintSupportWorkflowBackgroundTask"/>
        <psa:Extension Category="windows.printSupportSettingsUI" EntryPoint="PrintSupportApplication.App"/>
        <psa:Extension Category="windows.printSupportJobUI" EntryPoint="PrintSupportApplication.App"/>
      </Extensions>
      - Note: Names can be adjusted to fit the app.
    - Needs to have the following extension information (outside of application section) set for task activation:
        <Extensions>
          <Extension Category="windows.activatableClass.inProcessServer">
            <InProcessServer>
              <Path>WinRT.Host.dll</Path>
              <ActivatableClass ActivatableClassId="Tasks.PrintSupportExtensionBackgroundTask" ThreadingModel="both" />
              <ActivatableClass ActivatableClassId="Tasks.PrintSupportWorkflowBackgroundTask" ThreadingModel="both" />
            </InProcessServer>
          </Extension>
        </Extensions>
        - Note: Names can be adjusted to fit the app.
    - Needs other standard adjustments to support a PSA (i.e. PrintSupport namespaces, etc).
  - RSP files:
    - Support CsWinRT and have paths specific to the machine. will likely need to be adjusted/recreated for the machine creating the application. 

Known Issues:
  - Unable to retrieve the instance arguments as PrintWorkflowJobActivatedEventArgs when handling an activation event for PrintSupportJobUI
    (This may be resolved with .NET 10 and CsWinRT 2.2+ - to be tested)

Solution Structure:
  - PrintSupportApplication (C# .NET 10) - Main application with UI
  - Tasks (C# .NET 10) - Background tasks (Extension, Workflow, VirtualPrinter)
  - XpsUtil (C++ WinRT) - Watermarking component (XpsPageWatermarker, XpsSequentialDocument, XpsPageWrapper)

  Build Order (automatically handled by Visual Studio/MSBuild):
    1. XpsUtil.vcxproj (generates XpsUtil.winmd and XpsUtil.dll)
    2. Tasks.csproj (consumes XpsUtil.winmd via CsWinRT, generates WinRT.Host.dll)
    3. PrintSupportApplication.csproj (consumes both Tasks and XpsUtil)

Build Instructions:
  IMPORTANT: This solution includes XpsUtil C++ WinRT component and MUST be built with MSBuild or Visual Studio

  Option 1 - Visual Studio 2022 (RECOMMENDED):
    1. Open PrintSupportApplicationSample_CSharp.sln in Visual Studio 2022
    2. Set Platform to x64 (or ARM64 for ARM builds)
    3. Build Solution (Ctrl+Shift+B)
    - This will automatically build:
      1. XpsUtil C++ WinRT component first
      2. Tasks C# project (with XpsUtil support)
      3. PrintSupportApplication C# project (with XpsUtil support)
    - Full watermarking functionality will be enabled

  Option 2 - MSBuild Command Line:
    > cd PSASamples\WinAppSdk\CS
    > msbuild PrintSupportApplicationSample_CSharp.sln /p:Platform=x64 /p:Configuration=Debug
    - Builds complete solution with all dependencies in correct order
    - Watermarking fully enabled

  Option 3 - dotnet CLI (NOT SUPPORTED):
    > dotnet build
    - ERROR: Will fail because dotnet CLI cannot build C++ projects
    - XpsUtil.vcxproj requires MSBuild
    - Do NOT use dotnet CLI for this solution

  Supported Platforms:
    - x64 (Recommended for development/testing)
    - x86 (for x86 systems)
    - ARM64 (for ARM devices)

  First Build:
    - NuGet packages for XpsUtil are included in XpsUtil\packages\
    - No additional setup required
    - Just open solution and build

Additional Resources:
  - Windows App SDK: https://learn.microsoft.com/en-us/windows/apps/windows-app-sdk/
  - CsWinRT documentation: https://github.com/microsoft/CsWinRT/tree/master/docs
