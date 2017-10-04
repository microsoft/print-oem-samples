/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class handles calling the BiDi request callback handlers that have been added, if any
*   as described in https://msdn.microsoft.com/library/windows/hardware/dn265385
*/
#include "pch.h"

#include "PrinterQueueEventHelperEventArgs.h"
#include "BidiSetRequestCallback.h"

// Namespaces
using namespace Windows::Foundation;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            BidiSetRequestCallback::BidiSetRequestCallback(Windows::Foundation::TypedEventHandler<Object^, PrinterQueueEventHelperEventArgs^>^ func)
            {
                callback = func;
            }

            BidiSetRequestCallback::~BidiSetRequestCallback()
            {
            }

            /// <summary>
            /// This method is invoked when the asynchronous Bidi "Set" operation is completed.
            /// </summary>
            HRESULT STDMETHODCALLTYPE BidiSetRequestCallback::Completed(_In_ BSTR bstrResponse, HRESULT hrStatus)
            {
                HRESULT hr = S_OK;

                // This method must not throw any exceptions because it represents a COM object boundary.
                try
                {
                    if (callback != nullptr)
                    {

                        String^ result = ref new String(bstrResponse);
                        callback(nullptr, ref new PrinterQueueEventHelperEventArgs(result, hrStatus));
                    }
                }
                catch (Platform::Exception^ e)
                {
                    hr = e->HResult;
                }
                catch (...)
                {
                    hr = E_FAIL;
                }

                return hr;
            }
        }
    }
}

