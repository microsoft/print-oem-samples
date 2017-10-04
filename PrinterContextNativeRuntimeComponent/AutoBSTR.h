/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   Simple BSTR handling class
*/
#pragma once

#include "pch.h"

//The following was taken verbatim from R. Chen's sample
/// <summary>
/// RAII-style object that manages the memory allocation and deallocation of a BSTR instance.
/// </summary>
class AutoBSTR
{
public:
    AutoBSTR()
        : string(NULL)
    { }

    AutoBSTR(Platform::String^ str)
    {
        if (str != nullptr)
        {
            this->string = ::SysAllocString(str->Data());
            if (string == NULL)
            {
                throw ref new Platform::OutOfMemoryException("Cannot allocate BSTR");
            }
        }
    }

    ~AutoBSTR()
    {
        if (string)
        {
            ::SysFreeString(string);
            string = NULL;
        }
    }

    BSTR* Get()
    {
        return &string;
    }

    operator BSTR() const
    {
        return string;
    }

private:
    BSTR string;
};

