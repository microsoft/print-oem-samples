# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a Windows Print Support Application (PSA) sample built with C#/.NET 10 and Windows App SDK. It demonstrates print support extension capabilities including printer preferences, print workflow manipulation, virtual printers, and XPS watermarking.

## Build Requirements

**CRITICAL**: This solution MUST be built with MSBuild or Visual Studio 2022. The dotnet CLI will fail because XpsUtil is a C++ WinRT component that requires MSBuild.

### Prerequisites
- Visual Studio 2022
- .NET 10 SDK (10.0.102+)
- Windows SDK 10.0.26100.0+
- Windows 11 Build 26100+ for full PSA features

### Build Commands

**Option 1 - Visual Studio (Recommended):**
```powershell
# Open PrintSupportApplicationSample_CSharp.sln in Visual Studio 2022
# Set Platform to x64 (or ARM64/x86)
# Build Solution (Ctrl+Shift+B)
```

**Option 2 - MSBuild:**
```powershell
msbuild PrintSupportApplicationSample_CSharp.sln /p:Platform=x64 /p:Configuration=Debug
```

**Supported Platforms:** x64, x86, ARM64

### Install/Register Package
```powershell
Add-AppxPackage -Register PrintSupportApplication\bin\x64\Debug\net10.0-windows10.0.26100.0\win-x64\AppX\Package.appxmanifest
```

## Solution Architecture

### Project Dependencies (Build Order)
1. **XpsUtil** (C++ WinRT) - Generates XpsUtil.winmd and XpsUtil.dll
   - Provides XPS manipulation: XpsPageWatermarker, XpsSequentialDocument, XpsPageWrapper
   - Required for watermarking features

2. **XpsUtil.Projections** (C# .NET 10) - CsWinRT projections for XpsUtil
   - Consumes XpsUtil.winmd

3. **Tasks** (C# .NET 10) - Background tasks with CsWinRT hosting
   - Generates WinRT.Host.dll (required for background task activation)
   - Contains 3 background tasks:
     - PrintSupportExtensionBackgroundTask - Handles PDC/PDR changes
     - PrintSupportWorkflowBackgroundTask - Print job processing
     - VirtualPrinterBackgroundTask - Virtual printer operations

4. **PrintSupportApplication** (C# .NET 10) - Main WinUI 3 application
   - 13 XAML pages for UI across 3 activation modes
   - Consumes both Tasks and XpsUtil

### Activation Modes & UI Pages

The App.xaml.cs handles 3 activation types:

**1. Normal Launch** (`OnLaunched` → `HandleNormalLaunch`)
- Page: `UserLaunchMainPage.xaml`
- Navigation items:
  - AppInfo.xaml - Shows package family name
  - GetIppPrinterUrl.xaml - IPP printer discovery
  - VirtualPrintersPage.xaml - Virtual printer management

**2. Settings Activation** (Right-click printer → Printing Preferences)
- Page: `SettingsActivatedMainPage.xaml`
- Navigation item:
  - PrintTicketManipulationExample.xaml - Print ticket feature editing

**3. Job UI Activation** (During print job if UI is launched)
- Page: `JobActivatedMainPage.xaml`
- Navigation items:
  - JobNotificationExample.xaml
  - WatermarkManipulationExample.xaml
  - PreviewPaginationControl.xaml
  - WatermarkPreviewControl.xaml

### Background Task Flow

**Print Workflow:**
1. User initiates print → OS triggers PrintSupportWorkflowBackgroundTask
2. `OnJobStarting` event fires → calls `SetSkipSystemRendering()`
3. `OnPdlModificationRequested` event fires → manipulates XPS/PDF stream
4. Can optionally launch Job UI via `PrintWorkflowUILauncher.LaunchAndCompleteUIAsync`

**Virtual Printer:**
1. User prints to virtual printer → OS triggers VirtualPrinterBackgroundTask
2. `Session_VirtualPrinterDataAvailable` event → receives XPS/PostScript stream
3. Converts to PDF/PS using XpsUtil watermarking if configured
4. Saves to user-selected location

**Extension:**
1. Printer capabilities change → OS triggers PrintSupportExtensionBackgroundTask
2. `PrintDeviceCapabilitiesChanged` event → manipulates PDC/PDR
3. Can add custom media types, features, etc.

### CsWinRT Configuration

The Tasks project requires special configuration for WinRT hosting:

```xml
<PropertyGroup>
  <CsWinRTComponent>true</CsWinRTComponent>
  <CsWinRTWindowsMetadata>10.0.26100.0</CsWinRTWindowsMetadata>
</PropertyGroup>
```

This generates WinRT.Host.dll which is referenced in Package.appxmanifest:

```xml
<Extension Category="windows.activatableClass.inProcessServer">
  <InProcessServer>
    <Path>WinRT.Host.dll</Path>
    <ActivatableClass ActivatableClassId="Tasks.PrintSupportExtensionBackgroundTask" ThreadingModel="both" />
    <ActivatableClass ActivatableClassId="Tasks.PrintSupportWorkflowBackgroundTask" ThreadingModel="both" />
    <ActivatableClass ActivatableClassId="Tasks.VirtualPrinterBackgroundTask" ThreadingModel="both" />
  </InProcessServer>
</Extension>
```

### Configuration Files

Located in `PrintSupportApplication/Config/`:
- **PrinterPdc.xml** - Print Device Capabilities (custom features, media types, paper sizes)
- **PrinterPdr.xml** - Print Device Resources (localized display names for IPP features)

### Localization Resources

Located in `PrintSupportApplication/Strings/en-US/`:
- **Resources.resw** - Display names for virtual printers (PdfPrintDisplayName, PsPrintDisplayName)
- **ContosoMediaTypes.resw** - Custom media type names
- **IppMediaTypes.resw** - IPP standard media type names

Referenced in code as `ms-resource:ResourceKey` and resolved at runtime.

### Storage Pattern

LocalStorageUtil.cs provides persistent storage for print settings:
- Saves watermark configurations, print ticket preferences
- Uses Windows.Storage.ApplicationData.Current.LocalSettings
- Key-value pairs with serialization support

## Key Implementation Patterns

### Conditional XpsUtil Compilation
Code uses `#if XPSUTIL_AVAILABLE` preprocessor directives to handle builds without XpsUtil:
```csharp
#if XPSUTIL_AVAILABLE
using XpsUtil;
#endif
```

### Print Ticket Manipulation
PrintTicketManipulationExample.xaml.cs demonstrates:
- Enumerating print capabilities via `PrintWorkflowConfiguration.PrinterJob.GetJobPrintTicket()`
- Modifying features (duplex, color, orientation, media size)
- Handling feature inter-dependencies (e.g., collate disabled when copies = 1)
- Persisting changes via `UpdatePrintTicketAsync()`

### Virtual Printer Management
VirtualPrintersPage.xaml.cs shows:
- Creating virtual printers via `PrintSupportVirtualPrinterManager.CreateVirtualPrinterAsync()`
- Querying existing printers via `FindAllAsync()`
- Removing printers via `RemoveVirtualPrinterAsync()`
- Configured in Package.appxmanifest with DisplayName, PrinterUri, PreferredInputFormat

### IPP Printer Discovery
GetIppPrinterUrl.xaml.cs demonstrates:
- Query all IPP printers: `DeviceInformation.FindAllAsync(PrintDevice.GetDeviceSelector())`
- Filter by manufacturer using AQS query strings
- Extract printer URLs and pairing status

## Known Issues

1. **PrintWorkflowJobActivatedEventArgs retrieval may fail** - Casting activation arguments to PrintWorkflowJobActivatedEventArgs might not work correctly. This may be resolved with .NET 10 and CsWinRT 2.2+ but needs testing.

2. **RSP files are machine-specific** - The CsWinRT .rsp files contain paths specific to the build machine and may need regeneration.

## Testing

See TEST_PLAN.md for comprehensive test scenarios covering:
- All activation modes
- Print workflow with/without watermarking
- Virtual printer creation and usage
- IPP printer discovery
- Settings persistence

## Important Notes

- PrintSupportApplication.csproj requires: `<WindowsAppSDKSelfContained>true</WindowsAppSDKSelfContained>`
- CsWinRT version 2.2.0+ required for .NET 10 support
- XpsUtil NuGet packages are included in XpsUtil/packages/ directory
- First build automatically restores packages and generates necessary projections
