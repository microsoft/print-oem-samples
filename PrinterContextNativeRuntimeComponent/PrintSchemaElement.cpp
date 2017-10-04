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
#include "pch.h"

#include "PrintSchemaElement.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrintSchemaElement::PrintSchemaElement(IPrintSchemaElement* ptr)
            {
                m_ptr = ptr;
            }

            String^ PrintSchemaElement::Xml::get()
            {
                return GetXmlStreamForElement((ULONGLONG)m_ptr.Get());

            }

            String^ PrintSchemaElement::Name::get()
            {
                BSTR n = NULL;
                StringNonZeroCheck(m_ptr->get_Name(&n));
                String^ s = ref new String(n);
                return s;
            }

            String^ PrintSchemaElement::NamespaceUri::get()
            {
                BSTR n = NULL;
                StringNonZeroCheck(m_ptr->get_NamespaceUri(&n));
                String^ s = ref new String(n);
                return s;
            }

            String^ PrintSchemaElement::GetXmlStreamForElement(ULONGLONG ptrIPrintSchemaElement)
            {
                ComPtr<IPrintSchemaElement> cp = (IPrintSchemaElement*)ptrIPrintSchemaElement;
                ComPtr<IUnknown> i;
                StringNonZeroCheck(cp->get_XmlNode(i.GetAddressOf()));
                ComPtr<IXMLDOMDocument2> doc;
                StringNonZeroCheck(i.As(&doc));
                BSTR bstr;
                StringNonZeroCheck(doc->get_xml(&bstr));
                String^ ret = ref new String(bstr);
                return ret;
            }
        }
    }
}

