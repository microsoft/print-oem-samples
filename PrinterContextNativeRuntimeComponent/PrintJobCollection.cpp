/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print job collection
*   as described in https://msdn.microsoft.com/library/windows/hardware/dn265397
*/
#include "pch.h"

#include "PrintJob.h"
#include "PrintJobCollection.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrintJobCollection::PrintJobCollection(IPrintJobCollection* ptr)
            {
                m_ptr = ptr;
            }

            int PrintJobCollection::Count::get()
            {
                ULONG count;
                ThrowIf(m_ptr->get_Count(&count));
                return count;
            }

            PrintJob^ PrintJobCollection::GetAt(int index)
            {
                ULONG u = index;
                ComPtr<IPrintJob> pj;
                StringNonZeroCheck(m_ptr->GetAt(u, pj.GetAddressOf()));
                return ref new PrintJob(pj.Get());
            }
        }
    }
}

