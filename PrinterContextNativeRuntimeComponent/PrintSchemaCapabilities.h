#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents capabilities of a printer according to the IXMLDOMDocument schema
*   as described in https://msdn.microsoft.com/library/windows/hardware/hh451256
*/

// Namespaces
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents the capabilities of a printer according to a schema
            public ref class PrintSchemaCapabilities sealed
            {
            private:
                ComPtr<IPrintSchemaCapabilities> m_capabilities;
                ComPtr<IPrintSchemaCapabilities2> m_capabilities2;
            internal:
                PrintSchemaCapabilities(IPrintSchemaCapabilities* caps);
            public:
                PrintSchemaCapabilities(ULONGLONG caps);
                ULONGLONG GetFeatureByKeyName2(String^ keyName);
                PrintSchemaFeature^ GetFeatureByKeyName(String^ name);
                ULONGLONG GetFeature2(String^ featureName, String^ xmlNamespace);
                PrintSchemaFeature^ GetFeature(String^ featureName, String^ xmlNamespace);
                ULONGLONG GetFeature2(String^ name);
                PrintSchemaFeature^ GetFeature(String^ name);
                property ULONGLONG PageImageableSize2 { ULONGLONG get(); }
                property PrintSchemaPageImageableSize^ PageImageableSize { PrintSchemaPageImageableSize^ get(); }
                property unsigned int JobCopiesAllDocumentsMaxValue { unsigned int get(); }
                property unsigned int JobCopiesAllDocumentsMinValue { unsigned int get(); }
                //Could be one of several types - feature is IPrintSchemaFeature*
                Object^ GetSelectedOptionInPrintTicket2(ULONGLONG feature);
                IIterable<PrintSchemaOption^>^ GetOptions(PrintSchemaFeature^ feature);
                ULONGLONG GetParameterDefinition2(String^ parameterName, String^ xmlNamespace);
                PrintSchemaParameterDefinition^ GetParameterDefinition(String^ parameterName, String^ xmlNamespace);
                property String^ XmlStream { String^ get(); }
                property ULONGLONG Capabilities { ULONGLONG get(); }
                property ULONGLONG Capabilities2 { ULONGLONG get(); }
            };
        }
    }
}

