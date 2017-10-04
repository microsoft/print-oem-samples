#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print N-Up option
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/hh451302(v=vs.85).aspx
*/

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents print N-Up option
            public ref class PrintSchemaNUpOption sealed
            {
            private:
                ComPtr<IPrintSchemaNUpOption> m_option;
                const bool FreeStrings = true;
            internal:
                PrintSchemaNUpOption(IPrintSchemaNUpOption* option, void* notused);
            public:
                //Inherently unsafe
                PrintSchemaNUpOption(ULONGLONG option);
                property int PagesPerSheet { int get(); }
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

