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
#include "pch.h"

#include "PrintSChemaOption.h"
#include "PrintSchemaNUpOption.h"
#include "PrintSchemaFeature.h"
#include "PrintSchemaPageImageableSize.h"
#include "PrintSchemaPageMediaSizeOption.h"
#include "PrintSchemaParameterDefinition.h"
#include "PrintSchemaElement.h"
#include "PrintSchemaOption.h"
#include "PrintSchemaFeature.h"
#include "PrintSchemaParameterInitializer.h"
#include "PrintSchemaCapabilities.h"
#include "PrintSchemaAsyncOperationCallback.h"
#include "PrintSchemaAsyncOp.h"
#include "PrintSchemaTicket.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrintSchemaCapabilities::PrintSchemaCapabilities(IPrintSchemaCapabilities* caps)
            {
                m_capabilities = caps;
                ThrowIf(m_capabilities.As(&m_capabilities2));
            }

            PrintSchemaCapabilities::PrintSchemaCapabilities(ULONGLONG caps)
            {
                m_capabilities = (IPrintSchemaCapabilities*)caps;
                ThrowIf(m_capabilities.As(&m_capabilities2));
            }

            ULONGLONG PrintSchemaCapabilities::GetFeatureByKeyName2(String^ keyName)
            {
                if (keyName == nullptr || keyName->IsEmpty()) throw ref new InvalidArgumentException();
                AutoBSTR a(keyName);
                IPrintSchemaFeature* f;
                if (m_capabilities->GetFeatureByKeyName(a, &f) != 0) return NULL;
                return (ULONGLONG)f;
            }

            PrintSchemaFeature^ PrintSchemaCapabilities::GetFeatureByKeyName(String^ name)
            {
                auto f = (IPrintSchemaFeature*)GetFeatureByKeyName2(name);
                if (f)
                {
                    return ref new PrintSchemaFeature(f);
                }
                return nullptr;
            }

            ULONGLONG PrintSchemaCapabilities::GetFeature2(String^ featureName, String^ xmlNamespace)
            {
                if (featureName == nullptr || featureName->IsEmpty()) throw ref new InvalidArgumentException();
                if (xmlNamespace == nullptr || xmlNamespace->IsEmpty()) throw ref new InvalidArgumentException();
                AutoBSTR a(featureName);
                AutoBSTR b(xmlNamespace);
                IPrintSchemaFeature* f;
                if (m_capabilities->GetFeature(a, b, &f) != 0) return NULL;
                return (ULONGLONG)f;
            }

            PrintSchemaFeature^ PrintSchemaCapabilities::GetFeature(String^ featureName, String^ xmlNamespace)
            {
                auto f = (IPrintSchemaFeature*)GetFeature2(featureName, xmlNamespace);
                if (f)
                {
                    return ref new PrintSchemaFeature(f);
                }
                return nullptr;
            }

            ULONGLONG PrintSchemaCapabilities::GetFeature2(String^ name)
            {
                return GetFeature2(name, L"http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords");
            }

            PrintSchemaFeature^ PrintSchemaCapabilities::GetFeature(String^ name)
            {
                auto f = (IPrintSchemaFeature*)GetFeature2(name);
                if (f)
                {
                    return ref new PrintSchemaFeature(f);
                }
                return nullptr;
            }

            ULONGLONG PrintSchemaCapabilities::PageImageableSize2::get()
            {
                IPrintSchemaPageImageableSize* s;
                ThrowIf(m_capabilities->get_PageImageableSize(&s));
                if (s == NULL) throw ref new Exception(E_UNEXPECTED);
                return (ULONGLONG)s;
            }

            PrintSchemaPageImageableSize^ PrintSchemaCapabilities::PageImageableSize::get()
            {
                return ref new PrintSchemaPageImageableSize((IPrintSchemaPageImageableSize*)PageImageableSize2);
            }

            unsigned int PrintSchemaCapabilities::JobCopiesAllDocumentsMaxValue::get()
            {
                ULONG i = 0;
                ThrowIf(m_capabilities->get_JobCopiesAllDocumentsMaxValue(&i));
                return i;
            }

            unsigned int PrintSchemaCapabilities::JobCopiesAllDocumentsMinValue::get()
            {
                ULONG i = 0;
                ThrowIf(m_capabilities->get_JobCopiesAllDocumentsMinValue(&i));
                return i;
            }

            //Could be one of several types - feature is IPrintSchemaFeature*
            Object^ PrintSchemaCapabilities::GetSelectedOptionInPrintTicket2(ULONGLONG feature)
            {
                IPrintSchemaFeature* psf = (IPrintSchemaFeature*)feature;
                ComPtr<IPrintSchemaOption> opt;
                ThrowIf(m_capabilities->GetSelectedOptionInPrintTicket(psf, opt.GetAddressOf()));
                if (!opt.Get()) return nullptr;
                ComPtr<IPrintSchemaNUpOption> nuopt;
                if (SUCCEEDED(opt.As(&nuopt)))
                {
                    return ref new PrintSchemaNUpOption(nuopt.Get(), NULL);
                }
                ComPtr<IPrintSchemaPageMediaSizeOption> mopt;
                if (SUCCEEDED(opt.As(&mopt)))
                {
                    return ref new PrintSchemaPageMediaSizeOption(mopt.Get(), NULL);
                }
                return ref new PrintSchemaOption(opt.Get(), NULL);
            }

            IIterable<PrintSchemaOption^>^ PrintSchemaCapabilities::GetOptions(PrintSchemaFeature^ feature)
            {
                IPrintSchemaFeature* psf = feature->AsPtr;
                ComPtr<IPrintSchemaOptionCollection> opts;
                ThrowIf(m_capabilities->GetOptions(psf, opts.GetAddressOf()));
                if (opts == NULL) throw ref new Exception(E_UNEXPECTED);
                Platform::Collections::Vector<PrintSchemaOption^>^ v = ref new Platform::Collections::Vector<PrintSchemaOption^>();
                ULONG count;
                ThrowIf(opts->get_Count(&count));
                for (ULONG i = 0; i < count; i++)
                {
                    ComPtr<IPrintSchemaOption> opt;
                    ThrowIf(opts->GetAt(i, opt.GetAddressOf()));
                    v->Append(ref new PrintSchemaOption(opt.Get(), NULL));
                }
                return v;
            }

            ULONGLONG PrintSchemaCapabilities::GetParameterDefinition2(String^ parameterName, String^ xmlNamespace)
            {
                if (parameterName == nullptr || parameterName->IsEmpty()) throw ref new InvalidArgumentException();
                if (xmlNamespace == nullptr || xmlNamespace->IsEmpty()) throw ref new InvalidArgumentException();
                AutoBSTR a(parameterName);
                AutoBSTR b(xmlNamespace);
                IPrintSchemaParameterDefinition* i;
                ThrowIf(m_capabilities2->GetParameterDefinition(a, b, &i));
                if (i == NULL) throw ref new Exception(E_UNEXPECTED);
                return (ULONGLONG)i;
            }

            PrintSchemaParameterDefinition^ PrintSchemaCapabilities::GetParameterDefinition(String^ parameterName, String^ xmlNamespace)
            {
                return ref new PrintSchemaParameterDefinition((IPrintSchemaParameterDefinition*)GetParameterDefinition2(parameterName, xmlNamespace));
            }

            String^ PrintSchemaCapabilities::XmlStream::get()
            {
                ComPtr<IPrintSchemaElement> ip;
                StringNonZeroCheck(m_capabilities.As(&ip));
                return PrintSchemaTicket::GetXmlStreamForElement((ULONGLONG)ip.Get());
            }

            ULONGLONG PrintSchemaCapabilities::Capabilities::get()
            {
                return (ULONGLONG)m_capabilities.Get();
            }

            ULONGLONG PrintSchemaCapabilities::Capabilities2::get()
            {
                return (ULONGLONG)m_capabilities2.Get();
            }
        }
    }
}

