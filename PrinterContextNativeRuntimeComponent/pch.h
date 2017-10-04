#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   ***WARNING*** This sample code is a pre-release version, not for public release,
*   and is intended to unblock OEM partners writing WSDA/Workflow apps in UWP.
*   It is subject to change without warning and should NOT be used in production code.
*/

// Template Default Includes
#include <collection.h>
#include <ppltasks.h>

// Enable the desktop API partition to get the XML functionality
#pragma push_macro("WINAPI_PARTITION_DESKTOP")
#undef WINAPI_PARTITION_DESKTOP
#define WINAPI_PARTITION_DESKTOP 1
#pragma push_macro("__msxml_h__")
#undef __msxml_h__
#include <msxml6.h>
#pragma pop_macro("__msxml_h__")
#pragma pop_macro("WINAPI_PARTITION_DESKTOP")

// CPP Includes
#include <errno.h>
#include <iostream> 
#include <OCIdl.h>
#include <sstream> 
#include <strsafe.h>
#include <stdlib.h>
#include <stdio.h>

// Windows Runtime Library Includes
#include <wrl\implements.h>

// Printer Extension and Workflow Includes
#include <PrinterExtension.h>
#include <PrinterExtensionDispId.h>
#include <Windows.Devices.Printers.Extensions.h>
#include "Windows.Graphics.Printing.Workflow.Native.h"

// Print Context Native Runtime Component includes
#include "AutoBSTR.h"

// Namespaces
using namespace Concurrency;
using namespace Microsoft::WRL;
using namespace Platform;

// Macros
// Throw on HRESULTS that aren't S_OK
#define ThrowIf(x) ThrowIf2(x)
void FORCEINLINE ThrowIf2(HRESULT hr)
{
    __abi_ThrowIfFailed(hr);
    if (hr != S_OK)
    {
        wchar_t buffer[255];
        StringCbPrintf(buffer, 255, L"ThrowIf2 failed with an unexpected hr:\t%x\r\n", hr);
        OutputDebugStringW(buffer);
        __abi_WinRTraiseCOMException(E_UNEXPECTED);
    }
}

// This checks for non S_OK values; sets the last error code and returns nullptr if applicable
#define StringNonZeroCheck(x) if(x != S_OK) { SetLastError(x); return nullptr; }