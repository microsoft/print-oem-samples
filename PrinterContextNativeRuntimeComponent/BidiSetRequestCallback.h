#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class handles calling the BiDi request callback handlers that have been added, if any
*   as described in https://msdn.microsoft.com/library/windows/hardware/dn265385
*/

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents the callback object that is invoked when a Bidi "Set" operation is completed.
            class BidiSetRequestCallback
                : public RuntimeClass<
                RuntimeClassFlags<RuntimeClassType::ClassicCom>,
                IPrinterBidiSetRequestCallback>
            {
            public:
                BidiSetRequestCallback(Windows::Foundation::TypedEventHandler<Object^, PrinterQueueEventHelperEventArgs^>^ func);
                ~BidiSetRequestCallback();
            private:
                virtual HRESULT STDMETHODCALLTYPE Completed(_In_ BSTR bstrResponse, HRESULT hrStatus);
                Windows::Foundation::TypedEventHandler<Object^, PrinterQueueEventHelperEventArgs^>^ callback;
            };
        }
    }
}

