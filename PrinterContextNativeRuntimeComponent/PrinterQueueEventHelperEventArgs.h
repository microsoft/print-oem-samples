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
#pragma once

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            public ref class PrinterQueueEventHelperEventArgs sealed
            {
            private:
                String^ m_printerQueueEventResponse;
                int m_printerQueueEventHR;
                ComPtr<IPrinterQueue> m_printerQueue;
            public:
                PrinterQueueEventHelperEventArgs(Object^ printerQueue, String^ str, int hresult);
                PrinterQueueEventHelperEventArgs(String^ str, int hresult);
                property String^ Response { String^ get(); }
                property int HResult { int get(); }
                property Object^ Queue { Object^ get(); }
            };
        }
    }
}
