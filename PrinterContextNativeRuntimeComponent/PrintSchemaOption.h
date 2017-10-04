#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print schema option
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/hh451335(v=vs.85).aspx
*/

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents print schema option
            public ref class PrintSchemaOption sealed
            {
            private:
                ComPtr<IPrintSchemaOption> m_option;
                const bool FreeStrings = true;
            internal:
                PrintSchemaOption(IPrintSchemaOption* option, void* notused);
            public:
                //Inherently unsafe
                PrintSchemaOption(ULONGLONG option);
                property Boolean Selected { Boolean get(); }
                property int Constrained { int get(); }
                property String^ DisplayName { String^ get(); }
                property String^ Name { String^ get(); }
                property String^ XmlNamespace { String^ get(); }
                property ULONGLONG Option { ULONGLONG get(); }
            };
        }
    }
}

