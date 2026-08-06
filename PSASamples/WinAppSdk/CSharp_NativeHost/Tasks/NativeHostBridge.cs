// ---------------------------------------------------------------------------
// NativeHostBridge.cs
//
// Managed half of the native <-> managed contract for the hand-rolled
// CoreCLR hosting shim in Tasks\NativeHost\host.c.
//
// The native shim (NativeHost.dll) exports DllGetActivationFactory.
// On first call it bootstraps CoreCLR, loads Tasks.dll, resolves THIS
// method via load_assembly_and_get_function_pointer + UNMANAGEDCALLERSONLY,
// caches the function pointer, and forwards every subsequent activation
// request to us. We are responsible for:
//
//   1. Converting the inbound HSTRING to a managed System.String.
//   2. Looking up the WinRT class's activation factory through the
//      CsWinRT-generated per-assembly WinRT.Module registry.
//   3. Returning the raw IActivationFactory ABI pointer already produced
//      by that generated registry.
//   4. Mapping any managed exception to a stable HRESULT - we MUST NOT
//      let an exception unwind across the native boundary; that would
//      tear down BackgroundTaskHost.exe with a corrupted SEH frame.
//
// CONTRACT (must match the typedef pfn_get_activation_factory in host.c):
//
//   C:   int __stdcall (*)(void* activatableClassId, void** factory);
//   C#:  [UnmanagedCallersOnly]
//        static int GetActivationFactory(IntPtr activatableClassId, IntPtr* factory);
//
//   - activatableClassId is an HSTRING handle (opaque). Never null when
//     called via the WinRT activation path.
//   - factory is a non-null pointer into which we write a raw
//     IActivationFactory* (as IntPtr). The CsWinRT-generated activation
//     registry returns this ABI pointer directly; do not re-marshal it
//     as a managed object.
//   - Return value is an HRESULT. S_OK == 0; any negative int is an
//     error code combase will propagate up to the caller.
//
// PARTNER ADAPTATION
//   To repurpose this bridge for a different assembly:
//     - Change the namespace / class name to suit; keep the method
//       signature byte-for-byte.
//     - Update the three HOST_BRIDGE_* macros in host.c to match the
//       new "Namespace.Type, AssemblyName" / "MethodName" strings.
//     - Keep the reflection lookup anchored on a type in your component
//       assembly. CsWinRT emits an internal WinRT.Module type per assembly;
//       that internal type owns the activation registry and returns raw
//       IActivationFactory ABI pointers. Do NOT bind to the public
//       WinRT.Module type from the CsWinRT runtime package.
//     - If your component is NOT a CsWinRT component, replace the lookup
//       body with your runtime's activation strategy (for example, a
//       hand-written switch on classId).
// ---------------------------------------------------------------------------

using System;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Tasks.NativeHost;

internal static unsafe class NativeHostBridge
{
    // ----------------------------------------------------------------
    // Stable HRESULTs we hand back. Centralising these keeps the body
    // of GetActivationFactory readable and lets us audit which paths
    // map to which combase-visible error.
    // ----------------------------------------------------------------
    private const int S_OK                      =  0;
    private const int E_POINTER                 = unchecked((int)0x80004003);
    private const int E_INVALIDARG              = unchecked((int)0x80070057);
    private const int REGDB_E_CLASSNOTREG       = unchecked((int)0x80040154);

    [DllImport("api-ms-win-core-winrt-string-l1-1-0.dll", CallingConvention = CallingConvention.StdCall)]
    private static extern char* WindowsGetStringRawBuffer(IntPtr hstring, uint* length);

    private static MethodInfo? s_getActivationFactory;

    // volatile is required: the fast path in ResolveGetActivationFactory()
    // reads s_initialized WITHOUT acquiring s_lock. Without volatile, a
    // reader that observes s_initialized == true is not guaranteed to also
    // see the s_getActivationFactory write the initializing thread performed
    // before flipping the flag, because the lock release inside the writer
    // only establishes happens-before with threads that subsequently acquire
    // the same lock. Declaring this field volatile makes the write a
    // release-store and every read an acquire-load, which IS the publication
    // barrier the fast path depends on (equivalent to std::atomic<bool>
    // with memory_order_acquire/release).
    private static volatile bool s_initialized;
    private static string? s_initError;

    // Guards the one-shot initialization of s_getActivationFactory.
    // The fast path (s_initialized == true) reads without taking the lock;
    // any thread that observes s_initialized == false acquires this lock,
    // re-checks under the lock, performs the reflection work, and only
    // then publishes s_getActivationFactory and sets s_initialized = true.
    // This makes the init both thread-safe and atomic from the caller's
    // perspective: every caller observes either "not yet attempted" or
    // "attempt complete, here is the (possibly null) result".
    private static readonly object s_lock = new();

    /// <summary>
    /// Resolves the activation factory for a WinRT class hosted by this
    /// assembly. Invoked exclusively by the native host shim.
    /// </summary>
    /// <param name="activatableClassId">
    /// HSTRING handle whose contents name a runtime class (for example
    /// "Tasks.PrintSupportWorkflowBackgroundTask").
    /// </param>
    /// <param name="factory">
    /// On success, receives an IActivationFactory* ABI pointer (as IntPtr).
    /// On failure, set to IntPtr.Zero before return.
    /// </param>
    /// <returns>HRESULT (0 on success).</returns>
    [UnmanagedCallersOnly]
    public static int GetActivationFactory(IntPtr activatableClassId, IntPtr* factory)
    {
        if (factory == null)
        {
            return E_POINTER;
        }

        // Always initialise the out-pointer before any operation that
        // could throw - guarantees the caller never sees a stale value
        // on an error return.
        *factory = IntPtr.Zero;

        if (activatableClassId == IntPtr.Zero)
        {
            return E_INVALIDARG;
        }

        try
        {
            // --------------------------------------------------------
            // STEP 1: HSTRING -> managed string.
            //
            // activatableClassId is an HSTRING handle, not a raw wchar_t*.
            // WindowsGetStringRawBuffer exposes the underlying UTF-16
            // buffer without transferring ownership. The caller (combase)
            // owns the HSTRING lifetime, so we MUST NOT call
            // WindowsDeleteString on it.
            // --------------------------------------------------------
            uint length;
            char* raw = WindowsGetStringRawBuffer(activatableClassId, &length);
            if (raw == null || length == 0)
            {
                return E_INVALIDARG;
            }

            string className = new(raw, 0, (int)length);

            // Breadcrumb proving the managed bridge was reached. Visible to
            // DbgView running against the BTH process.
            Debug.WriteLine($"[NativeHost.Bridge] GetActivationFactory: {className}");

            // --------------------------------------------------------
            // STEP 2: locate the CsWinRT-generated activation registry.
            //
            // CsWinRT emits an internal per-assembly WinRT.Module when
            // <CsWinRTComponent>true</CsWinRTComponent> is set. That
            // internal Module.GetActivationFactory(string) returns a raw
            // IActivationFactory ABI pointer (IntPtr) for classes declared
            // in THIS assembly.
            //
            // Reflection is intentional: direct binding to the CsWinRT
            // runtime's public Module type does not own
            // this assembly's generated activation table and returns the
            // wrong shape. Anchor the lookup on NativeHostBridge's assembly
            // so we bind to this component's generated activation registry.
            // --------------------------------------------------------
            MethodInfo? getActivationFactory = ResolveGetActivationFactory();
            if (getActivationFactory == null)
            {
                Debug.WriteLine($"[NativeHost.Bridge] No factory method available. {s_initError ?? "(unknown)"}");
                return REGDB_E_CLASSNOTREG;
            }

            object? result = getActivationFactory.Invoke(null, new object?[] { className });
            IntPtr factoryPtr = (IntPtr)(result ?? IntPtr.Zero);

            if (factoryPtr == IntPtr.Zero)
            {
                Debug.WriteLine($"[NativeHost.Bridge] Class not registered: {className}");
                return REGDB_E_CLASSNOTREG;
            }

            *factory = factoryPtr;
            Debug.WriteLine($"[NativeHost.Bridge] factory=0x{factoryPtr.ToInt64():X} for {className}");
            return S_OK;
        }
        catch (TargetInvocationException ex) when (ex.InnerException != null)
        {
            // CRITICAL: never let an exception cross the native boundary.
            // Reflection wraps generated activation failures in
            // TargetInvocationException; unwrap so combase receives the
            // HRESULT that the CsWinRT activation path actually produced.
            Debug.WriteLine(
                $"[NativeHost.Bridge] TargetInvocationException: {ex.InnerException.GetType().Name}: {ex.InnerException.Message}");
            return Marshal.GetHRForException(ex.InnerException);
        }
        catch (Exception ex)
        {
            // CRITICAL: never let an exception cross the native boundary.
            Debug.WriteLine($"[NativeHost.Bridge] Exception: {ex.GetType().Name}: {ex.Message}");
            return Marshal.GetHRForException(ex);
        }
    }

    private static MethodInfo? ResolveGetActivationFactory()
    {
        // Fast path: once init has completed (success or recorded failure)
        // we never touch the lock again. s_initialized is volatile, so this
        // read is an acquire-load that pairs with the release-store inside
        // the lock block below; that pairing guarantees any reader who sees
        // s_initialized == true also observes the s_getActivationFactory
        // value the initializing thread published just before flipping the
        // flag. Without volatile, this would be a torn-state hazard - the
        // lock release alone does NOT synchronize with a non-locking reader.
        if (s_initialized)
        {
            return s_getActivationFactory;
        }

        lock (s_lock)
        {
            // Re-check under the lock - a racing thread may have completed
            // init while we were waiting. The lock acquire is its own
            // memory barrier; the volatile read is still cheap and keeps
            // the read semantics uniform with the fast path above.
            if (s_initialized)
            {
                return s_getActivationFactory;
            }

            // Do the work BEFORE flipping s_initialized so a partially
            // initialized state is never visible to other threads.
            MethodInfo? resolved = null;

            Assembly thisAssembly = typeof(NativeHostBridge).Assembly;
            Type? moduleType = thisAssembly.GetType("WinRT.Module");
            if (moduleType == null)
            {
                s_initError = "WinRT.Module type not found in assembly (CsWinRT source generator may not have run)";
                Debug.WriteLine($"[NativeHost.Bridge] ERROR: {s_initError}");
            }
            else
            {
                resolved = moduleType.GetMethod(
                    "GetActivationFactory",
                    BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic,
                    null,
                    new[] { typeof(string) },
                    null);

                if (resolved == null)
                {
                    s_initError = "WinRT.Module.GetActivationFactory(string) overload not found";
                    Debug.WriteLine($"[NativeHost.Bridge] ERROR: {s_initError}");
                }
            }

            // Publish-then-mark. The plain assignment to
            // s_getActivationFactory happens first; the volatile write to
            // s_initialized that follows is a release-store. Any reader on
            // the fast path that observes s_initialized == true is doing
            // an acquire-load, which means they are guaranteed to also see
            // the s_getActivationFactory value written here. The order of
            // these two statements matters - do NOT swap them.
            s_getActivationFactory = resolved;
            s_initialized = true;
            return s_getActivationFactory;
        }
    }

    // ----------------------------------------------------------------
    // Reserved for future use: a runtime preload entry point that the
    // native shim could call from a known-safe context (not from
    // DllMain) to warm the runtime before the first real activation.
    // Not wired in today; documented here so the bridge contract has
    // a single home for any future additions.
    // ----------------------------------------------------------------
    [ModuleInitializer]
    internal static void ModuleInit()
    {
        // No-op. CsWinRT initialisation happens lazily on first
        // WinRT.Module.GetActivationFactory call. Kept as a hook so that
        // partners with custom initialisation (logging, ETW provider
        // registration, etc.) have an obvious place to add it.
    }
}
