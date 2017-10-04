/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print schema asynchronous callback operation
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/hh451224(v=vs.85).aspx
*/
#include "pch.h"

#include <PrinterExtensionDispId.h>
#include "PrintSchemaAsyncOperationCallback.h"
#include "PrintSchemaAsyncOp.h"

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrintSchemaAsyncOp::~PrintSchemaAsyncOp()
            {
                CloseHandle(m_asyncOperationEvent);
            }

            void PrintSchemaAsyncOp::Initialize(IPrintSchemaAsyncOperation* op)
            {
                m_asyncOperationEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
                if (!m_asyncOperationEvent)
                {
                    throw ref new Exception(0x80000002, L"Failed to create event");
                }
                helper = Make<PrintSchemaAsyncOperationCallback>(op, this);
            }

            PrintSchemaAsyncOp::PrintSchemaAsyncOp(ULONGLONG op)
            {
                IPrintSchemaAsyncOperation* iop = (IPrintSchemaAsyncOperation*)op;
                Initialize(iop);
            }

            PrintSchemaAsyncOp::PrintSchemaAsyncOp(IPrintSchemaAsyncOperation* op)
            {
                Initialize(op);
            }

            void PrintSchemaAsyncOp::ManagedPPrintSchemaAsyncOperationCallback(ULONGLONG ticket, int hr)
            {
                m_printTicket = ticket;
                m_asyncOperationHR = hr;
                if (!SetEvent(m_asyncOperationEvent))
                {
                    throw ref new Exception(0x80000002, L"Failed to set event");
                }
            }


            void PrintSchemaAsyncOp::Wait()
            {
                WaitForSingleObject(m_asyncOperationEvent, -1);
            }

            bool PrintSchemaAsyncOp::Wait(int spanInMS)
            {
                return WaitForSingleObject(m_asyncOperationEvent, spanInMS) == STATUS_WAIT_0;
            }
        }
    }
}