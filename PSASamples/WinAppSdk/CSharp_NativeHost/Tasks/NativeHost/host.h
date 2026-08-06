// ---------------------------------------------------------------------------
// host.h - Public surface for the PSA native host shim.
//
// The DLL exposes exactly one Windows-callable entry point: DllGetActivationFactory.
// That symbol is published via host.def, NOT via __declspec(dllexport), so that
// the exported name is identical across x64 / x86 / ARM64 with no decoration
// surprises. Consequently this header is intentionally minimal - it exists so
// host.c compiles clean under /W4 and so there is a single place to add small
// in-DLL helper declarations if we ever need them (e.g. a test-only preload
// entry point). External callers do NOT need to consume this header.
// ---------------------------------------------------------------------------
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <inspectable.h>     // IInspectable
#include <activation.h>      // IActivationFactory

// Last-error stash. Set by InitializeRuntime on any failure path during
// one-time CoreCLR bootstrap so that callers (and live debuggers) can recover
// the real HRESULT from hostfxr / coreclr rather than just "E_FAIL".
//
// Defined in host.c. Declared here so a future diagnostics export could read
// it without exposing the entire .c implementation.
extern volatile LONG g_init_hresult;
