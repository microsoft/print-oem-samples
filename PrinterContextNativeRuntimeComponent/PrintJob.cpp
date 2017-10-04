/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print job
*   as described in https://msdn.microsoft.com/library/windows/hardware/dn265396
*/
#include "pch.h"

#include "PrintJob.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrintJob::PrintJob(IPrintJob* ptr)
            {
                m_ptr = ptr;
            }

            String^ PrintJob::Name::get()
            {
                BSTR b = NULL;
                StringNonZeroCheck(m_ptr->get_Name(&b));
                String^ s = ref new String(b);
                return s;
            }

            unsigned int PrintJob::Id::get()
            {
                ULONG u = 0;
                ThrowIf(m_ptr->get_Id(&u));
                return (unsigned int)u;
            }

            unsigned int PrintJob::PrintedPages::get()
            {
                ULONG u = 0;
                ThrowIf(m_ptr->get_PrintedPages(&u));
                return (unsigned int)u;
            }

            unsigned int PrintJob::TotalPages::get()
            {
                ULONG u = 0;
                ThrowIf(m_ptr->get_TotalPages(&u));
                return (unsigned int)u;
            }

            unsigned int PrintJob::Status::get()
            {
                PrintJobStatus p;
                ThrowIf(m_ptr->get_Status(&p));
                return (unsigned int)p;
            }

            //Convert to DateTime with from file time convertor (will be off if year is ~ +- 27,000 or more from 2000 ...)
            ULONGLONG PrintJob::SubmissionTime::get()
            {
                DATE d;
                ThrowIf(m_ptr->get_SubmissionTime(&d));
                return (ULONGLONG)d;
            }

            void PrintJob::RequestCancel()
            {
                ThrowIf(m_ptr->RequestCancel());
            }
        }
    }
}

