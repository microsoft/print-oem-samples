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
#include "pch.h"

#include "PrintSchemaElement.h"
#include "PrintSchemaPageImageableSize.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            PrintSchemaPageImageableSize::PrintSchemaPageImageableSize(IPrintSchemaPageImageableSize* ptr)
            {
                m_ptr = ptr;
            }

            String^ PrintSchemaPageImageableSize::Xml::get()
            {
                ComPtr<IPrintSchemaElement> ptr;
                StringNonZeroCheck(m_ptr.As(&ptr));
                return PrintSchemaElement::GetXmlStreamForElement((ULONGLONG)ptr.Get());

            }

            String^ PrintSchemaPageImageableSize::Name::get()
            {
                BSTR n = NULL;
                StringNonZeroCheck(m_ptr->get_Name(&n));
                String^ s = ref new String(n);
                return s;
            }

            String^ PrintSchemaPageImageableSize::NamespaceUri::get()
            {
                BSTR n = NULL;
                StringNonZeroCheck(m_ptr->get_NamespaceUri(&n));
                String^ s = ref new String(n);
                return s;
            }

            unsigned int PrintSchemaPageImageableSize::ExtentHeightInMicrons::get()
            {
                DWORD d = 0;
                ThrowIf(m_ptr->get_ExtentHeightInMicrons(&d));
                return d;
            }

            unsigned int PrintSchemaPageImageableSize::ExtentWidthInMicrons::get()
            {
                DWORD d = 0;
                ThrowIf(m_ptr->get_ExtentWidthInMicrons(&d));
                return d;
            }

            unsigned int PrintSchemaPageImageableSize::ImageableSizeHeightInMicrons::get()
            {
                DWORD d = 0;
                ThrowIf(m_ptr->get_ImageableSizeHeightInMicrons(&d));
                return d;
            }

            unsigned int PrintSchemaPageImageableSize::ImageableSizeWidthInMicrons::get()
            {
                DWORD d = 0;
                ThrowIf(m_ptr->get_ImageableSizeWidthInMicrons(&d));
                return d;
            }

            unsigned int PrintSchemaPageImageableSize::OriginHeightInMicrons::get()
            {
                DWORD d = 0;
                ThrowIf(m_ptr->get_OriginHeightInMicrons(&d));
                return d;
            }

            unsigned int PrintSchemaPageImageableSize::OriginWidthInMicrons::get()
            {
                DWORD d = 0;
                ThrowIf(m_ptr->get_OriginWidthInMicrons(&d));
                return d;
            }
        }
    }
}

