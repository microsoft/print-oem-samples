/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print schema option
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/hh451335(v=vs.85).aspx
*/
#include "pch.h"

#include "PrintSchemaOption.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrintSchemaOption::PrintSchemaOption(IPrintSchemaOption* option, void* notused)
            {
                m_option = option;
            }

            PrintSchemaOption::PrintSchemaOption(ULONGLONG option)
            {
                m_option = ((IPrintSchemaOption*)option);
            }

            Boolean PrintSchemaOption::Selected::get()
            {
                BOOL selected = FALSE;
                ThrowIf(m_option->get_Selected(&selected));
                return selected ? true : false;
            }

            int PrintSchemaOption::Constrained::get() {
                PrintSchemaConstrainedSetting setting;
                ThrowIf(m_option->get_Constrained(&setting));
                return setting;
            }

            String^ PrintSchemaOption::DisplayName::get()
            {
                BSTR b = NULL;
                StringNonZeroCheck(m_option->get_DisplayName(&b));
                String^ ret = ref new String(b);
                return ret;
            }
            String^ PrintSchemaOption::Name::get()
            {
                BSTR b = NULL;
                StringNonZeroCheck(m_option->get_Name(&b));
                String^ ret = ref new String(b);
                return ret;
            }

            String^ PrintSchemaOption::XmlNamespace::get()
            {
                BSTR b = NULL;
                StringNonZeroCheck(m_option->get_NamespaceUri(&b));
                String^ ret = ref new String(b);
                return ret;
            }

            ULONGLONG PrintSchemaOption::Option::get()
            {
                return (ULONGLONG)m_option.Get();
            }
        }
    }
}

