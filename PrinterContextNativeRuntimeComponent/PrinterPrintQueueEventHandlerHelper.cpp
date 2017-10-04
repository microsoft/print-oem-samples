/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a print queue event handler helper
*   as described in https://msdn.microsoft.com/library/windows/hardware/dn265393
*/
#include "pch.h"

#include "PrinterQueueViewEventHelperEventArgs.h"
#include "PrinterPrintJobEventHandler.h"
#include "PrinterPrintQueueEventHandlerHelper.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrinterPrintQueueEventHandlerHelper::PrinterPrintQueueEventHandlerHelper(int64 iPrintQueueView, Object^ notUsed)
            {
                IUnknown* pqview = (IUnknown*)(void*)iPrintQueueView;
                if (pqview == NULL)
                {
                    throw ref new InvalidArgumentException();
                }
                int hr = pqview->QueryInterface(__uuidof(IPrinterQueueView), (void**)view.GetAddressOf());
                __abi_ThrowIfFailed(hr);
                Windows::Foundation::TypedEventHandler<Object^, PrinterQueueViewEventHelperEventArgs^>^ p = ref new Windows::Foundation::TypedEventHandler<Object^, PrinterQueueViewEventHelperEventArgs^>(this, &PrinterPrintQueueEventHandlerHelper::_onChange);
                handler = Make<PrinterPrintJobEventHandler>((int64)(void*)view.Get(), p);
            }

            PrinterPrintQueueEventHandlerHelper::~PrinterPrintQueueEventHandlerHelper()
            {
                view = nullptr;
                handler = nullptr;
            }

            void PrinterPrintQueueEventHandlerHelper::_onChange(Object^ notUsed, PrinterQueueViewEventHelperEventArgs^ args)
            {
                OnChange(this, args);
            }
        }
    }
}

