#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print schema feature
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/hh451284(v=vs.85).aspx
*/

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a print schema feature
            public ref class PrintSchemaFeature sealed
            {
            private:
                ComPtr<IPrintSchemaFeature> m_ptr;
            internal:
                PrintSchemaFeature(IPrintSchemaFeature* ptr);
                property IPrintSchemaFeature* AsPtr { IPrintSchemaFeature* get(); }
            public:
                property String^ Xml { String^ get(); }
                property String^ Name { String^ get(); }
                property String^ NamespaceUri { String^ get(); }
                property String^ DisplayName { String^ get(); }
                ULONGLONG GetSelectedOption2();
                void PutSelectedOption2(ULONGLONG option);
                property PrintSchemaOption^ SelectedOption { PrintSchemaOption^ get(); void set(PrintSchemaOption^ value); }
                property int SelectionType { int get(); }
                ULONGLONG GetOption2(String^ name, String^ namespaceUri);
                property bool DisplayUI { bool get(); }
            };
        }
    }
}

