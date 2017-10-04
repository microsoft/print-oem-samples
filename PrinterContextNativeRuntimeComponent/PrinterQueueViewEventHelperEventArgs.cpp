/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents print queue view event helper event arguments
*   as described in https://msdn.microsoft.com/library/windows/hardware/dn265393
*/
#include "pch.h"

#include "PrinterQueueViewEventHelperEventArgs.h"

// Namespaces
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrinterQueueViewEventHelperEventArgs::PrinterQueueViewEventHelperEventArgs(int64 printerView,
                                                                                       IVector<int64>^ jobCollection,
                                                                                       int offsetForView,
                                                                                       int sizeForView,
                                                                                       int jobsInPrintQueue)
            {
                if (printerView != 0)
                {
                    IPrinterQueueView* qtmp = (IPrinterQueueView*)(void*)printerView;
                    view = qtmp;
                }

                collection = jobCollection;
                viewOffset = offsetForView;
                viewSize = sizeForView;
                countJobsInPrintQueue = jobsInPrintQueue;
            }

            Windows::Foundation::Collections::IVector<int64>^ PrinterQueueViewEventHelperEventArgs::IPrintJobVector::get()
            {
                return collection;
            }

            int PrinterQueueViewEventHelperEventArgs::ViewOffset::get()
            {
                return viewOffset;
            }

            int PrinterQueueViewEventHelperEventArgs::ViewSize::get()
            {
                return viewSize;
            }

            int PrinterQueueViewEventHelperEventArgs::CountJobsInPrintQueue::get()
            {
                return countJobsInPrintQueue;
            }

            Object^ PrinterQueueViewEventHelperEventArgs::View::get()
            {
                return (Object^)(IntPtr)view.Get();
            }
        }
    }
}

