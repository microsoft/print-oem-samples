/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print queue job event handler
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/dn265393(v=vs.85).aspx
*/
#include "pch.h"

#include "PrinterQueueViewEventHelperEventArgs.h"
#include "PrinterPrintJobEventHandler.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrinterPrintJobEventHandler::PrinterPrintJobEventHandler(int64 view, Windows::Foundation::TypedEventHandler<Object^, PrinterQueueViewEventHelperEventArgs^>^ func)
            {
                callback = func;
                ComPtr<IPrinterQueueView> queue;
                queue.Attach((IPrinterQueueView*)(void*)view);
                ComPtr<IConnectionPointContainer> cpc;
                __abi_ThrowIfFailed(queue.As(&cpc));
                __abi_ThrowIfFailed(cpc->FindConnectionPoint(__uuidof(IPrinterQueueViewEvent), &connectionPoint));
                __abi_ThrowIfFailed(connectionPoint->Advise((IUnknown*)(void*)(this), &connectionPointCookie));
            }

            PrinterPrintJobEventHandler::~PrinterPrintJobEventHandler()
            {
                if (connectionPoint != nullptr && connectionPoint.Get() != NULL)
                {
                    connectionPoint->Unadvise(connectionPointCookie);
                }

                connectionPoint = nullptr;
                view = nullptr;
            }

            HRESULT STDMETHODCALLTYPE  PrinterPrintJobEventHandler::OnChanged(IPrintJobCollection * collection, ULONG viewOffset, ULONG viewSize, ULONG countJobsInPrintQueue)
            {
                HRESULT hr = S_OK;
                try
                {
                    if (callback != nullptr)
                    {
                        Platform::Collections::Vector<int64>^ jobVector = ref new Platform::Collections::Vector<int64>();

                        ULONG countJobs;
                        __abi_ThrowIfFailed(
                            collection->get_Count(&countJobs));

                        for (ULONG i = 0; i < countJobs; i++)
                        {
                            ComPtr<IPrintJob> spJob;
                            __abi_ThrowIfFailed(
                                collection->GetAt(i, &spJob));
                            jobVector->Append((int64)(void*)reinterpret_cast<IUnknown*>(spJob.Get()));
                        }

                        callback(nullptr, ref new PrinterQueueViewEventHelperEventArgs((int64)view.Get(), jobVector, viewOffset, viewSize, countJobsInPrintQueue));
                    }
                }
                catch (...)
                {
                    hr = E_FAIL;
                }
                return hr;
            }

            HRESULT STDMETHODCALLTYPE  PrinterPrintJobEventHandler::Invoke(DISPID id, REFIID, LCID, WORD, DISPPARAMS * params, VARIANT *, EXCEPINFO *, UINT * wrongArgument)
            {
                HRESULT hr = S_OK;
                //TODO:Make sure that this won't throw exceptions
                try
                {
                    // Perform basic sanity checks
                    if ((params == nullptr) ||
                        (params->cArgs != 4))
                    {
                        throw Platform::Exception::CreateException(DISP_E_BADPARAMCOUNT);
                    }

                    if (id != DISPID_PRINTERQUEUEVIEW_EVENT_ONCHANGED)
                    {
                        throw Platform::Exception::CreateException(DISP_E_MEMBERNOTFOUND);
                    }

                    // Verify the contents of the VARIANT structure are in order.
                    VARIANT countJobs = params->rgvarg[0];
                    VARIANT viewSize = params->rgvarg[1];
                    VARIANT viewOffset = params->rgvarg[2];
                    VARIANT collection = params->rgvarg[3];

                    if (countJobs.vt != VT_UI4)
                    {
                        if (wrongArgument) *wrongArgument = 0;
                        throw Platform::Exception::CreateException(DISP_E_TYPEMISMATCH);
                    }
                    else if (viewSize.vt != VT_UI4)
                    {
                        if (wrongArgument) *wrongArgument = 1;
                        throw Platform::Exception::CreateException(DISP_E_TYPEMISMATCH);
                    }
                    else if (viewOffset.vt != VT_UI4)
                    {
                        if (wrongArgument) *wrongArgument = 2;
                        throw Platform::Exception::CreateException(DISP_E_TYPEMISMATCH);
                    }
                    else if (collection.vt != VT_DISPATCH)
                    {
                        if (wrongArgument) *wrongArgument = 3;
                        throw Platform::Exception::CreateException(DISP_E_TYPEMISMATCH);
                    }

                    IPrintJobCollection* pc = reinterpret_cast<IPrintJobCollection*>(collection.pdispVal);
                    if (pc == nullptr)
                    {
                        *wrongArgument = 3;
                        throw Platform::Exception::CreateException(DISP_E_TYPEMISMATCH);
                    }

                    // Invoke the event handler method to handle the queue enumeration.
                    __abi_ThrowIfFailed(
                        OnChanged(pc, viewOffset.ulVal, viewSize.ulVal, countJobs.ulVal));
                }
                catch (Exception^ exception)
                {
                    hr = exception->HResult;
                }

                return hr;
            }

            HRESULT STDMETHODCALLTYPE  PrinterPrintJobEventHandler::GetTypeInfoCount(__RPC__out UINT *)
            {
                return E_NOTIMPL;
            }

            HRESULT STDMETHODCALLTYPE  PrinterPrintJobEventHandler::GetTypeInfo(UINT, LCID, __RPC__deref_out_opt ITypeInfo **)
            {
                return E_NOTIMPL;
            }

            HRESULT STDMETHODCALLTYPE  PrinterPrintJobEventHandler::GetIDsOfNames(__RPC__in REFIID, __RPC__in_ecount_full(cNames) LPOLESTR *,
                                                                                  __RPC__in_range(0, 16384) UINT cNames, LCID, __RPC__out_ecount_full(cNames) DISPID *)
            {
                return E_NOTIMPL;
            }
        }
    }
}

