/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print queue view
*   as described in https://msdn.microsoft.com/library/windows/hardware/dn265392
*/
#include "pch.h"

#include "PrintJob.h"

#include "PrinterQueueEventHelperEventArgs.h"
#include "PrinterQueueViewEventHelperEventArgs.h"
#include "PrinterPrintJobEventHandler.h"
#include "PrinterPrintQueueEventHandlerHelper.h"
#include "PrinterQueueEventHelperCallback.h"
#include "PrinterQueueEventHelper.h"
#include "PrinterQueueViewEventArgs.h"
#include "PrinterQueueView.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrinterQueueView::~PrinterQueueView()
            {
                if (m_handler != nullptr)
                {
                    m_handler->OnChange -= token;
                }

                m_handler = nullptr;
            }

            PrinterQueueView::PrinterQueueView(IPrinterQueueView* view)
            {
                m_ptr = view;
                m_handler = ref new PrinterPrintQueueEventHandlerHelper((long long)view, nullptr);
                token = m_handler->OnChange += ref new Windows::Foundation::TypedEventHandler<Platform::Object ^, PrinterQueueViewEventHelperEventArgs ^>(this, &PrinterContextNativeRuntimeComponent::Printing::PrinterExtension::PrinterQueueView::OnOnChange);
            }

            void PrinterQueueView::OnOnChange(Platform::Object ^sender, PrinterQueueViewEventHelperEventArgs ^args)
            {
                // Invoke this event when the sink event is raised
                OnChange(this, ref new PrinterQueueViewEventArgs(args));
            }

            void PrinterQueueView::SetViewRange(int start, int length)
            {
                ULONG u1 = start;
                ULONG u2 = start;
                ThrowIf(m_ptr->SetViewRange(u1, u2));
            }
        }
    }
}

