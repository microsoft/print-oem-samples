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
#include "pch.h"

#include "PrintSchemaElement.h"
#include "PrintSchemaOption.h"
#include "PrintSchemaFeature.h"
#include "PrintSchemaParameterInitializer.h"
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
            PrintSchemaTicket::PrintSchemaTicket(IPrintSchemaTicket* ticket)
            {
                m_ticket = ((IPrintSchemaTicket*)ticket);
            }

            //Inherently unsafe
            PrintSchemaTicket::PrintSchemaTicket(ULONGLONG ptrTicket)
            {
                m_ticket = ((IPrintSchemaTicket*)ptrTicket);
            }

            ULONGLONG PrintSchemaTicket::Ticket::get()
            {
                return (ULONGLONG)m_ticket.Get();
            }

            ULONGLONG PrintSchemaTicket::GetCapabilities2()
            {
                IPrintSchemaCapabilities* i = NULL;
                ThrowIf(m_ticket->GetCapabilities(&i));
                return (ULONGLONG)i;
            }

            ULONGLONG PrintSchemaTicket::GetFeature2(String^ name, String^ namespaceString)
            {
                if (name == nullptr || name->IsEmpty() || namespaceString == nullptr || namespaceString->IsEmpty()) throw ref new InvalidArgumentException();
                AutoBSTR n(name);
                AutoBSTR ns(namespaceString);
                IPrintSchemaFeature* f = NULL;
                if (m_ticket->GetFeature(n, ns, &f) != 0) return NULL;
                return (ULONGLONG)f;
            }

            PrintSchemaFeature^ PrintSchemaTicket::GetFeature(String^ name, String^ namespaceString)
            {
                auto f = (IPrintSchemaFeature*)GetFeature2(name, namespaceString);
                if (f) {
                    return ref new PrintSchemaFeature(f);
                }
                return nullptr;
            }

            ULONGLONG PrintSchemaTicket::GetFeature2(String^ name)
            {
                return GetFeature2(name, L"http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords");
            }

            PrintSchemaFeature^ PrintSchemaTicket::GetFeature(String^ name)
            {
                auto f = (IPrintSchemaFeature*)GetFeature2(name);
                if (f)
                {
                    return ref new PrintSchemaFeature(f);
                }
                return nullptr;
            }

            ULONGLONG PrintSchemaTicket::GetFeatureByKeyname2(String^ name)
            {
                if (name == nullptr || name->IsEmpty()) throw ref new InvalidArgumentException();
                AutoBSTR n(name);
                IPrintSchemaFeature* f = NULL;
                if (m_ticket->GetFeatureByKeyName(n, &f) != 0) return NULL;
                return (ULONGLONG)f;
            }
            PrintSchemaFeature^ PrintSchemaTicket::GetFeatureByKeyName(String^ name)
            {
                auto f = (IPrintSchemaFeature*)GetFeatureByKeyname2(name);
                if (f)
                {
                    return ref new PrintSchemaFeature(f);
                }
                return nullptr;
            }

            ULONGLONG PrintSchemaTicket::GetParameterInitializer2(String^ name, String^ namespac)
            {
                if (name == nullptr || name->IsEmpty() || namespac == nullptr || namespac->IsEmpty()) throw ref new InvalidArgumentException();
                AutoBSTR n(name);
                AutoBSTR ns(namespac);
                IPrintSchemaParameterInitializer* i;
                ComPtr<IPrintSchemaTicket2> t2;
                ThrowIf(m_ticket.As(&t2));
                ThrowIf(t2->GetParameterInitializer(n, ns, &i));
                return (ULONGLONG)i;
            }

            ULONGLONG PrintSchemaTicket::GetParameterInitializer2(String^ name)
            {
                return GetParameterInitializer2(name, L"http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords");
            }

            PrintSchemaParameterInitializer^ PrintSchemaTicket::GetParameterInitializer(String^ name, String^ namespac)
            {
                return ref new PrintSchemaParameterInitializer((IPrintSchemaParameterInitializer*)GetParameterInitializer2(name, namespac));
            }

            PrintSchemaParameterInitializer^ PrintSchemaTicket::GetParameterInitializer(String^ name)
            {
                return ref new PrintSchemaParameterInitializer((IPrintSchemaParameterInitializer*)GetParameterInitializer2(name));
            }

            PrintSchemaAsyncOp^ PrintSchemaTicket::ValidateAsync2()
            {
                IPrintSchemaAsyncOperation* op = NULL;
                ThrowIf(m_ticket->ValidateAsync(&op));
                op->Start();
                return ref new PrintSchemaAsyncOp(op);
            }

            //Inherently unsafe
            PrintSchemaAsyncOp^ PrintSchemaTicket::CommitAsync2(ULONGLONG IPrintSchemaTicketAddr)
            {
                if (!IPrintSchemaTicketAddr) throw ref new InvalidArgumentException();
                IPrintSchemaAsyncOperation* op = NULL;
                ThrowIf(m_ticket->CommitAsync((IPrintSchemaTicket*)IPrintSchemaTicketAddr, &op));
                op->Start();
                return ref new PrintSchemaAsyncOp(op);
            }

            PrintSchemaAsyncOp^ PrintSchemaTicket::ValidateAndCommitPrintTicketXML(String^ xml)
            {
                XmlStream = xml;
                return CommitAsync2((ULONGLONG)m_ticket.Get());
            }

            void PrintSchemaTicket::NotifyXmlChanged()
            {
                ThrowIf(m_ticket->NotifyXmlChanged());
            }

            int PrintSchemaTicket::JobCopiesAllDocuments::get()
            {
                ULONG i;
                ThrowIf(m_ticket->get_JobCopiesAllDocuments(&i));
                return i;
            }

            void PrintSchemaTicket::JobCopiesAllDocuments::set(int val)
            {
                ThrowIf(m_ticket->put_JobCopiesAllDocuments(val));
            }

            String^ PrintSchemaTicket::XmlStream::get()
            {
                ComPtr<IUnknown> i;
                StringNonZeroCheck(m_ticket->get_XmlNode(i.GetAddressOf()));
                ComPtr<IXMLDOMDocument2> doc;
                StringNonZeroCheck(i.As(&doc));
                BSTR bstr;
                StringNonZeroCheck(doc->get_xml(&bstr));
                String^ ret = ref new String(bstr);
                return ret;
            }

            void PrintSchemaTicket::XmlStream::set(String^ xml)
            {
                ComPtr<IUnknown> i;
                ThrowIf(m_ticket->get_XmlNode(i.GetAddressOf()));
                ComPtr<IXMLDOMDocument2> doc;
                ThrowIf(i.As(&doc));
                AutoBSTR a(xml);
                VARIANT_BOOL success;
                ThrowIf(doc->loadXML(a, &success));
                ThrowIf(m_ticket->NotifyXmlChanged());
            }

            String^ PrintSchemaTicket::GetXmlStreamForCapabilities(ULONGLONG ptrIPrintSchemaCapabilities)
            {
                ComPtr<IPrintSchemaCapabilities> cp = (IPrintSchemaCapabilities*)ptrIPrintSchemaCapabilities;
                ComPtr<IUnknown> i;
                StringNonZeroCheck(cp->get_XmlNode(i.GetAddressOf()));
                ComPtr<IXMLDOMDocument2> doc;
                StringNonZeroCheck(i.As(&doc));
                BSTR bstr;
                StringNonZeroCheck(doc->get_xml(&bstr));
                String^ ret = ref new String(bstr);
                return ret;
            }

            String^ PrintSchemaTicket::GetXmlStreamForElement(ULONGLONG ptrIPrintSchemaElement)
            {
                return PrintSchemaElement::GetXmlStreamForElement(ptrIPrintSchemaElement);
            }
        }
    }
}

