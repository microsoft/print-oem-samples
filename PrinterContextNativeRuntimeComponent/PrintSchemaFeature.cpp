/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print schema feature
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/hh451284(v=vs.85).aspx
*/
#include "pch.h"

#include "PrintSchemaElement.h"
#include "PrintSchemaOption.h"
#include "PrintSchemaFeature.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrintSchemaFeature::PrintSchemaFeature(IPrintSchemaFeature* ptr)
            {
                m_ptr = ptr;
            }

            IPrintSchemaFeature* PrintSchemaFeature::AsPtr::get()
            {
                return m_ptr.Get();
            }

            String^ PrintSchemaFeature::Xml::get()
            {
                ComPtr<IPrintSchemaElement> ip;
                StringNonZeroCheck(m_ptr.As(&ip));

                return PrintSchemaElement::GetXmlStreamForElement((ULONGLONG)ip.Get());

            }

            String^ PrintSchemaFeature::Name::get()
            {
                BSTR n = NULL;
                StringNonZeroCheck(m_ptr->get_Name(&n));
                String^ s = ref new String(n);
                return s;
            }

            String^ PrintSchemaFeature::NamespaceUri::get()
            {
                BSTR n = NULL;
                StringNonZeroCheck(m_ptr->get_NamespaceUri(&n));
                String^ s = ref new String(n);
                return s;
            }

            String^ PrintSchemaFeature::DisplayName::get()
            {
                BSTR n = NULL;
                StringNonZeroCheck(m_ptr->get_DisplayName(&n));
                String^ s = ref new String(n);
                return s;
            }

            ULONGLONG PrintSchemaFeature::GetSelectedOption2()
            {
                IPrintSchemaOption* i = NULL;
                ThrowIf(m_ptr->get_SelectedOption(&i));
                return (ULONGLONG)i;
            }

            void PrintSchemaFeature::PutSelectedOption2(ULONGLONG option)
            {
                ComPtr<IPrintSchemaOption> i = (IPrintSchemaOption*)option;
                ThrowIf(m_ptr->put_SelectedOption(i.Get()));
            }

            PrintSchemaOption^ PrintSchemaFeature::SelectedOption::get()
            {
                return ref new PrintSchemaOption(GetSelectedOption2());
            }

            void PrintSchemaFeature::SelectedOption::set(PrintSchemaOption^ value)
            {
                PutSelectedOption2(value->Option);
            }

            int PrintSchemaFeature::SelectionType::get()
            {
                PrintSchemaSelectionType t;
                ThrowIf(m_ptr->get_SelectionType(&t));
                return (int)t;
            }

            ULONGLONG PrintSchemaFeature::GetOption2(String^ name, String^ namespaceUri)
            {
                AutoBSTR nm(name);
                AutoBSTR ns(namespaceUri);
                IPrintSchemaOption* i = NULL;
                ThrowIf(m_ptr->GetOption(nm, ns, &i));
                return (ULONGLONG)i;
            }

            bool PrintSchemaFeature::DisplayUI::get()
            {
                BOOL g;
                ThrowIf(m_ptr->get_DisplayUI(&g));
                return g ? true : false;
            }
        }
    }
}

