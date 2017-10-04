#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a WSDA printer extension context
*   as described in https://msdn.microsoft.com/library/windows/hardware/hh406649
*/

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a WSDA printer extension context
            public ref class PrinterExtensionContext sealed
            {
            private:
                ComPtr<IPrinterExtensionContext> m_context;
                ComPtr<IPrinterQueue> m_printerQueue;
                ComPtr<IPrintSchemaTicket> m_ticket;
                ~PrinterExtensionContext() {}
            public:
                //Inherently unsafe
                PrinterExtensionContext(ULONGLONG context);
                property PrinterQueue2^ Queue { PrinterQueue2^ get(); }
                property PrintSchemaTicket^ Ticket { PrintSchemaTicket^ get(); }
                property PrinterPropertyBag^ UserProperties { PrinterPropertyBag^ get(); }
                property PrinterPropertyBag^ DriverProperties { PrinterPropertyBag^ get(); }
                property ULONGLONG UserProperties2 { ULONGLONG get(); }
                property ULONGLONG DriverProperties2 { ULONGLONG get(); }
                property ULONGLONG Context { ULONGLONG get(); }
            };

        }
    }
}

