// ---------------------------------------------------------------------------
// host.c - Hand-rolled .NET hosting shim for WinRT in-process activation.
//
// PURPOSE
//   This native DLL is registered as the <Path> on an <InProcessServer>
//   element in Package.appxmanifest for a SELF-CONTAINED .NET WinAppSDK
//   Print Support Application (PSA). It provides the native bootstrap
//   required to load the managed Tasks assembly using only public .NET
//   native-hosting APIs.
//
// SELF-CONTAINED ONLY - NO SYSTEM .NET FALLBACK
//   This shim ONLY loads the BUNDLED hostfxr.dll that ships next to
//   NativeHost.dll in the MSIX payload. It deliberately does NOT call
//   nethost's get_hostfxr_path and it deliberately does NOT fall back
//   to a system-installed .NET runtime.
//
// SCOPE
//   - Exports: DllGetActivationFactory  (see host.def)
//   - First call performs CoreCLR bootstrap and resolves the managed bridge.
//   - All subsequent calls tail-call the managed bridge directly.
//
// APIs USED  (every one is public, documented native-hosting surface)
//   hostfxr.h            : hostfxr_initialize_for_dotnet_command_line,
//                          hostfxr_get_runtime_delegate,
//                          hostfxr_close
//   coreclr_delegates.h  : load_assembly_and_get_function_pointer_fn
//
//   nethost.h / get_hostfxr_path is deliberately NOT used. nethost would
//   try the bundled hostfxr first but would happily fall back to a
//   system install if the bundled file were missing. That fallback would
//   silently degrade the self-contained package contract.
//
//   Reference docs:
//     dotnet/runtime/docs/design/features/native-hosting.md
//     dotnet/samples/core/hosting          (textbook sample)
//
// BUNDLED RUNTIME LAYOUT CONTRACT
//   The MSIX package places hostfxr.dll, coreclr.dll, hostpolicy.dll,
//   System.*.dll, and Tasks.dll all in the same directory as
//   NativeHost.dll (the InProcessServer Path). The build publishes the
//   app as self-contained, then copies the publish output into the appx
//   package root next to NativeHost.dll.
//
//   This shim therefore composes paths relative to its own module
//   location - "self" being NativeHost.dll - and loads
//   "<NativeHostDir>\hostfxr.dll" and "<NativeHostDir>\Tasks.dll".
//
// WHY THE "APP" INIT PATH AND NOT THE "COMPONENT" INIT PATH
//   The textbook component-hosting entry point is
//   hostfxr_initialize_for_runtime_config (takes Tasks.runtimeconfig.json).
//   For SELF-CONTAINED WinAppSDK components it returns 0x80008093 because
//   the resolver rejects "frameworkdeps + self-contained" at
//   x_resolver_t::resolve_frameworks_for_app, and even when it does load it
//   trims the TPA (Trusted Platform Assemblies) such that WinAppSDK's
//   200+ assemblies fail to resolve at runtime.
//
//   The working pattern is to call the *app* init entry point
//   hostfxr_initialize_for_dotnet_command_line(argv = { "Tasks.dll" })
//   and then request hdt_load_assembly_and_get_function_pointer instead of
//   running main. This populates the default ALC from Tasks.deps.json and
//   makes the full TPA available, which is what self-contained WinAppSDK
//   needs. This combination uses the documented native-hosting flow.
//
//
// SINGLE-CORECLR INVARIANT
//   coreclr.dll enforces a per-process EEStartup singleton. PSA's process
//   model (one BackgroundTaskHost.exe per (package, task) tuple, AppContainer
//   SID per package, single InProcessServer per BTH) already guarantees that
//   only this DLL's Tasks.dll loads in this BTH. This shim does NOT broaden
//   that surface - it loads exactly one runtime against exactly one assembly.
//
// THREAD SAFETY
//   BTH may dispatch DllGetActivationFactory on a thread-pool thread. The
//   first-call bootstrap is serialized through InitOnceExecuteOnce, which
//   guarantees exactly-once semantics across concurrent callers and
//   publishes the resolved managed function pointer with a release barrier.
//
// CLEANUP
//   We do NOT close the hostfxr context on DLL_PROCESS_DETACH. Per the
//   native-hosting docs ("Hosting layer life-cycle"), the hosting context
//   only needs to be closed when finished with it for a *single*
//   initialize/run cycle; coreclr itself is designed to live for the
//   process lifetime and is torn down by the loader/OS on process exit.
//   Attempting to close coreclr during DLL_PROCESS_DETACH from inside the
//   loader lock is explicitly unsafe (managed finalizers, JIT helpers, GC
//   threads). We close the *initial* hostfxr_handle right after extracting
//   the loader delegate (see InitializeRuntime); the runtime itself remains
//   loaded and is reclaimed at process tear-down.
// ---------------------------------------------------------------------------

#include "host.h"

#include <stdint.h>
#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>

// Public .NET native-hosting headers. These ship in the .NET SDK at
//   packs\Microsoft.NETCore.App.Host.win-<arch>\<version>\runtimes\win-<arch>\native\
// and are added to AdditionalIncludeDirectories by NativeHost.vcxproj.
#include <hostfxr.h>
#include <coreclr_delegates.h>

// ---------------------------------------------------------------------------
// Compile-time configuration. Partners adapt these three constants for
// their own assembly / bridge type / bridge method. Everything else in
// this file is invariant.
// ---------------------------------------------------------------------------

// The managed assembly that contains the bridge. Lives next to this DLL
// in the MSIX payload (standard InProcessServer layout).
#define HOST_MANAGED_ASSEMBLY_NAME      L"Tasks.dll"

// Assembly-qualified type name of the [UnmanagedCallersOnly] bridge class.
// "<Namespace>.<Type>, <AssemblySimpleName>"
#define HOST_BRIDGE_TYPE_NAME           L"Tasks.NativeHost.NativeHostBridge, Tasks"

// Method name on the bridge type. The method's signature MUST match
// pfn_get_activation_factory below and MUST carry [UnmanagedCallersOnly].
#define HOST_BRIDGE_METHOD_NAME         L"GetActivationFactory"

// The bundled runtime entry point that must live next to NativeHost.dll
// in the MSIX payload. See "BUNDLED RUNTIME LAYOUT CONTRACT" above.
#define HOST_BUNDLED_HOSTFXR_NAME       L"hostfxr.dll"

// ---------------------------------------------------------------------------
// Custom HRESULT for "bundled hostfxr.dll is not present (or failed to
// load) next to NativeHost.dll." Chosen value:
//
//   MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0200) == 0x80040200
//
// FACILITY_ITF is the standard COM facility for interface-defined error
// codes in the 0x0200-0xFFFF range; 0x0200 is the first slot the .NET
// docs reserve for "user-defined" within FACILITY_ITF. This code is
// stable, distinct from any hostfxr/coreclr return code, and easy to
// recognise in a dump / ETW trace as "NativeHost bundled-runtime
// missing". The companion OutputDebugStringW message carries the
// failing path for human-readable diagnosis.
// ---------------------------------------------------------------------------
#define HOST_E_BUNDLED_RUNTIME_MISSING  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0200)

// ---------------------------------------------------------------------------
// Cross-boundary contract for the managed bridge.
//
// C side  : int (__stdcall *)(HSTRING activatableClassId, void** factory)
// C# side : [UnmanagedCallersOnly]
//           static int GetActivationFactory(IntPtr activatableClassId, IntPtr* factory)
//
// The HSTRING is forwarded verbatim. The bridge owns:
//   - HSTRING -> System.String conversion (via MarshalString.FromAbi)
//   - WinRT class lookup
//   - IActivationFactory CCW construction
//   - Marshalling the CCW back as a raw COM pointer (AddRef'd)
//   - Mapping any managed exception to a stable HRESULT (never throw).
// ---------------------------------------------------------------------------
typedef int32_t (__stdcall *pfn_get_activation_factory)(void* activatableClassId,
                                                        void** factory);

// ---------------------------------------------------------------------------
// Mutable state. All writes happen exactly once inside the InitOnce callback;
// reads after a successful InitOnceExecuteOnce return enjoy the implicit
// release/acquire barrier that InitOnce provides.
// ---------------------------------------------------------------------------
static INIT_ONCE g_init_once = INIT_ONCE_STATIC_INIT;
static pfn_get_activation_factory g_managed_bridge = NULL;

// Last HRESULT from the bootstrap. Exposed via host.h so diagnostics tools
// (or a future test-only export) can read the real failure code rather than
// the synthetic E_FAIL we return from DllGetActivationFactory.
volatile LONG g_init_hresult = S_OK;

// HMODULE for our own DLL, captured once. Used to derive the directory that
// contains hostfxr.dll and Tasks.dll (both bundled siblings in the MSIX).
static HMODULE g_self_module = NULL;

// ---------------------------------------------------------------------------
// Trace - emit a partner-readable diagnostic to OutputDebugStringW so a live
// DbgView / TraceView session can prove which phase of activation the shim
// reached. Tagged with "[NativeHost]" so test scripts can grep for it.
// All calls are best-effort; failures inside the formatter never propagate.
// Use in addition to (not instead of) the existing ReportBundledHostfxrMissing
// failure-path diagnostic. Keep messages short - OutputDebugStringW is best
// when the consumer can scroll through many lines.
// ---------------------------------------------------------------------------
static void Trace(_In_z_ _Printf_format_string_ const wchar_t* fmt, ...)
{
    wchar_t msg[512];
    va_list args;
    va_start(args, fmt);
    int n = _vsnwprintf_s(msg, ARRAYSIZE(msg), _TRUNCATE, fmt, args);
    va_end(args);
    if (n > 0)
    {
        OutputDebugStringW(msg);
    }
}

// ---------------------------------------------------------------------------
// BuildSiblingPath - given the absolute path of *this* DLL, replace the
// trailing filename with `leaf` and write the result into `out`.
// Returns S_OK or a Win32-derived HRESULT on overflow / malformed input.
// ---------------------------------------------------------------------------
static HRESULT BuildSiblingPath(_In_ PCWSTR selfPath,
                                _In_ PCWSTR leaf,
                                _Out_writes_z_(outChars) PWSTR out,
                                _In_ size_t outChars)
{
    if (selfPath == NULL || leaf == NULL || out == NULL || outChars == 0)
    {
        return E_INVALIDARG;
    }

    // Locate the final path separator. Self-contained MSIX paths always
    // have a backslash because they are absolute under WindowsApps\.
    PCWSTR lastSep = wcsrchr(selfPath, L'\\');
    if (lastSep == NULL)
    {
        return E_UNEXPECTED;
    }

    size_t dirChars = (size_t)(lastSep - selfPath) + 1; // include the backslash
    size_t leafChars = wcslen(leaf);
    if (dirChars + leafChars + 1 > outChars)
    {
        return HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
    }

    wmemcpy(out, selfPath, dirChars);
    wmemcpy(out + dirChars, leaf, leafChars + 1); // include terminator
    return S_OK;
}

// ---------------------------------------------------------------------------
// ReportBundledHostfxrMissing - emit a partner-readable diagnostic to the
// debugger output stream describing exactly which path failed to load and
// why it matters. Used on the failure leg of LoadLibraryExW(hostfxr).
// ---------------------------------------------------------------------------
static void ReportBundledHostfxrMissing(_In_ PCWSTR hostfxrPath, _In_ DWORD win32err)
{
    wchar_t msg[1024];
    // swprintf_s returns -1 on overflow; we tolerate that and still emit a
    // best-effort static message so a partner debugging the failure always
    // sees something.
    int n = swprintf_s(msg, ARRAYSIZE(msg),
        L"NativeHost: bundled hostfxr.dll not found or failed to load at "
        L"\"%s\" (LastError=%lu). Self-contained runtime is REQUIRED for "
        L"PSA Microsoft Store deployment - there is no system .NET "
        L"fallback. Verify the MSIX build staged hostfxr.dll, coreclr.dll, "
        L"hostpolicy.dll and the framework assemblies next to "
        L"NativeHost.dll. Returning HRESULT 0x%08lX "
        L"(HOST_E_BUNDLED_RUNTIME_MISSING).\n",
        hostfxrPath ? hostfxrPath : L"(null)",
        win32err,
        (unsigned long)HOST_E_BUNDLED_RUNTIME_MISSING);
    if (n < 0)
    {
        OutputDebugStringW(L"NativeHost: bundled hostfxr.dll missing; "
                           L"self-contained runtime required for Store "
                           L"deployment. No system fallback.\n");
    }
    else
    {
        OutputDebugStringW(msg);
    }
}

// ---------------------------------------------------------------------------
// InitializeRuntime - bootstrap CoreCLR and cache the managed bridge.
//
// Returns S_OK on success. On failure returns the first non-S_OK HRESULT
// encountered, and also stashes that HRESULT in g_init_hresult for
// diagnosability. Missing bundled hostfxr.dll returns the custom
// HOST_E_BUNDLED_RUNTIME_MISSING (0x80040200) so the failure cause is
// distinguishable from any in-band hostfxr / coreclr return code.
// ---------------------------------------------------------------------------
static HRESULT InitializeRuntime(void)
{
    Trace(L"[NativeHost] InitializeRuntime begin\n");
    // Step 0: discover our own DLL's path so we can locate hostfxr.dll and
    // Tasks.dll next to us. GetModuleHandleEx with
    // GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS is the documented "find me
    // by my own code address" pattern; UNCHANGED_REFCOUNT avoids
    // incrementing the loader refcount (we already own a reference via
    // having been loaded).
    if (g_self_module == NULL)
    {
        if (!GetModuleHandleExW(
                GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                (LPCWSTR)(void*)&InitializeRuntime,
                &g_self_module))
        {
            DWORD lastErr = GetLastError();
            Trace(L"[NativeHost] GetModuleHandleExW FAILED, win32=%lu\n", lastErr);
            return HRESULT_FROM_WIN32(lastErr);
        }
    }

    wchar_t selfPath[MAX_PATH];
    DWORD selfPathLen = GetModuleFileNameW(g_self_module, selfPath, ARRAYSIZE(selfPath));
    if (selfPathLen == 0 || selfPathLen >= ARRAYSIZE(selfPath))
    {
        DWORD lastErr = GetLastError();
        Trace(L"[NativeHost] GetModuleFileNameW FAILED, len=%lu win32=%lu\n",
              selfPathLen, lastErr);
        return HRESULT_FROM_WIN32(lastErr ? lastErr : ERROR_INSUFFICIENT_BUFFER);
    }
    Trace(L"[NativeHost] self path: %s\n", selfPath);

    // Compose the absolute path to Tasks.dll (our managed sibling).
    wchar_t assemblyPath[MAX_PATH];
    HRESULT hr = BuildSiblingPath(selfPath, HOST_MANAGED_ASSEMBLY_NAME,
                                  assemblyPath, ARRAYSIZE(assemblyPath));
    if (FAILED(hr))
    {
        Trace(L"[NativeHost] BuildSiblingPath(Tasks.dll) FAILED hr=0x%08lX\n",
              (unsigned long)hr);
        return hr;
    }

    // Compose the absolute path to the BUNDLED hostfxr.dll.
    wchar_t hostfxrPath[MAX_PATH];
    hr = BuildSiblingPath(selfPath, HOST_BUNDLED_HOSTFXR_NAME,
                          hostfxrPath, ARRAYSIZE(hostfxrPath));
    if (FAILED(hr))
    {
        Trace(L"[NativeHost] BuildSiblingPath(hostfxr.dll) FAILED hr=0x%08lX\n",
              (unsigned long)hr);
        return hr;
    }

    // ---------------------------------------------------------------
    // Step 1: load the BUNDLED hostfxr.dll. No system fallback.
    //
    // LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR adds the directory containing
    // the file being loaded (hostfxr.dll, which sits next to us in
    // the MSIX payload) to the loader's search list for the
    // *transitive* dependencies. This guarantees that hostpolicy.dll,
    // coreclr.dll, and the framework DLLs all resolve from the
    // bundled-runtime directory rather than from anywhere else on the
    // machine - critical for AppContainer activation and for
    // guaranteeing we are not accidentally mixing a bundled hostfxr
    // against a system coreclr.
    //
    // We deliberately do NOT pair this with LOAD_WITH_ALTERED_SEARCH_PATH:
    // per MSDN, when any LOAD_LIBRARY_SEARCH_* flag is supplied alongside
    // LOAD_WITH_ALTERED_SEARCH_PATH the altered-search-path value is
    // silently ignored, so it would be dead noise here.
    //
    // We deliberately do NOT add LOAD_LIBRARY_SEARCH_DEFAULT_DIRS either:
    // that flag would re-introduce the user-default + system search
    // directories, which defeats the SELF-CONTAINED ONLY policy.
    //
    // If LoadLibraryExW fails: FAIL LOUDLY. We do NOT call
    // get_hostfxr_path. We do NOT probe system .NET. We emit a
    // diagnostic and return HOST_E_BUNDLED_RUNTIME_MISSING.
    // ---------------------------------------------------------------
    Trace(L"[NativeHost] LoadLibraryExW begin path=\"%s\"\n", hostfxrPath);
    HMODULE hostfxr = LoadLibraryExW(hostfxrPath, NULL,
                                     LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR);
    if (hostfxr == NULL)
    {
        DWORD lastErr = GetLastError();
        Trace(L"[NativeHost] LoadLibraryExW(hostfxr) FAILED win32=%lu\n", lastErr);
        ReportBundledHostfxrMissing(hostfxrPath, lastErr);
        return HOST_E_BUNDLED_RUNTIME_MISSING;
    }
    Trace(L"[NativeHost] LoadLibraryExW(hostfxr) success, hmodule=0x%p\n",
          (void*)hostfxr);

    // From here on, every failure return path MUST FreeLibrary(hostfxr)
    // before returning. InitOnceCallback returns FALSE on failure, which
    // re-arms the InitOnce so the next activation re-enters
    // InitializeRuntime - without FreeLibrary, each retry would leak
    // another HMODULE reference. The success path intentionally leaves
    // hostfxr loaded for the process lifetime (see "CLEANUP" in the
    // file header).

    // ---------------------------------------------------------------
    // Step 2: resolve the hostfxr exports we use. If any are missing
    // the bundled hostfxr.dll is the wrong version / corrupted - we
    // still treat this as bundled-runtime failure because the only
    // recovery action is "fix the MSIX bundling".
    // ---------------------------------------------------------------
    hostfxr_initialize_for_dotnet_command_line_fn initForCmd =
        (hostfxr_initialize_for_dotnet_command_line_fn)
            GetProcAddress(hostfxr, "hostfxr_initialize_for_dotnet_command_line");
    hostfxr_get_runtime_delegate_fn getDelegate =
        (hostfxr_get_runtime_delegate_fn)
            GetProcAddress(hostfxr, "hostfxr_get_runtime_delegate");
    hostfxr_close_fn closeCtx =
        (hostfxr_close_fn)GetProcAddress(hostfxr, "hostfxr_close");

    if (initForCmd == NULL || getDelegate == NULL || closeCtx == NULL)
    {
        Trace(L"[NativeHost] GetProcAddress FAILED initForCmd=%p getDelegate=%p closeCtx=%p\n",
              (void*)initForCmd, (void*)getDelegate, (void*)closeCtx);
        ReportBundledHostfxrMissing(hostfxrPath, ERROR_PROC_NOT_FOUND);
        (void)FreeLibrary(hostfxr);
        return HOST_E_BUNDLED_RUNTIME_MISSING;
    }
    Trace(L"[NativeHost] hostfxr exports resolved\n");

    // ---------------------------------------------------------------
    // Step 3: initialize CoreCLR via the *app* path.
    //
    // argv[0] is the application path. The hosting layer reads
    // Tasks.deps.json and Tasks.runtimeconfig.json from alongside it
    // to build the TPA and configure the runtime properties. We pass
    // exactly one argv element (the assembly path); we never call
    // hostfxr_run_app - we only use the resulting context to obtain
    // a loader delegate, which is the documented "host a managed
    // component from a native app" recipe.
    // ---------------------------------------------------------------
    hostfxr_handle ctx = NULL;
    const wchar_t* argv[1];
    argv[0] = assemblyPath;

    Trace(L"[NativeHost] hostfxr_initialize_for_dotnet_command_line begin assembly=\"%s\"\n",
          assemblyPath);
    int rc = initForCmd(1, argv, NULL, &ctx);
    Trace(L"[NativeHost] hostfxr_initialize_for_dotnet_command_line returned rc=0x%08x ctx=0x%p\n",
          (unsigned)rc, (void*)ctx);

    // The hosting docs note that hostfxr_initialize_for_* can return
    // a "success but with non-zero status" value (e.g.
    // Success_HostAlreadyInitialized = 0x00000001). We treat any
    // value with the high bit clear as success.
    if (rc < 0)
    {
        Trace(L"[NativeHost] hostfxr_initialize FAILED rc=0x%08x\n", (unsigned)rc);
        if (ctx != NULL) { (void)closeCtx(ctx); }
        (void)FreeLibrary(hostfxr);
        return (HRESULT)rc;
    }
    if (ctx == NULL)
    {
        Trace(L"[NativeHost] hostfxr_initialize returned NULL ctx with success rc\n");
        (void)FreeLibrary(hostfxr);
        return E_UNEXPECTED;
    }

    // ---------------------------------------------------------------
    // Step 4: get the load_assembly_and_get_function_pointer delegate.
    // This is the documented entry point for "load a managed assembly
    // and resolve one of its [UnmanagedCallersOnly] methods".
    // ---------------------------------------------------------------
    void* delegateRaw = NULL;
    Trace(L"[NativeHost] hostfxr_get_runtime_delegate(load_assembly_and_get_function_pointer) begin\n");
    rc = getDelegate(ctx,
                     hdt_load_assembly_and_get_function_pointer,
                     &delegateRaw);
    Trace(L"[NativeHost] hostfxr_get_runtime_delegate returned rc=0x%08x delegate=0x%p\n",
          (unsigned)rc, delegateRaw);

    // Per the docs: "After the delegate is obtained the context is no
    // longer needed and should be closed." Closing the context does
    // NOT unload the runtime - the runtime stays alive process-wide.
    (void)closeCtx(ctx);

    if (rc < 0 || delegateRaw == NULL)
    {
        Trace(L"[NativeHost] hostfxr_get_runtime_delegate FAILED rc=0x%08x\n", (unsigned)rc);
        (void)FreeLibrary(hostfxr);
        return (HRESULT)(rc != 0 ? rc : (int)E_UNEXPECTED);
    }

    load_assembly_and_get_function_pointer_fn loadAndGet =
        (load_assembly_and_get_function_pointer_fn)delegateRaw;

    // ---------------------------------------------------------------
    // Step 5: resolve the managed bridge function pointer.
    //
    // UNMANAGEDCALLERSONLY_METHOD as delegate_type_name tells coreclr
    // "the target method is [UnmanagedCallersOnly], don't synthesize
    // a delegate marshaller, just hand me the raw function pointer".
    // ---------------------------------------------------------------
    void* bridgeRaw = NULL;
    Trace(L"[NativeHost] load_assembly_and_get_function_pointer begin type=\"%s\" method=\"%s\"\n",
          HOST_BRIDGE_TYPE_NAME, HOST_BRIDGE_METHOD_NAME);
    rc = loadAndGet(assemblyPath,
                    HOST_BRIDGE_TYPE_NAME,
                    HOST_BRIDGE_METHOD_NAME,
                    UNMANAGEDCALLERSONLY_METHOD,
                    NULL,
                    &bridgeRaw);
    Trace(L"[NativeHost] load_assembly_and_get_function_pointer returned rc=0x%08x bridge=0x%p\n",
          (unsigned)rc, bridgeRaw);

    if (rc < 0 || bridgeRaw == NULL)
    {
        Trace(L"[NativeHost] managed bridge resolution FAILED rc=0x%08x\n", (unsigned)rc);
        (void)FreeLibrary(hostfxr);
        return (HRESULT)(rc != 0 ? rc : (int)E_UNEXPECTED);
    }

    g_managed_bridge = (pfn_get_activation_factory)bridgeRaw;
    Trace(L"[NativeHost] InitializeRuntime success, bridge=0x%p\n",
          (void*)g_managed_bridge);
    return S_OK;
}

// ---------------------------------------------------------------------------
// InitOnce callback. Returning FALSE makes InitOnceExecuteOnce report
// failure to every concurrent caller and re-arms the InitOnce so that a
// future call may retry. We intentionally do NOT re-arm on success: once
// the runtime is up, it stays up for the process lifetime.
// ---------------------------------------------------------------------------
static BOOL CALLBACK InitOnceCallback(PINIT_ONCE initOnce, PVOID parameter, PVOID* context)
{
    (void)initOnce; (void)parameter; (void)context;

    HRESULT hr = InitializeRuntime();
    InterlockedExchange(&g_init_hresult, (LONG)hr);
    return SUCCEEDED(hr) ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
// DllGetActivationFactory - the ONLY native-callable entry point.
//
// Signature must match the WinRT contract exactly:
//   HRESULT STDAPICALLTYPE DllGetActivationFactory(
//       _In_  HSTRING                 activatableClassId,
//       _Out_ IActivationFactory**    factory);
//
// On first invocation we bootstrap the runtime; on every invocation we
// forward to the managed bridge. The bridge converts HSTRING, performs
// CsWinRT activation-factory lookup, and returns the CCW.
// ---------------------------------------------------------------------------
STDAPI DllGetActivationFactory(_In_ HSTRING activatableClassId,
                               _COM_Outptr_ IActivationFactory** factory)
{
    // ETW-equivalent breadcrumb so DbgView in the host process proves the
    // export was actually invoked. Use a const-string trace (no decode of
    // the HSTRING) - the managed bridge logs the class name itself.
    Trace(L"[NativeHost] DllGetActivationFactory entry, tid=%lu\n",
          GetCurrentThreadId());

    if (factory == NULL)
    {
        Trace(L"[NativeHost] DllGetActivationFactory: factory==NULL -> E_POINTER\n");
        return E_POINTER;
    }
    *factory = NULL;

    if (!InitOnceExecuteOnce(&g_init_once, InitOnceCallback, NULL, NULL))
    {
        LONG saved = InterlockedCompareExchange(&g_init_hresult, 0, 0);
        Trace(L"[NativeHost] InitOnceExecuteOnce FAILED, g_init_hresult=0x%08lX\n",
              (unsigned long)saved);
        // Surface the real bootstrap HRESULT if we have one; otherwise
        // fall back to E_FAIL. For missing bundled hostfxr the caller
        // will see HOST_E_BUNDLED_RUNTIME_MISSING (0x80040200) here
        // and a matching OutputDebugStringW diagnostic in the trace.
        return saved != S_OK ? (HRESULT)saved : E_FAIL;
    }

    // Belt-and-braces: a true single-threaded InitOnce success path
    // always publishes g_managed_bridge, but tolerate the impossible.
    if (g_managed_bridge == NULL)
    {
        Trace(L"[NativeHost] g_managed_bridge==NULL after InitOnce success -> E_UNEXPECTED\n");
        return E_UNEXPECTED;
    }

    HRESULT hr = (HRESULT)g_managed_bridge((void*)activatableClassId, (void**)factory);
    Trace(L"[NativeHost] bridge returned hr=0x%08lX, factory=0x%p\n",
          (unsigned long)hr, *factory);
    return hr;
}

// ---------------------------------------------------------------------------
// DllMain - intentionally minimal.
//
// On DLL_PROCESS_ATTACH we disable per-thread callbacks (we have no TLS
// to manage) and otherwise do nothing. CoreCLR bootstrap is deferred to
// the first DllGetActivationFactory call.
//
// On DLL_PROCESS_DETACH we do NOT close hostfxr / unload coreclr / null
// out g_managed_bridge. Per the .NET native-hosting documentation the
// runtime is expected to live for the process lifetime; tearing it down
// from inside the loader lock would deadlock or crash (managed finalizers
// take user locks, the GC has running threads, JIT helpers run at random
// addresses). The OS reclaims everything cleanly at process exit, which
// for a BTH activation is what we want.
// ---------------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID reserved)
{
    (void)reserved;
    if (reason == DLL_PROCESS_ATTACH)
    {
        (void)DisableThreadLibraryCalls(hinst);
        g_self_module = (HMODULE)hinst;
        // Earliest possible breadcrumb: proves the loader resolved NativeHost.dll
        // and DllMain ran. If you never see this string in a DbgView session
        // during print, the OS never asked combase to load NativeHost.dll - the
        // problem is upstream of the activation factory (AUMID resolution,
        // workflow association, or print path bypassing PSA entirely).
        OutputDebugStringW(L"[NativeHost] DLL_PROCESS_ATTACH\n");
    }
    return TRUE;
}
