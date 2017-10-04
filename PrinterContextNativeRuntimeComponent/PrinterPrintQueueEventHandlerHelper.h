#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print queue event handler helper
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
            // This class represents a print queue event handler helper
            public ref class PrinterPrintQueueEventHandlerHelper sealed
            {
            private:
                ComPtr<IPrinterQueueView> view;
                ComPtr<PrinterPrintJobEventHandler> handler;
                ~PrinterPrintQueueEventHandlerHelper();
            public:
                PrinterPrintQueueEventHandlerHelper(int64 iPrintQueueView, Object^ notUsed);
                event Windows::Foundation::TypedEventHandler<Object^, PrinterQueueViewEventHelperEventArgs^>^ OnChange;
                void _onChange(Object^ notUsed, PrinterQueueViewEventHelperEventArgs^ args);
            };
        }
    }
}

