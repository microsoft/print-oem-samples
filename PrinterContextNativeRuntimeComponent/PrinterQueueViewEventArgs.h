#pragma once
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

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents print queue view event arguments
            public ref class PrinterQueueViewEventArgs sealed
            {
            private:
                Windows::Foundation::Collections::IIterable<PrintJob^>^ m_jobs;
                int m_viewOffset;
                int m_ViewSize;
                int m_CountJobsInPrintQueue;
                Platform::Collections::Vector<PrintJob^>^ jobs;
                ~PrinterQueueViewEventArgs();
            public:
                PrinterQueueViewEventArgs(PrinterQueueViewEventHelperEventArgs^ args);

                property int ViewOffset { int get(); }

                property int ViewSize { int get(); }

                property int CountJobsInPrintQueue { int get(); }
                property Windows::Foundation::Collections::IIterable<PrintJob^>^ Collection { Windows::Foundation::Collections::IIterable<PrintJob^>^ get(); }
            };
        }
    }
}

