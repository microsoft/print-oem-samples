/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents print page media size options
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/hh451378(v=vs.85).aspx
*/
#include "pch.h"

#include "PrintSchemaPageMediaSizeOption.h"

// Namespaces
using namespace Windows::Foundation; 

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrintSchemaPageMediaSizeOption::PrintSchemaPageMediaSizeOption(IPrintSchemaPageMediaSizeOption* option, void* notused)
            {
                m_option = option;
            }

            PrintSchemaPageMediaSizeOption::PrintSchemaPageMediaSizeOption(ULONGLONG option)
            {
                ComPtr<IUnknown> unk = (IUnknown*)option;
                ThrowIf(unk.As(&m_option));
            }

            uint32 PrintSchemaPageMediaSizeOption::HeightInMicrons::get()
            {
                ULONG l = 0;
                ThrowIf(m_option->get_HeightInMicrons(&l));
                return l;
            }

            uint32 PrintSchemaPageMediaSizeOption::WidthInMicrons::get()
            {
                ULONG l = 0;
                ThrowIf(m_option->get_WidthInMicrons(&l));
                return l;
            }

            Boolean PrintSchemaPageMediaSizeOption::Selected::get()
            {
                BOOL selected = FALSE;
                ThrowIf(m_option->get_Selected(&selected));
                return selected ? true : false;
            }


            int PrintSchemaPageMediaSizeOption::Constrained::get()
            {
                PrintSchemaConstrainedSetting setting;
                ThrowIf(m_option->get_Constrained(&setting));
                return setting;
            }

            String^ PrintSchemaPageMediaSizeOption::DisplayName::get()
            {
                BSTR b = NULL;
                StringNonZeroCheck(m_option->get_DisplayName(&b));
                String^ ret = ref new String(b);
                return ret;
            }

            String^ PrintSchemaPageMediaSizeOption::Name::get()
            {
                BSTR b = NULL;
                StringNonZeroCheck(m_option->get_Name(&b));
                String^ ret = ref new String(b);
                return ret;
            }

            String^ PrintSchemaPageMediaSizeOption::XmlNamespace::get()
            {
                BSTR b = NULL;
                StringNonZeroCheck(m_option->get_NamespaceUri(&b));
                String^ ret = ref new String(b);
                return ret;
            }

            ULONGLONG PrintSchemaPageMediaSizeOption::Option::get()
            {
                return (ULONGLONG)m_option.Get();
            }
        }
    }
}

