/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a WSDA printer extension context
*   as described in https://msdn.microsoft.com/library/windows/hardware/hh406649
*/
#include "pch.h"

#include "PrintJob.h"
#include "PrinterProperty.h"
#include "PrinterPropertyBag.h"
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
#include "PrinterQueueEventHelperEventArgs.h"
#include "PrinterQueueViewEventHelperEventArgs.h"
#include "PrinterQueueEventHelperCallback.h"
#include "PrinterQueueViewEventArgs.h"
#include "PrinterPrintJobEventHandler.h"
#include "PrinterPrintQueueEventHandlerHelper.h"
#include "PrinterQueueEventHelper.h"
#include "PrinterQueueView.h"
#include "PrinterQueue2.h"
#include "PrintSchemaAsyncOperationCallback.h"
#include "PrintSchemaAsyncOp.h"
#include "PrintSchemaTicket.h"
#include "PrinterExtensionContext.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrinterExtensionContext::PrinterExtensionContext(ULONGLONG context)
            {
                m_context = ((IPrinterExtensionContext*)context);
                ThrowIf(m_context->get_PrinterQueue(m_printerQueue.GetAddressOf()));
                if (m_ticket != nullptr)
                {
                    ThrowIf(m_context->get_PrintSchemaTicket(m_ticket.GetAddressOf()));
                }
            }

            PrinterQueue2^ PrinterExtensionContext::Queue::get()
            {
                ComPtr<IPrinterQueue2> q;
                ThrowIf(m_printerQueue.As(&q));
                return ref new PrinterQueue2((ULONGLONG)q.Get());
            }

            PrintSchemaTicket^ PrinterExtensionContext::Ticket::get()
            {
                if (m_ticket == nullptr)
                {
                    ThrowIf(m_context->get_PrintSchemaTicket(m_ticket.GetAddressOf()));
                }
                return ref new PrintSchemaTicket(m_ticket.Get());
            }

            PrinterPropertyBag^ PrinterExtensionContext::UserProperties::get()
            {
                return ref new PrinterPropertyBag((IPrinterPropertyBag*)UserProperties2, 2);
            }

            PrinterPropertyBag^ PrinterExtensionContext::DriverProperties::get()
            {
                return ref new PrinterPropertyBag((IPrinterPropertyBag*)DriverProperties2, 1);
            }

            ULONGLONG PrinterExtensionContext::UserProperties2::get()
            {
                IPrinterPropertyBag* i = NULL; ThrowIf(m_context->get_UserProperties(&i)); return (ULONGLONG)i;
            }

            ULONGLONG PrinterExtensionContext::DriverProperties2::get()
            {
                IPrinterPropertyBag* i = NULL;
                ThrowIf(m_context->get_DriverProperties(&i));
                return (ULONGLONG)i;
            }

            ULONGLONG PrinterExtensionContext::Context::get()
            {
                return (ULONGLONG)m_context.Get();
            }
        }
    }
}

