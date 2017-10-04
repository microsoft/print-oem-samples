#pragma once
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

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a print schema asynchronous callback operation 
            public ref class PrintSchemaAsyncOp sealed
            {
            private:
                ComPtr<PrintSchemaAsyncOperationCallback> helper;
                HANDLE m_asyncOperationEvent;
                ULONGLONG m_printTicket;
                HRESULT m_asyncOperationHR;
                // Destructor
                ~PrintSchemaAsyncOp();
            internal:
                void Initialize(IPrintSchemaAsyncOperation* op);
                PrintSchemaAsyncOp(IPrintSchemaAsyncOperation* op);
                void ManagedPPrintSchemaAsyncOperationCallback(ULONGLONG ticket, int hr);
            public:
                // Constructor
                PrintSchemaAsyncOp(ULONGLONG op);
                // Wait functions
                void Wait();
                bool Wait(int spanInMS);
                // Properties
                // Will block waiting on the callback if needed
                property ULONGLONG Ticket
                {
                    ULONGLONG get()
                    {
                        WaitForSingleObject(m_asyncOperationEvent, -1);
                        return m_printTicket;
                    }
                }
                //Will block waiting on the callback if needed
                property int Hresult
                {
                    int get()
                    {
                        WaitForSingleObject(m_asyncOperationEvent, -1);
                        return m_asyncOperationHR;
                    }
                }
            };
        }
    }
}
