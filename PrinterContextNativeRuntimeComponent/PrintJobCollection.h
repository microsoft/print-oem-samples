#pragma once
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

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a print job collection
            public ref class PrintJobCollection sealed
            {
            private:
                ComPtr<IPrintJobCollection> m_ptr;
            internal:
                PrintJobCollection(IPrintJobCollection* ptr);
            public:
                property int Count { int get(); }
                PrintJob^ GetAt(int index);
            };
        }
    }
}

