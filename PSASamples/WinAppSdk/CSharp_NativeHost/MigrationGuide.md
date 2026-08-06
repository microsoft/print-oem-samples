# CSharp_NativeHost — Partner companion guide

> Companion to `README.md` and `Tasks\\NativeHost\\README.md`. This document is the partner-facing walk-through for adapting the normal `CSharp` PSA sample to a self-contained, Store-deployable package with an in-tree native host.

## Contents

- [1. Audience and positioning](#1-audience-and-positioning)
- [2. Source tree layout and dependencies](#2-source-tree-layout-and-dependencies)
- [3. Packaging a complete, self-contained runtime](#3-packaging-a-complete-self-contained-runtime)
- [4. Variants for partner adaptation](#4-variants-for-partner-adaptation)
- [5. Gotchas we hit and fixed so you do not have to](#5-gotchas-we-hit-and-fixed-so-you-do-not-have-to)
- [6. Build and verify](#6-build-and-verify)
- [7. Migration from the existing CSharp sample](#7-migration-from-the-existing-csharp-sample)
- [8. Limitations and caveats](#8-limitations-and-caveats)
- [9. Where to ask for help](#9-where-to-ask-for-help)

---

## 1. Audience and positioning

This guide is for print-stack partners building C# Print Support Applications (PSAs) for Windows. The `CSharp_NativeHost` sample starts from the same managed PSA shape as the normal `CSharp` sample, then adds the native hosting and MSIX payload work required for a self-contained Microsoft Store deployment.

Use this pattern when:

- You need the PSA to run on a clean customer machine with no system-wide .NET runtime installed.
- You want the runtime bootstrap to be visible and auditable in your source tree.
- You want a small native host that you can customize for diagnostics or product policy before managed activation begins.

The sample does not change the PSA workflow model. Your background task classes, print workflow event handlers, foreground app, resources, and manifest class IDs remain ordinary C# PSA code.

---

## 2. Source tree layout and dependencies

This sample is one of several Print Support Application samples shipped together. It builds against a shared `XpsUtil` helper project that lives outside the PSA samples folder. Partners who extract only `CSharp_NativeHost\` in isolation will not be able to build it.

### Expected on-disk layout

```
PrintSamples\
├── XpsUtil\                       <- shared helper (REQUIRED)
│   ├── XpsUtil.vcxproj
│   └── XpsUtil.Projections\
│       └── XpsUtil.Projections.csproj
└── PSASamples\
    └── WinAppSdk\
        ├── CSharp\                <- baseline PSA sample (sibling; not required to build)
        └── CSharp_NativeHost\     <- this sample
            ├── PrintSupportApplication\
            ├── Tasks\
            └── Deploy\
```

### What you must extract

`CSharp_NativeHost\` is not self-contained on disk. Both `PrintSupportApplication.csproj` and `Tasks.csproj` carry a `<ProjectReference>` to `..\..\..\XpsUtil\XpsUtil.Projections\XpsUtil.Projections.csproj`, and the solution file pulls in `..\..\..\XpsUtil\XpsUtil.vcxproj`. The `XpsUtil` tree at the documented relative path is therefore required for the sample to build. The sibling `CSharp\` PSA sample is not required to build `CSharp_NativeHost\`; it is included in the same drop for reference.

### Why XpsUtil is shared

`XpsUtil` provides XPS document manipulation helpers used by the PSA's preview and print-ticket flows. Every C# PSA sample in this drop references it as a single project rather than duplicating the source, so that fixes propagate to every sample at once.

### What happens if you extract wrong

If `XpsUtil\` is missing from the expected path, the first build fails with:

```
error MSB3202: The project file "..\..\..\XpsUtil\XpsUtil.Projections\XpsUtil.Projections.csproj" was not found.
```

Resolve by copying the `XpsUtil\` folder into the sibling location shown in the layout above. Do not rewrite the `<ProjectReference>` paths; they intentionally pin the shared helper to the documented location.

---

## 3. Packaging a complete, self-contained runtime

This sample ships every dependency your PSA needs inside the MSIX. Customers install one package and the PSA runs on a clean machine with no prior .NET installation, no external downloads, and no machine-wide registrations. This is the right shape for Microsoft Store deployment and for any enterprise rollout where partners want execution to be deterministic across diverse customer machines.

The practical result for a C# PSA is:

1. Publish the managed Tasks assembly self-contained.
2. Place `hostfxr.dll`, `hostpolicy.dll`, `coreclr.dll`, the framework assemblies, `Tasks.dll`, `Tasks.deps.json`, `Tasks.runtimeconfig.json`, and `Tasks.winmd` in the MSIX payload.
3. Register a native in-process server that loads the bundled runtime and returns CsWinRT activation factories to COM.

The rest of this guide walks through how the sample implements each of those three pieces and what to copy into your own project.

---

## 4. Variants for partner adaptation

The sample as shipped makes two independent choices. You can keep both, or adjust one of them when integrating the code into your product.

### Delta 1 — Explicit managed bridge: keep it or skip it

The sample routes every `DllGetActivationFactory` call from C through the `[UnmanagedCallersOnly]` method `Tasks.NativeHost.NativeHostBridge.GetActivationFactory`. That bridge converts the inbound HSTRING to a managed string, locates the CsWinRT-generated per-assembly `WinRT.Module`, and returns the raw `IActivationFactory` ABI pointer for the requested class.

| Aspect | Explicit bridge | Direct generated-registry binding |
| --- | --- | --- |
| Native symbol resolved | Your named bridge method. | A generated activation helper. |
| Managed hook point | Yes; add ETW, allowlists, diagnostics, or product policy in one method. | Minimal; custom hooks need another layer. |
| Coupling to generated symbol names | Low; you own the bridge type and method names. | Higher; native code must match generated output. |
| Code footprint | Slightly larger. | Slightly smaller. |
| Reviewer story | Every activation request crosses a named method in your tree. | Activation is satisfied by generated component code. |

Keep the bridge if you need logging, class filtering, test hooks, or a stable review point. Skip it only after confirming your CsWinRT version emits a callable activation helper whose ABI matches the C typedef in `host.c`.

If you skip the bridge, update the three macros in `host.c` so they name the managed type and method you intend to bind, and remove `NativeHostBridge.cs` from your project only after the package activates successfully.

### Delta 2 — Bundled-only load vs runtime discovery

The sample composes `<NativeHostDir>\\hostfxr.dll` and loads that exact file with `LoadLibraryExW` plus `LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR`. This makes missing or corrupt MSIX runtime payloads fail loudly during development instead of silently using a machine runtime that customers will not have.

| Aspect | Bundled-only load used by this sample | Runtime discovery variant |
| --- | --- | --- |
| Store deployment on a clean machine | Works when the MSIX contains the runtime payload. | Works only if discovery resolves the bundled payload. |
| Broken bundle behavior | Hard failure with a distinct HRESULT and debugger output. | May depend on local machine state. |
| Build dependency | Native hosting headers only. | Requires linking `libnethost.lib`. |
| Transitive DLL search | Pinned to the package directory. | Depends on the selected discovery and load path. |

For Store-deployed PSAs, keep the bundled-only path. Consider runtime discovery only for enterprise deployments where an administrator owns the target runtime installation and your package intentionally does not carry the runtime.

---

## 5. Gotchas we hit and fixed so you do not have to

The sample has been verified end to end on a clean VM: package install, IPP printer association, PSA activation, foreground job UI, background workflow task, page-render callbacks, and repeated jobs.

### Gotcha 1 — SDK class libraries do not emit `runtimeconfig.json`

- **Symptom:** `hostfxr_initialize_for_dotnet_command_line` fails before managed code runs. `backgroundtaskhost.exe` exits unexpectedly and the managed bridge breadcrumbs never appear.
- **Root cause:** The .NET SDK target that writes build-time runtime config files is gated on executable output. `Tasks.csproj` is a class library, so setting `<GenerateRuntimeConfigurationFiles>true</GenerateRuntimeConfigurationFiles>` does not write `Tasks.runtimeconfig.json` next to `Tasks.dll`.
- **Fix in this sample:** `Tasks\\Tasks.csproj` defines `EmitTasksRuntimeConfig`. It writes a self-contained runtime config with the included framework and the CsWinRT configuration properties needed by hostfxr.

```xml
<Target Name="EmitTasksRuntimeConfig" AfterTargets="Build">
  <WriteLinesToFile File="$(OutputPath)Tasks.runtimeconfig.json"
                    Lines="$(_TasksRuntimeConfigJson)"
                    Overwrite="true"
                    Encoding="UTF-8" />
</Target>
```

### Gotcha 2 — WinAppSDK packaging silently drops payload that is not a default managed asset

- **Symptom:** Files that you copied into the build output (`NativeHost.dll`, `Tasks.deps.json`, `Tasks.runtimeconfig.json`, `Tasks.winmd`) are missing from the produced MSIX. The build completes with no error. Activation fails at runtime because the file the manifest or hostfxr expects is not in the package.
- **Root cause:** WinAppSDK's package computation walks MSBuild item lists, not the file system. Two classes of files are silently dropped:
 - **Non-default managed payload** (`.deps.json`, `.runtimeconfig.json`, `.winmd`) is not picked up by the default content rules and is not staged into the MSIX.
 - **Native DLLs from a build-only `<ProjectReference … ReferenceOutputAssembly="false">`** are opted out of the automatic MSIX-payload flow entirely, so the native shim never reaches the package even though the referenced project built it successfully.
- **Do not use `<AppxPackagePayload>` to fix this.** It is the documented WinAppSDK extension point for adding arbitrary files, but in WinAppSDK 1.6 the `%(AppxPackagePayload..TargetPath)` metadata expression does not resolve. The literal string is passed through as the destination path. With one item the file lands at the literal path and activation fails. With two or more items the build fails with `APPX1101` (duplicate-destination paths).
- **Fix in this sample:** `PrintSupportApplication\\PrintSupportApplication.csproj` lists each required payload as a normal `Content` item with a package-root `Link`. The same pattern stages `NativeHost.dll`, `Tasks.deps.json`, `Tasks.runtimeconfig.json`, and `Tasks.winmd`.

```xml
<Content Include="..\\Tasks\\NativeHost\\bin\\$(Platform)\\$(Configuration)\\NativeHost.dll">
  <Link>NativeHost.dll</Link>
  <CopyToOutputDirectory>PreserveNewest</CopyToOutputDirectory>
</Content>
<Content Include="..\\Tasks\\bin\\$(Platform)\\$(Configuration)\\$(TargetFramework)\\win-x64\\Tasks.runtimeconfig.json">
  <Link>Tasks.runtimeconfig.json</Link>
  <CopyToOutputDirectory>PreserveNewest</CopyToOutputDirectory>
</Content>
```

### Gotcha 3 — The native host's CRT must be statically linked

- **Symptom in Debug builds:** `NativeHost.dll` fails to load before `DllGetActivationFactory`, `InitializeRuntime`, or any managed breadcrumbs appear. The host process exits immediately on activation. This is guaranteed because Debug builds import `VCRUNTIME140D.dll` and `ucrtbased.dll`, which are developer-only DLLs and are never present on customer machines.
- **Risk in Release builds:** A default Release build imports `VCRUNTIME140.dll` (and `MSVCP140.dll` if C++ runtime is used). These ship through the Visual C++ Redistributable, which is commonly but not universally present on customer machines. An MSIX package cannot declare a dependency on the VC++ Redistributable, so a Release `NativeHost.dll` built with the default dynamic CRT can fail to load on clean customer machines that happen not to have it installed.
- **Fix in this sample:** `Tasks\\NativeHost\\NativeHost.vcxproj` statically links the CRT in both Debug and Release. The resulting `NativeHost.dll` imports only `KERNEL32.dll` and runs on any supported Windows version with no CRT redistributable dependency.

```xml
<RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>
<RuntimeLibrary>MultiThreaded</RuntimeLibrary>
```

### Gotcha 4 — The public `WinRT.Module` is not the generated activation registry

- **Symptom:** The managed bridge is reached, but factory lookup returns a wrapped managed exception or a null factory pointer.
- **Root cause:** CsWinRT emits an internal `WinRT.Module` type into each component assembly. That generated type owns the activation factory registry for classes in the component assembly. The public `WinRT.Module` from the CsWinRT runtime package is a different type and does not know about your activatable classes.
- **Fix in this sample:** `Tasks\\NativeHostBridge.cs` anchors reflection on a type inside `Tasks.dll`, finds the internal generated `WinRT.Module`, invokes `GetActivationFactory(string)` with public and non-public binding flags, and casts the result directly to `IntPtr`. It decodes the inbound HSTRING with `WindowsGetStringRawBuffer` rather than treating it as a raw `wchar_t*`.

```csharp
Assembly thisAssembly = typeof(NativeHostBridge).Assembly;
Type? moduleType = thisAssembly.GetType("WinRT.Module");
MethodInfo? getActivationFactory = moduleType?.GetMethod(
    "GetActivationFactory",
    BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic,
    null,
    new[] { typeof(string) },
    null);
```

---

## 6. Build and verify

### Build environment

**Prerequisites**

- **Windows 11**, build 26100 or later, on your build host.
- **Visual Studio with MSBuild 18 or later.** `Tasks.csproj` targets .NET SDK 10, which requires MSBuild 18. MSBuild 18 ships with Visual Studio 2026 (or its Preview). Visual Studio 2022 ships MSBuild 17 and cannot build this sample. If you have multiple VS installations side by side, point your build (Developer Command Prompt, GUI launch, or CI agent) at the newer one explicitly.
- **.NET SDK 10.0** or later, installed alongside the VS install above. Run `dotnet --list-sdks` to confirm a `10.0.*` entry is present.
- **Windows 10 SDK 26100** and the **Desktop development with C++** workload, both selectable in the Visual Studio Installer. The C++ workload provides the MSBuild C++ targets needed by `NativeHost.vcxproj`.

`dotnet publish` and `dotnet build` cannot build the sample on their own. `NativeHost.vcxproj` requires the C++ MSBuild targets that only ship with Visual Studio, so the build must go through `msbuild.exe` from a VS install or through the Visual Studio IDE.

**Build from Visual Studio**

1. Open `PrintSupportApplication\PrintSupportApplication.sln`.
2. Set the active configuration to `Release|x64` (or `Debug|x64` for diagnostic builds).
3. **Build → Build Solution.**
4. The MSIX is written to `PrintSupportApplication\bin\x64\Release\net10.0-windows10.0.26100.0\win-x64\AppPackages\PrintSupportApplication_1.0.0.0_x64_Test\PrintSupportApplication_1.0.0.0_x64.msix`.

**Build from the command line**

Use the Developer Command Prompt for your VS install. From the sample root:

```cmd
msbuild PrintSupportApplication\PrintSupportApplication.sln ^
    /t:Restore;Build ^
    /p:Configuration=Release ^
    /p:Platform=x64 ^
    /p:GenerateAppxPackageOnBuild=true
```

Substitute `Debug` for `Release` to produce the Debug-flavor MSIX. The output MSIX is at the path shown above.

### Verification checklist

Use this checklist before handing a partner port to QA or an IHV validation team.

1. **Build the MSIX for `x64`.** This sample's native host is x64-only as shipped. If you add ARM64, update the native host pack paths and repeat validation on ARM64 hardware or an ARM64 VM.
2. **Verify `NativeHost.dll` imports.** Run `dumpbin /imports NativeHost.dll`. Expect only `KERNEL32.dll`. Any `VCRUNTIME*.dll`, `ucrtbase*.dll`, or `api-ms-win-crt-*.dll` import means the CRT linkage regressed; see Gotcha 3.
3. **Inspect the MSIX as a zip.** Confirm these files are in the package root: `NativeHost.dll`, `Tasks.dll`, `Tasks.deps.json`, `Tasks.runtimeconfig.json`, `Tasks.winmd`, `hostfxr.dll`, `hostpolicy.dll`, and `coreclr.dll`.
4. **Run the registry-first validation in `TESTPLAN.md` §2.1.** This exercises the install order where the PSA registry association is written before the printer is installed. Use the printer-first order only for the production-recommended sequence in `TESTPLAN.md` §2.3.
5. **Watch DbgView during activation.** On success, expect the breadcrumb chain: `DLL_PROCESS_ATTACH` -> `InitializeRuntime success` -> `bridge returned hr=0x00000000` -> `OnLaunched: Kind=PrintSupportJobUI` -> page-render callbacks.
6. **Submit repeated jobs.** Run at least three print jobs on the clean VM. Each job should activate the host process, reach managed PSA code, render pages, drain the print queue, and exit without WER events.

---

## 7. Migration from the existing CSharp sample

Start here if your existing PSA uses Microsoft's stock `WinRT.Host.dll` for activation, which is the default in the baseline `CSharp` sample. The migration is mechanical, but it touches the Tasks project, the packaging project, the native shim, and one manifest line.

### What changes at a high level

1. **Add a native shim project.** Copy `Tasks\\NativeHost\\` from this sample, including `host.c`, `host.h`, `host.def`, and `NativeHost.vcxproj`. Keep the static CRT settings from the sample.
2. **Add a managed bridge.** Copy `Tasks\\NativeHostBridge.cs`. It reflects over the per-assembly internal `WinRT.Module` described in Gotcha 4.
3. **Update `Tasks.csproj`.** Add self-contained hosting properties, dynamic loading, native-hosting support, a CsWinRT generator exclusion for the bridge, and the `EmitTasksRuntimeConfig` target.
4. **Update `PrintSupportApplication.csproj`.** Build the native project and explicitly stage `NativeHost.dll`, `Tasks.deps.json`, `Tasks.runtimeconfig.json`, and `Tasks.winmd` into the MSIX root.
5. **Update `Package.appxmanifest`.** Change the in-process server path from `WinRT.Host.dll` to `NativeHost.dll`. Leave the activatable class IDs, threading model, capabilities, and print-support extensions unchanged.

### Step 1 — Copy the native shim project

```cmd
robocopy <path-to-sample>\\Tasks\\NativeHost ^
         <YourPsaRoot>\\Tasks\\NativeHost /E
```

Required files:

| File | Why you need it |
| --- | --- |
| `host.c` | Bootstraps hostfxr and forwards `DllGetActivationFactory` to managed code. |
| `host.h` | DLL-private declarations used by `host.c`. |
| `host.def` | Exports `DllGetActivationFactory` with the exact name COM activation expects. |
| `NativeHost.vcxproj` | Builds `NativeHost.dll` and statically links the CRT. |
| `NativeHost.vcxproj.filters` | Optional Solution Explorer layout. |

After copying, update the three `HOST_*` macros in `host.c` only if your managed assembly, namespace, or bridge method name differs from the sample.

### Step 2 — Copy the managed bridge

```cmd
copy <path-to-sample>\\Tasks\\NativeHostBridge.cs ^
     <YourPsaRoot>\\Tasks\\NativeHostBridge.cs
```

SDK-style C# projects compile `*.cs` files by default. If your project disables default compile items, add a manual `<Compile Include="NativeHostBridge.cs" />`.

### Step 3 — Update `Tasks.csproj`

Add the self-contained and native-hosting properties from this sample while keeping your existing target framework, CsWinRT package, project references, and business logic.

```xml
<PropertyGroup>
  <SelfContained>true</SelfContained>
  <PublishSelfContained>true</PublishSelfContained>
  <PublishTrimmed>false</PublishTrimmed>
  <EnableConsumingManagedCodeFromNativeHosting>true</EnableConsumingManagedCodeFromNativeHosting>
  <EnableDynamicLoading>true</EnableDynamicLoading>
</PropertyGroup>
<PropertyGroup>
  <CsWinRTComponent>true</CsWinRTComponent>
  <CsWinRTWindowsMetadata>10.0.26100.0</CsWinRTWindowsMetadata>
  <CsWinRTExcludeFromSourceGenerator>Tasks.NativeHost.NativeHostBridge</CsWinRTExcludeFromSourceGenerator>
</PropertyGroup>
<PropertyGroup Condition="'$(Platform)'=='x64'">
  <RuntimeIdentifier>win-x64</RuntimeIdentifier>
</PropertyGroup>
```

If your bridge namespace differs, update `CsWinRTExcludeFromSourceGenerator`. Then copy the `EmitTasksRuntimeConfig` target from this sample and rename the output file if your assembly is not named `Tasks.dll`.

### Step 4 — Update `PrintSupportApplication.csproj`

Add a build-only project reference to the native project:

```xml
<ProjectReference Include="..\\Tasks\\NativeHost\\NativeHost.vcxproj">
  <ReferenceOutputAssembly>false</ReferenceOutputAssembly>
  <SkipGetTargetFrameworkProperties>true</SkipGetTargetFrameworkProperties>
  <UndefineProperties>TargetFramework</UndefineProperties>
  <SetPlatform>Platform=x64</SetPlatform>
  <Private>false</Private>
</ProjectReference>
```

Then add the payload copy target and explicit `Content` entries from this sample so the package root contains `NativeHost.dll`, the managed assembly, `.deps.json`, `.runtimeconfig.json`, `.winmd`, and the bundled runtime files.

### Step 5 — Change the manifest in-process server path

```xml
<!-- Before: baseline CSharp sample -->
<Path>WinRT.Host.dll</Path>
<!-- After: CSharp_NativeHost sample -->
<Path>NativeHost.dll</Path>
```

Everything else in the `<InProcessServer>` block stays the same.

### Step 6 — Rebuild and verify

Rebuild the package, install it on a clean VM, and run the checklist in Section 6. Pay special attention to MSIX payload inspection: missing `.json`, `.winmd`, or runtime DLL files are package failures even when the build itself succeeds.

### What stays the same

- Managed background task code.
- App-level foreground UI flow.
- Assets, resources, and `ms-resource` bindings.
- Print-support extension declarations and activatable class IDs.
- Test certificate model and production signing handoff.
- Install script structure, except for package identity or filename changes if you rename the sample.

### What you gain

- A Store-deployable MSIX whose .NET runtime is bundled.
- A reproducible activation path under your control: native entry point, hostfxr load policy, managed bridge, and diagnostics all live in your source tree.
- Clear failure behavior when the runtime payload is missing or corrupt.

---

## 8. Limitations and caveats

These are the costs of this approach. They are not defects.

- **Native host ownership.** The shim is small and annotated, but it becomes your code. If you change runtime loading policy, assembly names, or diagnostics, you own those changes.
- **Self-contained component-hosting shape.** The sample uses the app-init hostfxr path with `Tasks.dll` as the command-line assembly so the default assembly load context is populated from `Tasks.deps.json`. This is the load shape this sample is built and tested against; if you change it, re-run the validation in Section 6.
- **x64-only as shipped.** ARM64 requires parallel native include paths, publish configuration, package validation, and hardware or VM coverage.
- **First-call latency.** The first activation pays the CoreCLR bootstrap cost. Subsequent calls use the cached function pointer. `InitOnce` serializes concurrent first calls.
- **Install ordering is your installer's call.** If you use an extension INF or your own installer to deploy the printer plus the PSA association, handle them however your install pipeline already does it. The sample does not impose an order; it only requires that the registry association exists by the time the printer is queried.
- **MSIX size.** A self-contained .NET + Windows App SDK PSA carries runtime payload files in the package. That size buys clean-machine execution.

---

## 9. Where to ask for help

- **Native-hosting docs:** https://github.com/dotnet/runtime/blob/main/docs/design/features/native-hosting.md
- **Reference samples:** https://github.com/dotnet/samples/tree/main/core/hosting
- **Native shim deep dive:** `Tasks\\NativeHost\\README.md`
- **Validation plan:** `TESTPLAN.md`
