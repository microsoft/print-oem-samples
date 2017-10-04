#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print queue view
*   as described in https://msdn.microsoft.com/library/windows/hardware/dn265392
*/

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a print queue view
            public ref class PrinterQueueView sealed
            {
            private:
                ComPtr<IPrinterQueueView> m_ptr;
                PrinterPrintQueueEventHandlerHelper^ m_handler;
                Windows::Foundation::EventRegistrationToken token;
                ~PrinterQueueView();
            internal:
                PrinterQueueView(IPrinterQueueView* view);
            public:
                void OnOnChange(Platform::Object ^sender, PrinterQueueViewEventHelperEventArgs ^args);
                event Windows::Foundation::EventHandler<PrinterQueueViewEventArgs^>^ OnChange;
                void SetViewRange(int start, int length);
            };
        }
    }
}

