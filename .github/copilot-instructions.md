# Copilot Instructions for print-oem-samples

## Repository Overview

This repository contains **Print Support App (PSA) samples** and a legacy **Print Workflow + WSDA migration sample** for Windows OEM printer partners. All projects are Visual Studio solutions targeting Windows; there are no CI/CD pipelines, test suites, or linters configured.

## Build

Open the relevant `.sln` in Visual Studio (2017+ for the legacy sample, 2022+ for WinAppSdk samples) and build with `Ctrl+Shift+B`. Each sample area has its own solution:

| Sample | Solution path |
|---|---|
| Legacy Workflow + WSDA | `WorkflowAndWSDACombinedSample.sln` |
| PSA — UWP C# | `PSASamples/UWP/CSharp/PrintSupportApp/PrintSupportApp.sln` |
| PSA — WinAppSdk C++ | `PSASamples/WinAppSdk/C++/PSAAppSdk/PSAAppSdk.sln` |
| PSA — WinAppSdk C# | `PSASamples/WinAppSdk/CSharp/PrintSupportApplication/PrintSupportApplication.sln` |
| PSA — WinAppSdk C# NativeAOT | `PSASamples/WinAppSdk/CSharp_NativeAOT/PrintSupportApplication/PrintSupportApplication.sln` |
| PSA — WinAppSdk C# CustomHost | `PSASamples/WinAppSdk/CSharp_CustomHost/PrintSupportApplication/PrintSupportApplication.sln` |

NuGet packages restore from nuget.org (see `NuGet.Config`). Key dependencies: `Microsoft.WindowsAppSDK`, `Microsoft.Windows.CsWinRT`, `Microsoft.Windows.SDK.BuildTools`, and `Microsoft.Windows.CppWinRT` (C++ projects).

## Architecture

### Legacy combined sample (root-level solution)

The root `WorkflowAndWSDACombinedSample.sln` demonstrates migrating WSDAs (Windows Store Device Apps) to UWP Print Workflow:

- **PrintWorkflowAndWSDACombinedSample** — UWP app handling both `PrintTaskSettings` (WSDA) and `PrintWorkflowForegroundTask` activation in `App.xaml.cs`.
- **WFBackgroundTasks** — UWP background task component implementing `IBackgroundTask`. Performs XPS manipulation and uses a static ref-count + event pattern to prevent suspension while background work completes.
- **PrinterContextNativeRuntimeComponent** — C++ WinRT component wrapping Printer Extension COM APIs. Accepts either the classic WSDA context or the Print Workflow `IPrinterExtensionContextNative`.
- **XpsOMRuntimeComponent** — C++ WinRT component implementing `IPrintWorkflowXpsReceiver` for off-thread XPS page mutation (watermarking). Callbacks are HRESULT-based.
- **UtilitiesLibrary / LocalStorageUtilities** — Small shared helpers for toast notifications, local settings, and bitmap loading.

### PSA samples (`PSASamples/`)

Each PSA sample follows a two-project pattern: an **app** project (UI, activation routing) and a **Tasks** project (background tasks as a WinRT component). The app routes activation by `ActivationKind` to scenario-specific pages.

- **UWP/CSharp** — Classic UWP app + Tasks + XpsUtil (C++ native WinRT component for XPS rendering).
- **WinAppSdk/C++** — WinUI 3 + C++/WinRT. `PSAAppSdk` (app) + `BackgroundTasks` (WinRT component) + shared `XpsUtil`.
- **WinAppSdk/CSharp** — WinUI 3 + C#. Uses CsWinRT. The app post-build copies `Tasks.winmd` into the MSIX package.
- **WinAppSdk/CSharp_NativeAOT** — Same as C# but Tasks compiles with `PublishAot`. Removes `WinRT.Host.*` files and replaces them with native output.
- **WinAppSdk/CSharp_CustomHost** — Hybrid model with a managed bridge (`CustomWinRTHost.Managed`) exposing `[UnmanagedCallersOnly]` activation factory, plus a native host DLL. Avoids stock `WinRT.Host.dll` bootstrapping.

**XpsUtil** (`PSASamples/XpsUtil/`) is a shared C++ WinRT component for XPS watermark/page manipulation, used by multiple samples. It has a C# projection wrapper (`XpsUtil.Projections`).

## Conventions

### Activation pattern
Apps branch on activation kind (e.g., `PrintSupportSettingsUI`, `PrintSupportJobUI`, `PrintWorkflowForegroundTask`) in `App.xaml.cs` / `App.xaml.cpp` to select the appropriate XAML page. UWP uses `OnActivated`; WinAppSdk uses `AppInstance.GetActivatedEventArgs()`.

### Async patterns
- **C#**: `async Task<T>`, `await`, background task deferrals (`GetDeferral()` / `Complete()`).
- **C++/WinRT**: `winrt::fire_and_forget`, `co_await winrt::resume_background()`, `DispatcherQueue().TryEnqueue()` for UI marshaling.

### COM interop
- **C#**: `Marshal.QueryInterface` / `IntPtr` for printer extension context.
- **C++**: `ComPtr`, `reinterpret_cast`, `RoGetActivationFactory`, HRESULT-first error handling.

### Naming
- PascalCase for namespaces, classes, and public members.
- `m_` prefix for private fields in WinUI 3 C# code.
- Event handlers named descriptively: `OnPdlModificationRequested`, `OnSetupRequested`, etc.

### Background task lifecycle
Background tasks use `IBackgroundTask` with deferral patterns. The legacy sample uses a static reference-count plus `ManualResetEvent` to keep the process alive up to 30 minutes while background work finishes after UI dismissal.

### Project packaging
WinAppSdk C# samples have post-build steps that copy WinRT component outputs (`.winmd`, `.dll`) into the MSIX package. The NativeAOT variant additionally strips `WinRT.Host.*` files. These post-build steps are defined in the `.csproj` files and are critical to correct deployment.
