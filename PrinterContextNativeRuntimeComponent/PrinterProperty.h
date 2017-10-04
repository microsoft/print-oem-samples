#pragma once
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a specific printer property
*   as described in https://msdn.microsoft.com/library/windows/hardware/hh439547
*/

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            // This class represents a specific printer property
            public ref class PrinterProperty sealed
            {
            private:
                ComPtr<IPrinterPropertyBag> m_printerPropertyBag;
                AutoBSTR m_printerName;

            internal:
                PrinterProperty(IPrinterPropertyBag* bag, String^ name);
                static bool GetBool(IPrinterPropertyBag* bag, BSTR name);
                static void SetBool(IPrinterPropertyBag* bag, BSTR name, bool value);
                static int GetInt(IPrinterPropertyBag* bag, BSTR name);
                static void SetInt(IPrinterPropertyBag* bag, BSTR name, int value);
                static String^ GetString(IPrinterPropertyBag* bag, BSTR name);
                static void SetString(IPrinterPropertyBag* bag, BSTR name, String^ value);
                static Array<byte>^ GetBytes(IPrinterPropertyBag* bag, BSTR name);
                static void SetBytes(IPrinterPropertyBag* bag, BSTR name, const Array<byte>^ value);
                static IStream* GetReadStream(IPrinterPropertyBag* bag, BSTR name);
                static IStream* GetWriteStream(IPrinterPropertyBag* bag, BSTR name);
            public:
                // Properties
                property bool Bool { bool get(); void set(bool); }
                property int Int { int get(); void set(int); }
                property String^ String { Platform::String^ get(); void set(Platform::String^); }
                property Array<byte>^ Bytes { Array<byte>^ get(); void set(const Array<byte>^); }
                //Actually an IStream* return value
                property ULONGLONG ReadStream2 { ULONGLONG get(); }
                //Actually an IStream* return value
                property ULONGLONG WriteStream2 { ULONGLONG get(); }
            };
        };
    };
};
