/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print N-Up option
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/hh451302(v=vs.85).aspx
*/
#include "pch.h"

#include "PrintSchemaNUpOption.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrintSchemaNUpOption::PrintSchemaNUpOption(IPrintSchemaNUpOption* option, void* notused)
            {
                m_option = option;
            }

            PrintSchemaNUpOption::PrintSchemaNUpOption(ULONGLONG option)
            {
                ComPtr<IUnknown> unk = (IUnknown*)option;
                ThrowIf(unk.As(&m_option));
            }

            int PrintSchemaNUpOption::PagesPerSheet::get()
            {
                ULONG pps = 0;
                ThrowIf(m_option->get_PagesPerSheet(&pps)); return pps;
            }

            Boolean PrintSchemaNUpOption::Selected::get()
            {
                BOOL selected = FALSE;
                ThrowIf(m_option->get_Selected(&selected));
                return selected ? true : false;
            }

            int PrintSchemaNUpOption::Constrained::get() {
                PrintSchemaConstrainedSetting setting;
                ThrowIf(m_option->get_Constrained(&setting));
                return setting;
            }

            String^ PrintSchemaNUpOption::DisplayName::get()
            {
                BSTR b = NULL;
                StringNonZeroCheck(m_option->get_DisplayName(&b));
                String^ ret = ref new String(b);
                return ret;
            }

            String^ PrintSchemaNUpOption::Name::get()
            {
                BSTR b = NULL;
                StringNonZeroCheck(m_option->get_Name(&b));
                String^ ret = ref new String(b);
                return ret;
            }

            String^ PrintSchemaNUpOption::XmlNamespace::get()
            {
                BSTR b = NULL;
                StringNonZeroCheck(m_option->get_NamespaceUri(&b));
                String^ ret = ref new String(b);
                return ret;
            }

            ULONGLONG PrintSchemaNUpOption::Option::get()
            {
                return (ULONGLONG)m_option.Get();
            }
        }
    }
}

