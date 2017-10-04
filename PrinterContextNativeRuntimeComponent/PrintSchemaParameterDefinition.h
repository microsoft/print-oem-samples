#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print schema parameter definition
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/dn465890(v=vs.85).aspx
*/

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a print schema parameter definition
            public ref class PrintSchemaParameterDefinition sealed
            {
            private:
                ComPtr<IPrintSchemaParameterDefinition> m_ptr;
            internal:
                PrintSchemaParameterDefinition(IPrintSchemaParameterDefinition* ptr);
            public:
                property String^ Xml { String^ get(); }
                property String^ Name { String^ get(); }
                property String^ NamespaceUri { String^ get(); }
                property bool UserInputRequired { bool get(); }
                property String^ UnitType { String^ get(); }
                property int DataType { int get(); }
                property int RangeMin { int get(); }
                property int RangeMax { int get(); }
            };
        }
    }
}

