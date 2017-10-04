/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a printer queue v2 interface
*   as described in https://msdn.microsoft.com/library/windows/hardware/dn265389
*/
#include "pch.h"


#include "PrintJob.h"
#include "PrinterProperty.h"
#include "PrinterPropertyBag.h"
#include "PrinterQueueEventHelperEventArgs.h"
#include "PrinterQueueViewEventHelperEventArgs.h"
#include "PrinterQueueEventHelperCallback.h"
#include "PrinterPrintJobEventHandler.h"
#include "PrinterPrintQueueEventHandlerHelper.h"
#include "PrinterQueueEventHelper.h"
#include "PrinterQueueBidiSetRequestTask.h"
#include "BidiSetRequestCallback.h"
#include "PrinterQueueViewEventArgs.h"
#include "PrinterQueueView.h"
#include "PrinterQueue2.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrinterQueue2::~PrinterQueue2()
            {
                if (_sink != nullptr)
                {
                    _sink->OnPrinterQueueEvent -= _handler;
                }
                _sink = nullptr;
            }

            PrinterQueue2::PrinterQueue2(ULONGLONG ptrIPrinterQueue2)
            {
                m_ptr = (IPrinterQueue2*)ptrIPrinterQueue2;
                ComPtr<IPrinterQueue> ip;
                ThrowIf(m_ptr.As(&ip));
                _sink = ref new PrinterQueueEventHelper(ip.Get());
                _handler = _sink->OnPrinterQueueEvent += ref new PrintQueueEventHelperHandler(this, &PrinterContextNativeRuntimeComponent::Printing::PrinterExtension::PrinterQueue2::OnOnPrinterQueueEvent);
            }

            String^ PrinterQueue2::Name::get()
            {
                BSTR b = NULL;
                StringNonZeroCheck(m_ptr->get_Name(&b));
                String^ s = ref new String(b);
                return s;
            }

            void PrinterQueue2::SendBidiQuery(String^ bidiQuery)
            {
                AutoBSTR b(bidiQuery);
                ThrowIf(m_ptr->SendBidiQuery(b));
            }

            ULONGLONG PrinterQueue2::Handle::get()
            {
                HANDLE h;
                ThrowIf(m_ptr->get_Handle(&h));
                return (ULONGLONG)h;
            }

            void PrinterQueue2::SendBidiSetRequestAsyncNoTask(String^ bidiRequest)
            {
                auto cb = ref new Windows::Foundation::TypedEventHandler<Object^, PrinterQueueEventHelperEventArgs^>(this, &PrinterContextNativeRuntimeComponent::Printing::PrinterExtension::PrinterQueue2::OnOnPrinterQueueEvent);
                ComPtr<BidiSetRequestCallback> b = Make<BidiSetRequestCallback>(cb);
                ComPtr<IPrinterBidiSetRequestCallback> p;
                ThrowIf(b.As(&p));
                AutoBSTR b2(bidiRequest);
                ComPtr<IPrinterExtensionAsyncOperation> ia;
                ThrowIf(m_ptr->SendBidiSetRequestAsync(b2, p.Get(), ia.GetAddressOf()));
            }

            IAsyncOperation<PrinterQueueEventHelperEventArgs^>^ PrinterQueue2::SendBidiSetRequestAsync(String^ bidiRequest)
            {
                PrinterQueueBidiSetRequestTask^ task = ref new PrinterQueueBidiSetRequestTask(m_ptr.Get());
                return task->SendBidiRequest(bidiRequest);
            }

            PrinterPropertyBag^ PrinterQueue2::GetProperties()
            {
                ComPtr<IPrinterPropertyBag> bag;
                ThrowIf(m_ptr->GetProperties(&bag));
                return ref new PrinterPropertyBag(bag.Get(), 2); //User
            }

            PrinterQueueView^ PrinterQueue2::GetPrinterQueueView(int viewOffset, int viewSize)
            {
                ComPtr<IPrinterQueueView> view;
                ThrowIf(m_ptr->GetPrinterQueueView(viewOffset, viewSize, view.GetAddressOf()));
                return ref new PrinterQueueView(view.Get());
            }

            void PrinterQueue2::OnOnPrinterQueueEvent(Platform::Object ^helper, PrinterQueueEventHelperEventArgs ^args)
            {
                //Invoke this event when the sink event is raised
                OnBidiResponseReceivedEvent(this, args);
            }
        }
    }
}

