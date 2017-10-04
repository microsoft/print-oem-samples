/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print schema asynchronous callback operation callback
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/hh451224(v=vs.85).aspx
*/
#include "pch.h"

#include <PrinterExtensionDispId.h>
#include "PrintSchemaAsyncOperationCallback.h"
#include "PrintSchemaAsyncOp.h"

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrintSchemaAsyncOperationCallback::PrintSchemaAsyncOperationCallback(IPrintSchemaAsyncOperation* op, Object^ callback)
            {
                m_printerAsyncOperationCallbackHandler = callback;
                m_printSchemaAsyncOperation = op;
                // IID_IPrintSchemaAsyncOperationEvent 23adbb16-0133-4906-b29a-1dce1d026379
                GUID guidIPrintSchemaAsyncOperationEvent = { 0x23adbb16, 0x0133, 0x4906,{ 0xb2, 0x9a, 0x1d, 0xce, 0x1d, 0x02, 0x63, 0x79 } };
                ComPtr<IConnectionPointContainer> container;
                ThrowIf(m_printSchemaAsyncOperation.As(&container));
                ThrowIf(container->FindConnectionPoint(guidIPrintSchemaAsyncOperationEvent, m_connectionPoint.GetAddressOf()));
                ThrowIf(m_connectionPoint->Advise((IUnknown*)(void*)(this), &m_connectionPointCookie));
                op->Start();
            }

            PrintSchemaAsyncOperationCallback::~PrintSchemaAsyncOperationCallback()
            {

                if (m_connectionPoint != nullptr && m_connectionPoint.Get() != NULL)
                {
                    m_connectionPoint->Unadvise(m_connectionPointCookie);
                }
                m_connectionPoint = nullptr;
            }

            // Inherited via RuntimeClass
            HRESULT STDMETHODCALLTYPE PrintSchemaAsyncOperationCallback::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS * pDispParams, VARIANT * pVarResult, EXCEPINFO * pExcepInfo, UINT * puArgErr)
            {
                if (dispIdMember == DISPID_PRINTSCHEMA_ASYNCOPERATIONEVENT_COMPLETED)
                {
                    if (pDispParams)
                    {
                        if (pDispParams->cArgs == 2)
                        {
                            return Completed(NULL, pDispParams->rgvarg[0].intVal);
                        }
                    }
                }
                return E_INVALIDARG;
            }

            HRESULT PrintSchemaAsyncOperationCallback::Completed(IPrintSchemaTicket * pTicket, HRESULT hrOperation)
            {
                if (m_printerAsyncOperationCallbackHandler != nullptr)
                {
                    ((PrintSchemaAsyncOp^)m_printerAsyncOperationCallbackHandler)->ManagedPPrintSchemaAsyncOperationCallback((ULONGLONG)pTicket, hrOperation);
                }
                return S_OK;
            }
        }
    }
}
