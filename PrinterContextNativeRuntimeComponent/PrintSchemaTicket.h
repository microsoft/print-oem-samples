#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print schema print ticket
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/hh451398(v=vs.85).aspx
*/

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a print ticket
            public ref class PrintSchemaTicket sealed
            {
            private:
                ComPtr<IPrintSchemaTicket> m_ticket;
            internal:
                PrintSchemaTicket(IPrintSchemaTicket* ticket);
            public:
                //Inherently unsafe
                PrintSchemaTicket(ULONGLONG ptrTicket);
                property ULONGLONG Ticket { ULONGLONG get(); }
                ULONGLONG GetCapabilities2();
                ULONGLONG GetFeature2(String^ name, String^ xmlNamespace);
                PrintSchemaFeature^ GetFeature(String^ name, String^ xmlNamespace);
                ULONGLONG GetFeature2(String^ name);
                PrintSchemaFeature^ GetFeature(String^ name);
                ULONGLONG GetFeatureByKeyname2(String^ name);
                PrintSchemaFeature^ GetFeatureByKeyName(String^ name);
                ULONGLONG GetParameterInitializer2(String^ name, String^ xmlNamespace);
                ULONGLONG GetParameterInitializer2(String^ name);
                PrintSchemaParameterInitializer^ GetParameterInitializer(String^ name, String^ xmlNamespace);
                PrintSchemaParameterInitializer^ GetParameterInitializer(String^ name);
                PrintSchemaAsyncOp^ ValidateAsync2();
                //Inherently unsafe
                PrintSchemaAsyncOp^ CommitAsync2(ULONGLONG IPrintSchemaTicketAddr);
                PrintSchemaAsyncOp^ ValidateAndCommitPrintTicketXML(String^ xml);
                void NotifyXmlChanged();
                property int JobCopiesAllDocuments { int get(); void set(int val); }
                property String^ XmlStream { String^ get(); internal: void set(String^ xml); }
                static String^ GetXmlStreamForCapabilities(ULONGLONG ptrIPrintSchemaCapabilities);
                static String^ GetXmlStreamForElement(ULONGLONG ptrIPrintSchemaElement);
            };
        }
    }
}

