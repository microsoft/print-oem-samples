/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print schema displayable element
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/hh451262(v=vs.85).aspx
*/
#include "pch.h"

#include "PrintSchemaElement.h"
#include "PrintSchemaDisplayableElement.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrintSchemaDisplayableElement::PrintSchemaDisplayableElement(IPrintSchemaDisplayableElement* ptr)
            {
                m_ptr = ptr;
            }

            String^ PrintSchemaDisplayableElement::Xml::get()
            {
                ComPtr<IPrintSchemaElement> ip;
                StringNonZeroCheck(m_ptr.As(&ip));
                return PrintSchemaElement::GetXmlStreamForElement((ULONGLONG)ip.Get());
            }

            String^ PrintSchemaDisplayableElement::Name::get()
            {
                BSTR n = NULL;
                StringNonZeroCheck(m_ptr->get_Name(&n));
                String^ s = ref new String(n);
                return s;
            }

            String^ PrintSchemaDisplayableElement::NamespaceUri::get()
            {
                BSTR n = NULL;
                StringNonZeroCheck(m_ptr->get_NamespaceUri(&n));
                String^ s = ref new String(n);
                return s;
            }

            String^ PrintSchemaDisplayableElement::DisplayName::get()
            {
                BSTR n = NULL;
                StringNonZeroCheck(m_ptr->get_DisplayName(&n));
                String^ s = ref new String(n);
                return s;
            }
        }
    }
}

