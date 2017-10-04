#pragma once

#include <collection.h>
#include <ppltasks.h>
#include <xpsobjectmodel.h>
#include <xpsobjectmodel_1.h>
#pragma push_macro("WINAPI_PARTITION_DESKTOP")
#undef WINAPI_PARTITION_DESKTOP
#define WINAPI_PARTITION_DESKTOP 1
#include <wininet.h>   // For INTERNET_MAX_PATH_LENGTH
#include <cfgmgr32.h>  // For MAX_GUID_STRING_LEN
#pragma pop_macro("WINAPI_PARTITION_DESKTOP")
#include "PrinterExtension.h"
#include <windows.graphics.printing.workflow.native.h>

using namespace Microsoft::WRL;
using namespace Platform;
using namespace std;
using namespace Windows::Storage::Streams;
using namespace Windows::Graphics::Printing::Workflow;

// Define some error macros - the WRT component cannot throw because it is not running in the app thread
#define RETURN_INVALID_ARGUMENT_IF_NULL(x) do {bool argumentIsNull = ((x)==nullptr); if (argumentIsNull) {return E_INVALIDARG;} else {break;}} while (0, 0)
#define RETURN_HR_IF_FAILED(hr) do {HRESULT __hrRet = (hr); if (FAILED(__hrRet)) { return __hrRet;}} while (0, 0)
#define THROW_IF_FAILED(hr) do { HRESULT __hrRet = (hr); if (FAILED(__hrRet)) { throw ref new Platform::Exception(__hrRet); } } while (0, 0)

