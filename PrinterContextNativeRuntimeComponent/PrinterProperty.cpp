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
#include "pch.h"

#include "PrinterProperty.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrinterProperty::PrinterProperty(IPrinterPropertyBag* bag, Platform::String^ printerName)
            {
                m_printerPropertyBag = bag;
                m_printerName = AutoBSTR(printerName);
            }

            bool PrinterProperty::GetBool(IPrinterPropertyBag* bag, BSTR name)
            {
                BOOL b;
                ThrowIf(bag->GetBool(name, &b));
                return b ? true : false;
            }

            void PrinterProperty::SetBool(IPrinterPropertyBag* bag, BSTR name, bool value)
            {
                ThrowIf(bag->SetBool(name, value));
            }
            
            int PrinterProperty::GetInt(IPrinterPropertyBag* bag, BSTR name)
            {
                LONG u;
                ThrowIf(bag->GetInt32(name, &u));
                return u;
            }

            void PrinterProperty::SetInt(IPrinterPropertyBag* bag, BSTR name, int value)
            {
                LONG u = value;
                ThrowIf(bag->SetInt32(name, u));
            }

            String^ PrinterProperty::GetString(IPrinterPropertyBag* bag, BSTR name)
            {
                BSTR u = NULL;
                StringNonZeroCheck(bag->GetString(name, &u));
                Platform::String^ s = ref new Platform::String(u);
                return s;
            }

            void PrinterProperty::SetString(IPrinterPropertyBag* bag, BSTR name, Platform::String^ value)
            {
                AutoBSTR b(value);
                ThrowIf(bag->SetString(name, b));
            }

            Array<byte>^ PrinterProperty::GetBytes(IPrinterPropertyBag* bag, BSTR name)
            {
                DWORD count;
                BYTE* val = NULL;
                StringNonZeroCheck(bag->GetBytes(name, &count, &val));
                return ref new Array<byte, 1>(val, count); //TODO:Test that this is OK

            }

            void PrinterProperty::SetBytes(IPrinterPropertyBag* bag, BSTR name, const Array<byte>^ value)
            {
                ThrowIf(bag->SetBytes(name, value->Length, value->Data));
            }

            IStream* PrinterProperty::GetReadStream(IPrinterPropertyBag* bag, BSTR name)
            {
                IStream* s = NULL;
                ThrowIf(bag->GetReadStream(name, &s));
                return s;
            }

            IStream* PrinterProperty::GetWriteStream(IPrinterPropertyBag* bag, BSTR name)
            {
                IStream* s = NULL;
                ThrowIf(bag->GetWriteStream(name, &s));
                return s;
            }

            // Properties
            bool PrinterProperty::Bool::get()
            {
                return GetBool(m_printerPropertyBag.Get(), m_printerName);
            }
            void PrinterProperty::Bool::set(bool value)
            {
                SetBool(m_printerPropertyBag.Get(), m_printerName, value);
            }

            int PrinterProperty::Int::get()
            {
                return GetInt(m_printerPropertyBag.Get(), m_printerName);
            }
            void PrinterProperty::Int::set(int value)
            {
                SetInt(m_printerPropertyBag.Get(), m_printerName, value);
            }

            Platform::String^ PrinterProperty::String::get()
            {
                return GetString(m_printerPropertyBag.Get(), m_printerName);
            }
            void PrinterProperty::String::set(Platform::String^ value)
            {
                SetString(m_printerPropertyBag.Get(), m_printerName, value);
            }

            Array<byte>^ PrinterProperty::Bytes::get()
            {
                return GetBytes(m_printerPropertyBag.Get(), m_printerName);
            }
            void PrinterProperty::Bytes::set(const Array<byte>^ value)
            {
                SetBytes(m_printerPropertyBag.Get(), m_printerName, value);
            }

            //Actually an IStream* return value
            ULONGLONG PrinterProperty::ReadStream2::get()
            {
                return (ULONGLONG)GetReadStream(m_printerPropertyBag.Get(), m_printerName);
            }

            //Actually an IStream* return value
            ULONGLONG PrinterProperty::WriteStream2::get()
            {
                return (ULONGLONG)GetWriteStream(m_printerPropertyBag.Get(), m_printerName);
            }
        }
    }
}