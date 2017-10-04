#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a printer queue v1
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/hh439635
*/

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a print queue
            public ref class PrintQueue sealed
            {
            private:
                ComPtr<IPrinterQueue> m_printerQueue;
            internal:
                PrintQueue(IPrinterQueue* queue);
            public:
                property ULONGLONG Queue { ULONGLONG get(); }
            };
        }
    }
}

