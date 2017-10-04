#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a printer queue v2 interface
*   as described in https://msdn.microsoft.com/library/windows/hardware/dn265389
*/

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a print queue 2
            public ref class PrinterQueue2 sealed
            {
            private:
                ComPtr<IPrinterQueue2> m_ptr;
                PrinterQueueEventHelper^ _sink;
                Windows::Foundation::EventRegistrationToken _handler;
                ~PrinterQueue2();
            public:
                PrinterQueue2(ULONGLONG ptrIPrinterQueue2);
                property String^ Name { String^ get(); }
                void SendBidiQuery(String^ bidiQuery);
                property ULONGLONG Handle { ULONGLONG get(); }
                void SendBidiSetRequestAsyncNoTask(String^ bidiRequest);
                IAsyncOperation<PrinterQueueEventHelperEventArgs^>^ SendBidiSetRequestAsync(String^ bidiRequest);
                PrinterPropertyBag^ GetProperties();
                PrinterQueueView^ GetPrinterQueueView(int viewOffset, int viewSize);
                event Windows::Foundation::EventHandler<PrinterQueueEventHelperEventArgs^>^ OnBidiResponseReceivedEvent;
                void OnOnPrinterQueueEvent(Platform::Object ^helper, PrinterQueueEventHelperEventArgs ^args);
            };
        }
    }
}

