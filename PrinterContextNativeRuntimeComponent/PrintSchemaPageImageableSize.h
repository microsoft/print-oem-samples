#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents the imageable size of the media
*   as described in https://msdn.microsoft.com/en-us/library/windows/hardware/hh451366(v=vs.85).aspx
*/

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents the imageable size of the media
            public ref class PrintSchemaPageImageableSize sealed
            {
            private:
                ComPtr<IPrintSchemaPageImageableSize> m_ptr;
            internal:
                PrintSchemaPageImageableSize(IPrintSchemaPageImageableSize* ptr);
            public:
                property String^ Xml { String^ get(); }
                property String^ Name { String^ get(); }
                property String^ NamespaceUri { String^ get(); }
                property unsigned int ExtentHeightInMicrons { unsigned int get(); }
                property unsigned int ExtentWidthInMicrons { unsigned int get(); }
                property unsigned int ImageableSizeHeightInMicrons { unsigned int get(); }
                property unsigned int ImageableSizeWidthInMicrons { unsigned int get(); }
                property unsigned int OriginHeightInMicrons { unsigned int get(); }
                property unsigned int OriginWidthInMicrons { unsigned int get(); }
            };
        }
    }
}

