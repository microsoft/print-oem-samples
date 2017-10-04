#pragma once
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

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a print queue events helper callback handler
            class PrinterQueueEventHelperCallback
                : public RuntimeClass<
                RuntimeClassFlags<RuntimeClassType::ClassicCom>,
                IPrinterQueueEvent, IDispatch>
            {
            private:
                ComPtr<IPrinterQueue> queue;
                Windows::Foundation::TypedEventHandler<Object^, PrinterQueueEventHelperEventArgs^>^ callback;
                ComPtr<IConnectionPoint> connectionPoint;
                DWORD connectionPointCookie;
                virtual HRESULT STDMETHODCALLTYPE OnBidiResponseReceived(BSTR bstrResponse, HRESULT hrStatus);
            public:
                PrinterQueueEventHelperCallback(IPrinterQueue* printerQueue, Windows::Foundation::TypedEventHandler<Object^, PrinterQueueEventHelperEventArgs^>^ func);
                ~PrinterQueueEventHelperCallback();
                // Inherited via RuntimeClass
                virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT * pctinfo) override;
                virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo) override;
                virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR * rgszNames, UINT cNames, LCID lcid, DISPID * rgDispId) override;
                virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS * pDispParams, VARIANT * pVarResult, EXCEPINFO * pExcepInfo, UINT * puArgErr) override;
            };
        }
    }
}

