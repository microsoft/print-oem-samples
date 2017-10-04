#pragma once
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

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a print schema asynchronous callback handler
            typedef void (STDMETHODCALLTYPE *PPrintSchemaAsyncOperationCallbackHandler)(IPrintSchemaTicket* ticket, HRESULT hr);
            class PrintSchemaAsyncOperationCallback : public RuntimeClass<
                RuntimeClassFlags<RuntimeClassType::ClassicCom>,
                IPrintSchemaAsyncOperationEvent, IDispatch>
            {
            private:
                Object^ m_printerAsyncOperationCallbackHandler;
                ComPtr<IConnectionPoint> m_connectionPoint;
                DWORD m_connectionPointCookie;
                ComPtr<IPrintSchemaAsyncOperation> m_printSchemaAsyncOperation;
            public:
                PrintSchemaAsyncOperationCallback(IPrintSchemaAsyncOperation* op, Object^ callback);
                ~PrintSchemaAsyncOperationCallback();
                virtual HRESULT STDMETHODCALLTYPE Completed(IPrintSchemaTicket * pTicket, HRESULT hrOperation) override;
                // Inherited via RuntimeClass
                virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT * pctinfo) override { if (pctinfo) *pctinfo = 0; return E_NOTIMPL; }
                virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo) override { return E_NOTIMPL; }
                virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR * rgszNames, UINT cNames, LCID lcid, DISPID * rgDispId) override { return E_NOTIMPL; }
                virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS * pDispParams, VARIANT * pVarResult, EXCEPINFO * pExcepInfo, UINT * puArgErr) override;
            };
        }
    }
}
