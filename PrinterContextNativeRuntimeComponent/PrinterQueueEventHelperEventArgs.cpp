/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents print queue event helper callback handler arguments
*   as described in https://msdn.microsoft.com/library/windows/hardware/dn265393
*/
#include "pch.h"

#include "PrinterQueueEventHelperEventArgs.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrinterQueueEventHelperEventArgs::PrinterQueueEventHelperEventArgs(Object^ printerQueue, String^ str, int hresult)
            {
                if (printerQueue == nullptr)
                {
                    throw ref new InvalidArgumentException();
                }

                m_printerQueueEventHR = hresult;
                IPrinterQueue* qtmp = (IPrinterQueue*)(void*)printerQueue;
                m_printerQueue.Attach(qtmp);
                m_printerQueueEventResponse = str;
            }

            PrinterQueueEventHelperEventArgs::PrinterQueueEventHelperEventArgs(String^ str, int hresult)
            {
                m_printerQueueEventHR = hresult;
                m_printerQueueEventResponse = str;
            }

            String^ PrinterQueueEventHelperEventArgs::Response::get()
            {
                return m_printerQueueEventResponse;
            }

            int PrinterQueueEventHelperEventArgs::HResult::get()
            {
                return m_printerQueueEventHR;
            }

            Object^ PrinterQueueEventHelperEventArgs::Queue::get()
            {
                return (Object^)(IntPtr)m_printerQueue.Get();
            }
        }
    }
}

