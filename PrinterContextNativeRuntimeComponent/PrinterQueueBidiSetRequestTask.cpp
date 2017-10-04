/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class allows a bidisetrequest to be done as a task - one instance is required per task
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/dn265391(v=vs.85).aspx
*/
#include "pch.h"


#include "PrinterQueueEventHelperEventArgs.h"
#include "PrinterQueueViewEventHelperEventArgs.h"
#include "PrinterQueueEventHelperCallback.h"
#include "PrinterPrintJobEventHandler.h"
#include "PrinterQueueEventHelper.h"
#include "BidiSetRequestCallback.h"
#include "PrinterQueueBidiSetRequestTask.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrinterQueueBidiSetRequestTask::~PrinterQueueBidiSetRequestTask()
            {
                if (hEvent)
                {
                    CloseHandle(hEvent);
                    hEvent = NULL;
                }

                m_lastargs = nullptr;
            }

            PrinterQueueBidiSetRequestTask::PrinterQueueBidiSetRequestTask(IPrinterQueue2* queue)
            {
                hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
                if (hEvent == NULL) throw ref new Exception(HRESULT_FROM_WIN32(GetLastError()));
                m_ptr = queue;
                ComPtr<IPrinterQueue> ip;
                ThrowIf(m_ptr.As(&ip));
            }

            IAsyncOperation<PrinterQueueEventHelperEventArgs^>^ PrinterQueueBidiSetRequestTask::SendBidiRequest(String^ bidiRequest)
            {
                HANDLE hHandle = hEvent;
                return create_async([bidiRequest, this, hHandle] {
                    auto cb = ref new Windows::Foundation::TypedEventHandler<Object^, PrinterQueueEventHelperEventArgs^>(this, &PrinterContextNativeRuntimeComponent::Printing::PrinterExtension::PrinterQueueBidiSetRequestTask::OnOnPrinterQueueEvent);
                    ComPtr<BidiSetRequestCallback> b = Make<BidiSetRequestCallback>(cb);
                    ComPtr<IPrinterBidiSetRequestCallback> p;
                    ThrowIf(b.As(&p));
                    AutoBSTR b2(bidiRequest);
                    ComPtr<IPrinterExtensionAsyncOperation> ia;
                    ThrowIf(m_ptr->SendBidiSetRequestAsync(b2, p.Get(), ia.GetAddressOf()));
                    WaitForSingleObject(hHandle, -1);
                    ResetEvent(hHandle);
                    return this->LastArgs;
                }
                );
            }

            PrinterQueueEventHelperEventArgs^ PrinterQueueBidiSetRequestTask::LastArgs::get()
            {
                return m_lastargs;
            }

            void PrinterQueueBidiSetRequestTask::OnOnPrinterQueueEvent(Platform::Object ^helper, PrinterQueueEventHelperEventArgs ^args)
            {
                m_lastargs = args;
                SetEvent(hEvent);
            }

        }
    }
}

