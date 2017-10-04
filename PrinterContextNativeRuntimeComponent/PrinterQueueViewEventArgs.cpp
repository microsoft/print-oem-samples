/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents print queue view event arguments
*   as described in https://msdn.microsoft.com/library/windows/hardware/dn265393
*/
#include "pch.h"

#include "PrintJob.h"
#include "PrinterQueueEventHelperEventArgs.h"
#include "PrinterQueueViewEventHelperEventArgs.h"
#include "PrinterQueueViewEventArgs.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {

            PrinterQueueViewEventArgs::~PrinterQueueViewEventArgs()
            {
                m_jobs = nullptr;
                jobs = nullptr;
            }

            PrinterQueueViewEventArgs::PrinterQueueViewEventArgs(PrinterQueueViewEventHelperEventArgs^ args)
            {
                jobs = ref new Platform::Collections::Vector<PrintJob^>();
                for each(auto a in args->IPrintJobVector)
                {
                    IPrintJob* ipj = (IPrintJob*)(long long)a;
                    PrintJob^ pj = ref new PrintJob(ipj);
                    jobs->Append(pj);
                }
                m_jobs = jobs;
            }

            int PrinterQueueViewEventArgs::ViewOffset::get()
            {
                return m_viewOffset;
            }

            int PrinterQueueViewEventArgs::ViewSize::get()
            {
                return m_ViewSize;
            }

            int PrinterQueueViewEventArgs::CountJobsInPrintQueue::get()
            {
                return m_CountJobsInPrintQueue;
            }

            Windows::Foundation::Collections::IIterable<PrintJob^>^ PrinterQueueViewEventArgs::Collection::get()
            {
                return m_jobs;
            }
        }
    }
}

