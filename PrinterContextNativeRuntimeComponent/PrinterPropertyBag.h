#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a printer property bag
*   as described in https://msdn.microsoft.com/library/windows/hardware/hh439547
*/

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a printer property bag
            public ref class PrinterPropertyBag sealed
            {
            private:
                ComPtr<IPrinterPropertyBag> m_printerPropertyBag;
                int m_printerPropertyBagType;
            internal:
                PrinterPropertyBag(IPrinterPropertyBag* ptr, int type);
            public:
                PrinterProperty^ GetProperty(String^ name);
                String^ GetString(String^ name);
                int GetInt(String^ name);
                bool GetBool(String^ name);
                void SetString(String^ name, String^ value);
                void SetBool(String^ name, bool value);
            };
        }
    }
}
