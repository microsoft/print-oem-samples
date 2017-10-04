/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print queue event helper callback handler
*   as described in https://msdn.microsoft.com/library/windows/hardware/dn265393
*/
#include "pch.h"

#include "PrinterQueueEventHelperEventArgs.h"
#include "PrinterQueueEventHelperCallback.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrinterQueueEventHelperCallback::PrinterQueueEventHelperCallback(IPrinterQueue * printerQueue, Windows::Foundation::TypedEventHandler<Object^, PrinterQueueEventHelperEventArgs^>^ func)
            {
                queue = printerQueue;
                callback = func;
                connectionPointCookie = 0;
                ComPtr<IConnectionPointContainer> cpc;
                __abi_ThrowIfFailed(queue.As(&cpc));
                __abi_ThrowIfFailed(cpc->FindConnectionPoint(__uuidof(IPrinterQueueEvent), &connectionPoint));
                __abi_ThrowIfFailed(connectionPoint->Advise((IUnknown*)(void*)(this), &connectionPointCookie));
            }

            PrinterQueueEventHelperCallback::~PrinterQueueEventHelperCallback()
            {
                queue = nullptr;
                if (connectionPoint != nullptr && connectionPoint.Get() != NULL)
                {
                    connectionPoint->Unadvise(connectionPointCookie);
                }

                connectionPoint = nullptr;
            }

            HRESULT PrinterQueueEventHelperCallback::OnBidiResponseReceived(BSTR bstrResponse, HRESULT hrStatus)
            {
                HRESULT hr = S_OK;
                try
                {
                    if (callback != nullptr)
                    {
                        String^ str = ref new String(bstrResponse);

                        PrinterQueueEventHelperEventArgs^ args = ref new PrinterQueueEventHelperEventArgs((Object^)(IntPtr)queue.Get(), str, hrStatus);
                        callback(nullptr, args);
                    }
                }
                catch (...)
                {
                    hr = E_FAIL;
                }

                return hr;
            }

            HRESULT PrinterQueueEventHelperCallback::GetTypeInfoCount(UINT * pctinfo)
            {
                if (pctinfo)
                {
                    *pctinfo = 1;
                    return S_OK;
                }

                return E_INVALIDARG;
            }

            HRESULT PrinterQueueEventHelperCallback::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo)
            {
                if (iTInfo == 1 && (ppTInfo != NULL))
                {
                    return E_NOTIMPL;
                }

                return E_INVALIDARG;
            }

            HRESULT PrinterQueueEventHelperCallback::GetIDsOfNames(REFIID riid, LPOLESTR * rgszNames, UINT cNames, LCID lcid, DISPID * rgDispId)
            {
                if (riid == __uuidof(IPrinterQueueEvent))
                {
                    if (cNames == 1)
                    {
                        if (rgszNames && rgDispId)
                        {
                            //This ignores the locale id
                            if (CompareStringOrdinal(rgszNames[0], -1, L"OnBidiResponseReceived", -1, TRUE) == 0)
                            {
                                rgDispId[0] = DISPID_PRINTERQUEUEEVENT_ONBIDIRESPONSERECEIVED;
                                return S_OK;
                            }
                        }
                    }
                }

                return E_INVALIDARG;
            }

            HRESULT PrinterQueueEventHelperCallback::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS * pDispParams, VARIANT * pVarResult, EXCEPINFO * pExcepInfo, UINT * puArgErr)
            {
                if (dispIdMember == DISPID_PRINTERQUEUEEVENT_ONBIDIRESPONSERECEIVED)
                {
                    if (pDispParams)
                    {
                        if (pDispParams->cArgs == 2)
                        {
                            switch (pDispParams->rgvarg[0].vt)
                            {
                            case VT_ERROR:
                                break;
                            default:
                                return E_INVALIDARG;
                            }
                            if (pDispParams->rgvarg[1].vt == VT_BSTR)
                            {
                                return OnBidiResponseReceived(pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].scode);
                            }
                        }
                    }
                }

                return E_INVALIDARG;
            }
        }
    }
}

