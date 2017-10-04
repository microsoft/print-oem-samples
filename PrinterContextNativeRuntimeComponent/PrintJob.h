#pragma once
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

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a print job
            public ref class PrintJob sealed
            {
            private:
                ComPtr<IPrintJob> m_ptr;
            internal:
                PrintJob(IPrintJob* ptr);
            public:
                property String^ Name { String^ get(); }
                property unsigned int Id { unsigned int get(); }
                property unsigned int PrintedPages { unsigned int get(); }
                property unsigned int TotalPages { unsigned int get(); }
                property unsigned int Status { unsigned int get(); }
                //Convert to DateTime with from file time convertor (will be off if year is ~ +- 27,000 or more from 2000 ...)
                property ULONGLONG SubmissionTime { ULONGLONG get(); }
                void RequestCancel();
            };
        }
    }
}

