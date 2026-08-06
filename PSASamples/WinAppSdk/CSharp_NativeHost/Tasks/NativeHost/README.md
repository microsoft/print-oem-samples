# NativeHost — hand-rolled .NET hosting shim for WinRT in-process activation

This directory contains the native DLL registered as the WinRT `InProcessServer` for the C# PSA background tasks. The DLL exports `DllGetActivationFactory`, loads the bundled .NET runtime from the MSIX package, resolves a managed bridge method in `Tasks.dll`, and forwards activation requests to the CsWinRT-generated factory registry.

The shim is intentionally small and uses only public Win32 and .NET native-hosting APIs. It exists so partners can review and customize the runtime bootstrap that runs inside `backgroundtaskhost.exe`.

---

## File layout

| File | Purpose |
| --- | --- |
| `host.c` | CoreCLR bootstrap and `DllGetActivationFactory` implementation. |
| `host.h` | Internal declarations shared inside this DLL. |
| `host.def` | Exports `DllGetActivationFactory` with an undecorated name. |
| `NativeHost.vcxproj` | Builds the native shim, sets include paths, exports, and CRT settings. |
| `README.md` | This file. |

The managed half of the contract lives one directory up in `..\\NativeHostBridge.cs` and is compiled into `Tasks.csproj`.

---

## How PSA activation flows through this shim

```
                        BackgroundTaskHost.exe
                                 |
                                 v
              combase: DllGetActivationFactory("Tasks.PrintSupportWorkflowBackgroundTask")
                                 |
                                 v
        +----------------------------------------------------------+
        | NativeHost.dll                                           |
        |  DllGetActivationFactory                                 |
        |   InitOnceExecuteOnce(InitializeRuntime)                  |
        |    GetModuleHandleEx(&InitializeRuntime) -> self          |
        |    GetModuleFileName(self) -> "<dir>\\NativeHost.dll"   |
        |    LoadLibraryEx("<dir>\\hostfxr.dll",                  |
        |      LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR)                   |
        |    hostfxr_initialize_for_dotnet_command_line(            |
        |      argv = { "<dir>\\Tasks.dll" })                    |
        |    hostfxr_get_runtime_delegate(                          |
        |      hdt_load_assembly_and_get_function_pointer)          |
        |    load_assembly_and_get_function_pointer(                 |
        |      "Tasks.dll",                                       |
        |      "Tasks.NativeHost.NativeHostBridge, Tasks",         |
        |      "GetActivationFactory",                            |
        |      UNMANAGEDCALLERSONLY_METHOD, ...) -> ptr             |
        |   ptr(activatableClassId, factory)                        |
        +-------------------------+--------------------------------+
                                  | raw function pointer call
                                  v
        +----------------------------------------------------------+
        | Tasks.dll                                                |
        |  NativeHostBridge.GetActivationFactory                   |
        |   WindowsGetStringRawBuffer(HSTRING)                     |
        |   generated WinRT.Module.GetActivationFactory(className)  |
        |   returns raw IActivationFactory*                         |
        +----------------------------------------------------------+
                                  |
                                  v
              combase receives IActivationFactory* and activates the task
```

---

## Bridge contract between native and managed code

The C shim resolves and caches one function pointer:

```c
typedef int32_t (__stdcall *pfn_get_activation_factory)(
    void* activatableClassId,
    void** factory);
```

The managed bridge must expose the matching signature:

```csharp
[UnmanagedCallersOnly]
public static unsafe int GetActivationFactory(
    IntPtr activatableClassId,
    IntPtr* factory);
```

Three macros in `host.c` define the managed target:

```c
#define HOST_MANAGED_ASSEMBLY_NAME  L"Tasks.dll"
#define HOST_BRIDGE_TYPE_NAME       L"Tasks.NativeHost.NativeHostBridge, Tasks"
#define HOST_BRIDGE_METHOD_NAME     L"GetActivationFactory"
```

Partners who rename the assembly or bridge update those macros and the matching namespace/type name in `NativeHostBridge.cs`.

---

## Self-contained runtime model

This shim runs in self-contained mode. It loads the `hostfxr.dll` bundled next to `NativeHost.dll` in the MSIX payload. It does not call `get_hostfxr_path` and it does not fall back to a system-installed .NET runtime.

### Why no system .NET fallback

A PSA distributed through the Microsoft Store has no documented way to declare a package dependency on a Microsoft-published .NET runtime package. The supported Store-ready path for this C# sample is to bundle the runtime. A system-runtime fallback would be dead code in production and harmful in development because it could hide broken package layout.

### File layout contract

The MSIX payload places these files in the same directory as `NativeHost.dll`:

| File | Provided by |
| --- | --- |
| `NativeHost.dll` | This native project. |
| `Tasks.dll` | Managed PSA assembly. |
| `Tasks.deps.json` | Managed publish output. |
| `Tasks.runtimeconfig.json` | Custom runtime-config target in `Tasks.csproj`. |
| `Tasks.winmd` | CsWinRT component metadata. |
| `hostfxr.dll` | .NET self-contained publish. |
| `hostpolicy.dll` | .NET self-contained publish. |
| `coreclr.dll` | .NET self-contained publish. |
| `System.*.dll`, `Microsoft.*.dll` | Framework assemblies from the publish output. |
| Windows App SDK runtime files | Windows App SDK self-contained publish. |

`PrintSupportApplication.csproj` owns the packaging rules that stage this layout.

### Failure mode when the bundle is incomplete

`InitializeRuntime` discovers its own DLL directory, composes `<NativeHostDir>\\hostfxr.dll`, and calls:

```c
LoadLibraryExW(hostfxrPath, NULL,
               LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR);
```

`LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR` is critical because it causes `hostfxr.dll`'s transitive dependencies (`hostpolicy.dll`, `coreclr.dll`, and framework assemblies) to resolve from the package directory. The shim deliberately does not combine this with `LOAD_WITH_ALTERED_SEARCH_PATH`: per MSDN, when any `LOAD_LIBRARY_SEARCH_*` flag is supplied, `LOAD_WITH_ALTERED_SEARCH_PATH` is silently ignored. It also deliberately does not add `LOAD_LIBRARY_SEARCH_DEFAULT_DIRS`, which would re-introduce the user-default and system directories and defeat the bundled-only policy.

On failure, the shim:

1. Emits `OutputDebugStringW` with the failing path, `LastError`, and HRESULT.
2. Returns `HOST_E_BUNDLED_RUNTIME_MISSING` (`0x80040200`) from `DllGetActivationFactory` so the failure is distinct from in-band hostfxr or CoreCLR errors.

Recovery is always to fix the MSIX bundling.

---

## Why the app init path is used

The textbook component-hosting entry point is `hostfxr_initialize_for_runtime_config(...)`. For this self-contained WinAppSDK PSA shape, that path does not produce the assembly load context needed by the Windows App SDK and CsWinRT payload.

The working path is to call `hostfxr_initialize_for_dotnet_command_line(argv = { Tasks.dll })` and request `hdt_load_assembly_and_get_function_pointer` without running `main`. That causes the hosting layer to read `Tasks.deps.json`, populate the default assembly load context from the self-contained payload, and return a raw function pointer to the managed `[UnmanagedCallersOnly]` bridge method.

---

## Single-CoreCLR invariant

The shim loads one runtime against one managed assembly and never re-enters the hosting layer after the first successful initialization. The PSA process model already gives each package/task activation its own `backgroundtaskhost.exe` instance, so the CoreCLR per-process singleton is respected.

---

## Thread safety and lifecycle

- Initialization is gated by `InitOnceExecuteOnce`. Concurrent first-call activation requests wait on the same InitOnce state and observe the published `g_managed_bridge` pointer.
- Failed initialization reports the real HRESULT through `g_init_hresult` rather than collapsing every failure to `E_FAIL`.
- `DllMain DLL_PROCESS_DETACH` does not tear down CoreCLR. Unloading CoreCLR from inside the loader lock is unsafe; the OS reclaims the runtime when the host process exits.

---

## APIs touched

| API | Header | Status |
| --- | --- | --- |
| `GetModuleHandleExW` / `GetModuleFileNameW` | `windows.h` | Public Win32 APIs. |
| `LoadLibraryExW` | `windows.h` | Public Win32 loader API. |
| `hostfxr_initialize_for_dotnet_command_line` | `hostfxr.h` | Public .NET hosting API. |
| `hostfxr_get_runtime_delegate` | `hostfxr.h` | Public .NET hosting API. |
| `hostfxr_close` | `hostfxr.h` | Public .NET hosting API. |
| `load_assembly_and_get_function_pointer_fn` | `coreclr_delegates.h` | Public .NET hosting delegate type. |
| `WindowsGetStringRawBuffer` | WinRT string API | Public HSTRING decoding API used by the managed bridge. |
| Generated `WinRT.Module.GetActivationFactory(string)` | CsWinRT output | Generated when `<CsWinRTComponent>true</CsWinRTComponent>` is set. |

Reference docs:

- https://github.com/dotnet/runtime/blob/main/docs/design/features/native-hosting.md
- https://github.com/dotnet/samples/tree/main/core/hosting

---

## What partners need to add to their projects

- **`NativeHost.vcxproj`** — include the .NET SDK native-hosting headers, compile `host.c`, statically link the CRT, and export `DllGetActivationFactory` via `host.def`.
- **MSIX packaging** — publish self-contained and stage the publish output next to `NativeHost.dll` in the package root.
- **`Tasks.csproj`** — enable self-contained native hosting, exclude the bridge from CsWinRT runtime-class generation, and emit `Tasks.runtimeconfig.json` for the class-library output.
- **`Package.appxmanifest`** — point the existing `<InProcessServer><Path>` at `NativeHost.dll`. Keep the activatable class IDs unchanged.

No other production code needs to know the shim exists.
