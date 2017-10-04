#pragma once
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

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a print queue job event handler
            class PrinterPrintJobEventHandler : public Microsoft::WRL::RuntimeClass<
                Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::ClassicCom>,
                IPrinterQueueViewEvent,
                IDispatch>
            {
            private:
                ComPtr<IPrinterQueueView> view;
                ComPtr<IConnectionPoint> connectionPoint;
                DWORD connectionPointCookie;
                Windows::Foundation::TypedEventHandler<Object^, PrinterQueueViewEventHelperEventArgs^>^ callback;
            public:
                PrinterPrintJobEventHandler(int64 view, Windows::Foundation::TypedEventHandler<Object^, PrinterQueueViewEventHelperEventArgs^>^ func);
                ~PrinterPrintJobEventHandler();
                HRESULT STDMETHODCALLTYPE OnChanged(IPrintJobCollection * collection, ULONG viewOffset, ULONG viewSize, ULONG countJobsInPrintQueue);
                virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID id, REFIID, LCID, WORD, DISPPARAMS * params, VARIANT *, EXCEPINFO *, UINT * wrongArgument);
                virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(__RPC__out UINT *);
                virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT, LCID, __RPC__deref_out_opt ITypeInfo **);
                virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(__RPC__in REFIID, __RPC__in_ecount_full(cNames) LPOLESTR*, __RPC__in_range(0,16384) UINT cNames, LCID, __RPC__out_ecount_full(cNames) DISPID*);
            };
        }
    }
}

