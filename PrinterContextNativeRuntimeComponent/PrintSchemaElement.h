#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print schema element
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/hh451270(v=vs.85).aspx
*/

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a print schema element
            public ref class PrintSchemaElement sealed
            {
            private:
                ComPtr<IPrintSchemaElement> m_ptr;
            internal:
                PrintSchemaElement(IPrintSchemaElement* ptr);
            public:
                property String^ Xml { String^ get(); }
                property String^ Name { String^ get(); }
                property String^ NamespaceUri { String^ get(); }
                static String^ GetXmlStreamForElement(ULONGLONG ptrIPrintSchemaElement);
            };
        }
    }
}

