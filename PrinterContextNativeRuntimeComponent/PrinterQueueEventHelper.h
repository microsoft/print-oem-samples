/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print queue event helper
*   as described in https://msdn.microsoft.com/library/windows/hardware/dn265393
*/
#pragma once

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class is the printer queue event helper
            public delegate void PrintQueueEventHelperHandler(Object^ helper, PrinterQueueEventHelperEventArgs^ args);

            public ref class PrinterQueueEventHelper sealed
            {
            private:
                ComPtr<IPrinterQueue> queue;
                ComPtr<PrinterQueueEventHelperCallback> helperHelper;
                void PrinterQueueEventHelperResult(Object^ helper, PrinterQueueEventHelperEventArgs^ args);
                ~PrinterQueueEventHelper();
            internal:
                PrinterQueueEventHelper(IPrinterQueue* printerQueue);
            public:
                PrinterQueueEventHelper(Object^ printerQueue, Object^ notUsed);
                event PrintQueueEventHelperHandler^ OnPrinterQueueEvent;
                Windows::Foundation::EventRegistrationToken AddHandler(PrintQueueEventHelperHandler^ handler);
                void RemoveHandler(Windows::Foundation::EventRegistrationToken handler);
            };
        }
    }
}
