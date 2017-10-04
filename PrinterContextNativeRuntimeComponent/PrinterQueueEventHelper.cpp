/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print queue event helper
*   as described in https://msdn.microsoft.com/library/windows/hardware/dn265393
*/
#include "pch.h"

#include "PrinterQueueEventHelperEventArgs.h"
#include "PrinterQueueViewEventHelperEventArgs.h"
#include "PrinterQueueEventHelperCallback.h"
#include "PrinterPrintJobEventHandler.h"
#include "PrinterQueueEventHelper.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrinterQueueEventHelper::PrinterQueueEventHelper(IPrinterQueue * printerQueue)
            {
                queue.Attach(printerQueue);
                Windows::Foundation::TypedEventHandler<Object^, PrinterQueueEventHelperEventArgs^>^ p = ref new Windows::Foundation::TypedEventHandler<Object^, PrinterQueueEventHelperEventArgs^>(this, &PrinterQueueEventHelper::PrinterQueueEventHelperResult);
                helperHelper = Make<PrinterQueueEventHelperCallback>(printerQueue, p);
            }

            PrinterQueueEventHelper::~PrinterQueueEventHelper()
            {
                queue = nullptr;
                helperHelper = nullptr;
            }

            void PrinterQueueEventHelper::PrinterQueueEventHelperResult(Object^ helper, PrinterQueueEventHelperEventArgs ^ args)
            {
                this->OnPrinterQueueEvent(this, args);
            }

            PrinterQueueEventHelper::PrinterQueueEventHelper(Object^ printerQueue, Object^ notUsed)
            {
                IPrinterQueue* q = (IPrinterQueue*)(void*)printerQueue;
                queue.Attach(q);
                Windows::Foundation::TypedEventHandler<Object^, PrinterQueueEventHelperEventArgs^>^ p = ref new Windows::Foundation::TypedEventHandler<Object^, PrinterQueueEventHelperEventArgs^>(this, &PrinterQueueEventHelper::PrinterQueueEventHelperResult);
                helperHelper = Make<PrinterQueueEventHelperCallback>(q, p);
            }

            Windows::Foundation::EventRegistrationToken PrinterQueueEventHelper::AddHandler(PrintQueueEventHelperHandler^ handler)
            {
                return OnPrinterQueueEvent += (handler);
            }

            void PrinterQueueEventHelper::RemoveHandler(Windows::Foundation::EventRegistrationToken handler)
            {
                OnPrinterQueueEvent -= handler;
            }
        }
    }
}