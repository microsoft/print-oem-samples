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
#include "pch.h"

#include "PrintSchemaElement.h"
#include "PrintSchemaParameterDefinition.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrintSchemaParameterDefinition::PrintSchemaParameterDefinition(IPrintSchemaParameterDefinition* ptr)
            {
                m_ptr = ptr;
            }

            String^ PrintSchemaParameterDefinition::Xml::get()
            {
                ComPtr<IPrintSchemaElement> ptr;
                ThrowIf(m_ptr.As(&ptr));
                return PrintSchemaElement::GetXmlStreamForElement((ULONGLONG)ptr.Get());

            }

            String^ PrintSchemaParameterDefinition::Name::get()
            {
                BSTR n = NULL;
                StringNonZeroCheck(m_ptr->get_Name(&n));
                String^ s = ref new String(n);
                return s;
            }

            String^ PrintSchemaParameterDefinition::NamespaceUri::get()
            {
                BSTR n = NULL;
                StringNonZeroCheck(m_ptr->get_NamespaceUri(&n));
                String^ s = ref new String(n);
                return s;
            }

            bool PrintSchemaParameterDefinition::UserInputRequired::get()
            {
                BOOL b;
                ThrowIf(m_ptr->get_UserInputRequired(&b));
                return b ? true : false;
            }

            String^ PrintSchemaParameterDefinition::UnitType::get()
            {
                BSTR b = NULL;
                StringNonZeroCheck(m_ptr->get_UnitType(&b));
                String^ s = ref new String(b);
                return s;
            }

            int PrintSchemaParameterDefinition::DataType::get()
            {
                PrintSchemaParameterDataType i;
                ThrowIf(m_ptr->get_DataType(&i));
                return i;
            }

            int PrintSchemaParameterDefinition::RangeMin::get()
            {
                INT i;
                ThrowIf(m_ptr->get_RangeMin(&i));
                return i;
            }

            int PrintSchemaParameterDefinition::RangeMax::get()
            {
                INT i;
                ThrowIf(m_ptr->get_RangeMax(&i));
                return i;
            }
        }
    }
}

