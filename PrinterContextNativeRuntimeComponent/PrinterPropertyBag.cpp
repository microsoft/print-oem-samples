/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents a printer property bag
*   as described in https://msdn.microsoft.com/library/windows/hardware/hh439547
*/
#include "pch.h"

#include "PrinterProperty.h"
#include "PrinterPropertyBag.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrinterPropertyBag::PrinterPropertyBag(IPrinterPropertyBag* ptr, int type)
            {
                m_printerPropertyBag = ptr;
                m_printerPropertyBagType = type;
            }

            PrinterProperty^ PrinterPropertyBag::GetProperty(String^ name)
            {
                return ref new PrinterProperty(m_printerPropertyBag.Get(), name);
            }

            String^ PrinterPropertyBag::GetString(String^ name)
            {
                //return GetProperty(name)->String;
                return PrinterProperty::GetString(m_printerPropertyBag.Get(), AutoBSTR(name));

            }

            int PrinterPropertyBag::GetInt(String^ name)
            {
                //return GetProperty(name)->Int;
                return PrinterProperty::GetInt(m_printerPropertyBag.Get(), AutoBSTR(name));

            }

            bool PrinterPropertyBag::GetBool(String^ name)
            {
                //return GetProperty(name)->Bool;
                return PrinterProperty::GetBool(m_printerPropertyBag.Get(), AutoBSTR(name));
            }

            void PrinterPropertyBag::SetString(String^ name, String^ value)
            {
                //GetProperty(name)->String = value;
                PrinterProperty::SetString(m_printerPropertyBag.Get(), AutoBSTR(name), value);
            }

            void PrinterPropertyBag::SetBool(String^ name, bool value)
            {
                //GetProperty(name)->Bool = value;
                PrinterProperty::SetBool(m_printerPropertyBag.Get(), AutoBSTR(name), value);
            }
        }
    }
}