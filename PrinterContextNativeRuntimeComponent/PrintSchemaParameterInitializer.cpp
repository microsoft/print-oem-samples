/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print schema parameter initializer
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/dn454557(v=vs.85).aspx
*/
#include "pch.h"

#include "PrintSchemaElement.h"
#include "PrintSchemaParameterInitializer.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrintSchemaParameterInitializer::PrintSchemaParameterInitializer(IPrintSchemaParameterInitializer* ptr)
            {
                m_ptr = ptr;
            }

            String^ PrintSchemaParameterInitializer::Xml::get()
            {
                ComPtr<IPrintSchemaElement> ptr;
                StringNonZeroCheck(m_ptr.As(&ptr));
                return PrintSchemaElement::GetXmlStreamForElement((ULONGLONG)ptr.Get());

            }

            String^ PrintSchemaParameterInitializer::Name::get()
            {
                BSTR n = NULL;
                StringNonZeroCheck(m_ptr->get_Name(&n));
                String^ s = ref new String(n);
                return s;
            }

            String^ PrintSchemaParameterInitializer::NamespaceUri::get()
            {
                BSTR n = NULL;
                StringNonZeroCheck(m_ptr->get_NamespaceUri(&n));
                String^ s = ref new String(n);
                return s;
            }

            Object^ PrintSchemaParameterInitializer::Value::get()
            {
                VARIANT vt;
                StringNonZeroCheck(m_ptr->get_Value(&vt));
                switch (vt.vt)
                {
                case VT_LPWSTR:
                case VT_BSTR:
                    return ref new String(vt.bstrVal);
                }
                return  (ULONGLONG)vt.ullVal;
            }
        }
    }
}

