#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class allows a bidisetrequest to be done as a task - one instance is required per task
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/dn265391(v=vs.85).aspx
*/

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class allows a bidisetrequest to be done as a task - one instance is required per task
            public ref class PrinterQueueBidiSetRequestTask sealed
            {
            private:
                ComPtr<IPrinterQueue2> m_ptr;
                HANDLE hEvent;
                PrinterQueueEventHelperEventArgs^ m_lastargs;
                ~PrinterQueueBidiSetRequestTask();
            internal:
                PrinterQueueBidiSetRequestTask(IPrinterQueue2* queue);
            public:
                IAsyncOperation<PrinterQueueEventHelperEventArgs^>^ SendBidiRequest(String^ bidiRequest);
                property PrinterQueueEventHelperEventArgs^ LastArgs { PrinterQueueEventHelperEventArgs^ get(); }
                void OnOnPrinterQueueEvent(Platform::Object ^helper, PrinterQueueEventHelperEventArgs^ args);
            };
        }
    }
}

