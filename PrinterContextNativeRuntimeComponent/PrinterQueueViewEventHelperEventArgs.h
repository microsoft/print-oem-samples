#pragma once
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

// Namespaces
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents print queue view event helper event arguments
            public ref class PrinterQueueViewEventHelperEventArgs sealed
            {
            private:
                IVector<int64>^ collection;
                int viewOffset;
                int viewSize;
                int countJobsInPrintQueue;
                ComPtr<IPrinterQueueView> view;
            public:
                PrinterQueueViewEventHelperEventArgs(int64 printerView, Windows::Foundation::Collections::IVector<int64>^ collection, int viewOffset, int viewSize, int countJobsInPrintQueue);
                property Windows::Foundation::Collections::IVector<int64>^ IPrintJobVector { Windows::Foundation::Collections::IVector<int64>^ get(); }
                property int ViewOffset { int get(); }
                property int ViewSize { int get(); }
                property int CountJobsInPrintQueue { int get(); }
                property Object^ View { Object^ get(); }
            };
        }
    }
}

