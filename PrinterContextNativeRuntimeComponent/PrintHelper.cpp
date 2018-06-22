/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This class represents print helper class which creates classes for the print queue and other COM objects.
*   It can be initialized with the WSDA context, or with a Print Workflow Native Context to allow a combined sample
*   to handle both WSDA and Print Workflow activation. The accompanying sample .cs code shows how this is done.
*
*   The classes in this runtime component are mostly thin wrappers over the COM objects which enable them to be
*   used from C# on Windows 10 UWP platforms.
*
*   Described in https://msdn.microsoft.com/en-us/library/windows/hardware/hh464103(v=vs.85).aspx
*/
#include "pch.h"

#include "PrintJob.h"
#include "PrinterProperty.h"
#include "PrinterPropertyBag.h"
#include "PrinterQueueEventHelperEventArgs.h"
#include "PrinterQueueViewEventHelperEventArgs.h"
#include "PrinterQueueEventHelperCallback.h"
#include "PrinterPrintJobEventHandler.h"
#include "PrinterPrintQueueEventHandlerHelper.h"
#include "PrinterQueueEventHelper.h"
#include "PrinterQueueViewEventArgs.h"
#include "PrinterQueueView.h"
#include "PrinterQueue2.h"
#include "PrinterQueueBidiSetRequestTask.h"
#include "PrintSchemaOption.h"
#include "PrintSchemaFeature.h"
#include "PrintSchemaParameterInitializer.h"
#include "PrintSchemaAsyncOperationCallback.h"
#include "PrintSchemaAsyncOp.h"
#include "PrintSchemaTicket.h"
#include "PrinterExtensionContext.h"
#include "PrintHelper.h"

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {

            /*  PrintHelperClass::PrintHelperClass
            *
            *   Constructor
            *
            *   This class represents print helper class which creates classes for the print queue and other COM objects.
            *   It can be constructed from the WSDA context, or from a Print Workflow Native Context to allow a combined sample
            *   to handle both WSDA and Print Workflow activation.
            *
            *  Arguments:
            *          InitializationType initializationType   this can be PrinterExtensionContextType from WSDA activation or
            *                                                  PrinterExtensionContextNativeType from Print Workflow activation
            *
            *          ULONGLONG ptrToContext                  An opaque type representing either the WSDA or Print Workflow context object
            *
            */
            PrintHelperClass::PrintHelperClass(InitializationType initializationType, ULONGLONG ptrToContext)
            {
                m_initializationType = initializationType;

                if (m_initializationType == InitializationType::PrinterExtensionContextType)
                {
                    m_context = reinterpret_cast<IPrinterExtensionContext*>(ptrToContext);
                }
                else
                {
                    m_contextNative = reinterpret_cast<IPrintWorkflowConfigurationNative*>(ptrToContext);
                }

                Initialize();
            }

            /*  PrintHelperClass::FromDeviceId
            *
            *  Creates a PrintHelperClass for the printer with the specified device ID
            *
            *  Arguments:
            *       String ^ deviceId    The device hardware ID of the printer
            *
            *  Returns:
            *       A PrintHelperClass for the printer with the specified device ID
            */
            PrintHelperClass^ PrintHelperClass::FromDeviceId(String ^ deviceId)
            {
                wchar_t activatableClassIdName[] = L"Windows.Devices.Printers.Extensions.PrintExtensionContext";
                HSTRING activatableClassIdHSTRING;
                ThrowIf(WindowsCreateString(activatableClassIdName, _countof(activatableClassIdName) - 1, &activatableClassIdHSTRING));
                ComPtr<ABI::Windows::Devices::Printers::Extensions::IPrintExtensionContextStatic> printExtensionContextStatic;
                HRESULT hr = RoGetActivationFactory(activatableClassIdHSTRING, ABI::Windows::Devices::Printers::Extensions::IID_IPrintExtensionContextStatic, reinterpret_cast<void**>(printExtensionContextStatic.GetAddressOf()));
                WindowsDeleteString(activatableClassIdHSTRING);
                ThrowIf(hr);
                HSTRING deviceInterfaceId;
                ThrowIf(WindowsCreateString(deviceId->Data(), deviceId->Length(), &deviceInterfaceId));
                ComPtr<IInspectable> inspectableInterface;
                hr = printExtensionContextStatic->FromDeviceId(deviceInterfaceId, inspectableInterface.GetAddressOf());
                WindowsDeleteString(deviceInterfaceId);
                ThrowIf(hr);
                ComPtr<IPrinterExtensionContext> printerExtensionContext;
                ThrowIf(inspectableInterface.As(&printerExtensionContext));
                return ref new PrintHelperClass(InitializationType::PrinterExtensionContextType, (ULONGLONG)printerExtensionContext.Get());
            }

            /*  PrintHelperClass::~PrintHelperClass
            *
            *   Destructor
            */
            PrintHelperClass::~PrintHelperClass()
            {
                if (m_printQueueEvents)
                {
                    m_printQueueEvents->RemoveHandler(m_regToken);
                }
            }

            /*  PrintHelperClass::SendInkLevelQuery
            *
            *  Sends standard query string to retrieve the ink level, and raise the onInkLevelReceived event when data is available.
            *
            *  Arguments:
            *     None
            *
            *  Returns:
            *     Nothing
            */
            void PrintHelperClass::SendInkLevelQuery()
            {
                AutoBSTR queryString(L"\\Printer.Consumables");
                ThrowIf(m_printerQueue->SendBidiQuery(queryString));
            }

            /*  PrintHelperClass::FeatureExists
            *
            *  Determines whether the print capabilities contain the specified feature.
            *
            *  Arguments:
            *          String ^ feature   The feature to search the capabilities for
            *
            *  Returns:
            *           True if the capabilities and the ticket contain the specified feature, False if the feature was not found
            */
            Boolean PrintHelperClass::FeatureExists(String ^ feature)
            {
                if (feature == nullptr || feature->IsEmpty()) return false;
                if (!(HasTicket() && HasCapabilities())) return false;
                AutoBSTR bFeature(feature);
                ComPtr<IPrintSchemaFeature> ticketFeature;
                ComPtr<IPrintSchemaFeature> capFeature;
                if (FAILED((m_ticket->GetFeatureByKeyName(bFeature, ticketFeature.GetAddressOf())))) return false;
                if (FAILED((m_capabilities->GetFeatureByKeyName(bFeature, capFeature.GetAddressOf())))) return false;

                if (ticketFeature && capFeature)
                    return true;
                return false;
            }

            /*  PrintHelperClass::FeatureExists
            *
            *  Set a specified feature's selected option to the specified option in the print ticket
            *
            *  Arguments:
            *     String ^ feature             The feature whose option will be set
            *     String ^ optionIndex         The index of the option that will be selected in the list of options retrieved for the specified feature
            *  Returns:
            *     Nothing
            */
            void PrintHelperClass::SetFeatureOption(String ^ feature, String ^ optionIndex)
            {
                if (feature == nullptr || feature->IsEmpty()) throw ref new InvalidArgumentException();
                if (optionIndex == nullptr || optionIndex->IsEmpty()) throw ref new InvalidArgumentException();
                int32 index = PrintHelperClass::FromString(optionIndex);
                AutoBSTR bFeature(feature);
                ComPtr<IPrintSchemaFeature> ticketFeature;
                ThrowIf(m_ticket->GetFeatureByKeyName(bFeature, ticketFeature.GetAddressOf()));
                if (!ticketFeature.Get()) ThrowIf(E_UNEXPECTED);
                String^ selectedIndex = GetSelectedOptionIndex(feature);
                int32 sIndex = PrintHelperClass::FromString(selectedIndex);
                if (index != sIndex)
                {
                    ComPtr<IPrintSchemaOptionCollection> options;
                    GetFeatureOption(feature, options.GetAddressOf());
                    ComPtr<IPrintSchemaOption> option;
                    ThrowIf(options->GetAt(index, option.GetAddressOf()));
                    if (!option.Get()) ThrowIf(E_UNEXPECTED);
                    ThrowIf(ticketFeature->get_SelectedOption(option.GetAddressOf()));
                    if (!option.Get()) ThrowIf(E_UNEXPECTED);
                }
            }

            /*  PrintHelperClass::GetOptionInfo
            *
            *  Get a specified feature's selected option information
            *
            *  Arguments:
            *     String ^ feature             The feature whose option information will be retrieved
            *     String ^ infoTypeString      DisplayName or Index of the feature
            *  Returns:
            *     Array<String^>^              Requested information about the requested feature option
            */
            Array<String^>^ PrintHelperClass::GetOptionInfo(String ^ feature, String ^ infoTypeString)
            {
                if (feature == nullptr || feature->IsEmpty()) throw ref new InvalidArgumentException();
                if (infoTypeString == nullptr || infoTypeString->IsEmpty()) throw ref new InvalidArgumentException();
                int enumIndex = -1;
                if (infoTypeString->Equals(L"DisplayName"))
                {
                    enumIndex = 0;
                }
                else if (infoTypeString->Equals("Index"))
                {
                    enumIndex = 1;
                }
                if (enumIndex < 0)  throw ref new InvalidArgumentException();
                ComPtr<IPrintSchemaOptionCollection> options;
                GetFeatureOption(feature, options.GetAddressOf());
                ULONG vectorSize;
                ThrowIf(options->get_Count(&vectorSize));

                Array<String^>^ results = ref new Array<String^>(vectorSize);
                if (enumIndex == 0)
                {
                    for (ULONG i = 0; i < vectorSize; i++)
                    {
                        ComPtr<IPrintSchemaOption> option;
                        ThrowIf(options->GetAt(i, option.GetAddressOf()));
                        BSTR b = NULL;
                        ThrowIf(option->get_DisplayName(&b));
                        results->set(i, ref new String(b));
                    }
                }
                else
                {
                    for (ULONG i = 0; i < vectorSize; i++)
                    {
                        results->set(i, i.ToString());
                    }
                }
                return results;
            }

            /*  PrintHelperClass::GetSelectedOptionIndex
            *
            *  Gets the index of the currently selected option in the list of options for a specified feature in the current print ticket
            *
            *  Arguments:
            *     String ^ feature             The feature whose currently selected option will be looked up
            *
            *  Returns:
            *     String ^                     The currently selected option index of the feature
            */
            String ^ PrintHelperClass::GetSelectedOptionIndex(String ^ feature)
            {
                if (feature == nullptr || feature->IsEmpty()) throw ref new InvalidArgumentException();
                ComPtr<IPrintSchemaOptionCollection> options;
                GetFeatureOption(feature, options.GetAddressOf());
                if (!options.Get()) ThrowIf(E_UNEXPECTED);
                ULONG vectorSize;
                ThrowIf(options->get_Count(&vectorSize));
                for (ULONG i = 0; i < vectorSize; i++)
                {
                    ComPtr<IPrintSchemaOption> option;
                    ThrowIf(options->GetAt(i, option.GetAddressOf()));
                    if (!option.Get()) ThrowIf(E_UNEXPECTED);
                    BOOL selected;
                    ThrowIf(option->get_Selected(&selected));
                    if (selected)
                    {
                        return i.ToString();
                    }
                }

                // It is possible for the PrintTicket object to not contain a current selection for this feature causing none
                // of the options in the print capabilities to be marked as selected.  In this case, the developers should
                // be familiar enough with the printer hardware to display and set the feature to the correct printer default option.
                // Because this is a generic sample app, the first option will be displayed and set when the user hits the back button.
                return "0";
            }

            /*  PrintHelperClass::GetFeatureDisplayName
            *
            *  Gets the index of the currently selected option in the list of options for a specified feature in the current print ticket
            *
            *  Arguments:
            *     String ^ feature             The feature whose currently selected option will be looked up
            *
            *  Returns:
            *     String ^                     The display name of the feature
            */
            String ^ PrintHelperClass::GetFeatureDisplayName(String ^ feature)
            {
                if (feature == nullptr || feature->IsEmpty()) throw ref new InvalidArgumentException();
                if (!HasCapabilities()) throw ref new Exception(E_FAIL);
                ComPtr<IPrintSchemaFeature> f;
                AutoBSTR a(feature);
                if (SUCCEEDED(m_capabilities->GetFeatureByKeyName(a, f.GetAddressOf())) && f.Get())
                {
                    BSTR b = NULL;
                    StringNonZeroCheck(f->get_DisplayName(&b));
                    String^ returnValue = ref new String(b);
                    return returnValue;
                }
                return String::Concat(String::Concat(L"Feature \"", feature), "\" Display Name was not found in the print capabilities");
            }

            /*  PrintHelperClass::GetOptionConstraints
            *
            *  Gets the constraints the driver publishers have set for the feature
            *  as described in https://docs.microsoft.com/en-us/windows-hardware/drivers/print/javascript-constraints
            *
            *  Arguments:
            *     String ^ feature             The feature for which constrainst will be looked up
            *
            *  Returns:
            *     Array<Boolean>^              Array of constraints the driver publishers have set for the feature
            */
            Array<Boolean>^ PrintHelperClass::GetOptionConstraints(String ^ feature)
            {
                if (feature == nullptr || feature->IsEmpty()) throw ref new InvalidArgumentException();
                ComPtr<IPrintSchemaOptionCollection> options;
                GetFeatureOption(feature, options.GetAddressOf());
                if (!options.Get()) ThrowIf(E_UNEXPECTED);
                ULONG vectorSize;
                ThrowIf(options->get_Count(&vectorSize));
                Array<Boolean>^ ret = ref new Array<Boolean>(vectorSize);
                for (ULONG i = 0; i < vectorSize; i++)
                {
                    ComPtr<IPrintSchemaOption> option;
                    ThrowIf(options->GetAt(i, option.GetAddressOf()));
                    if (!option.Get()) ThrowIf(E_UNEXPECTED);
                    PrintSchemaConstrainedSetting setting;
                    ThrowIf(option->get_Constrained(&setting));
                    ret->set(i, setting != PrintSchemaConstrainedSetting_None);
                }
                return ret;
            }

            /* PrintHelperClass::GetPrinterName
            *
            *  Get the printer name of the device for which the helper class was constructed
            *
            *  Arguments:
            *       None
            *
            *  Returns:
            *       String^    Name of the printer for which the helper class was constructed, or an empty string if it could not be obtained
            */
            String^ PrintHelperClass::GetPrinterName()
            {
                Platform::String^ printerNameString = "";
                BSTR printerName = NULL;
                HRESULT hr = m_printerQueue->get_Name(&printerName);
                if (SUCCEEDED(hr))
                {
                    printerNameString = ref new Platform::String(printerName);
                }

                return printerNameString;
            }

            /*  PrintHelperClass::Initialize
            *
            *  Internal function called from the constructor
            *
            *  Arguments:
            *     None
            *
            *  Returns:
            *     Nothing
            */
            void PrintHelperClass::Initialize()
            {
                HRESULT hr = S_OK;
                if (m_initializationType == InitializationType::PrinterExtensionContextType)
                {
                    ThrowIf(m_context->get_PrinterQueue(m_printerQueue.GetAddressOf()));
                }
                else
                {
                    hr = m_contextNative->get_PrinterQueue(m_printerQueue.GetAddressOf());
                }

                // Add the BiDi query response event handler
                if (SUCCEEDED(hr))
                {
                    m_printQueueEvents = ref new PrinterQueueEventHelper(m_printerQueue.Get());
                    PrintQueueEventHelperHandler^ p = ref new PrintQueueEventHelperHandler(this, &PrintHelperClass::OnBidiEvent);
                    m_regToken = m_printQueueEvents->AddHandler(p);
                }
            }

            /*  PrintHelperClass::HasCapabilities
            *
            *  Internal function called to determine if we can get capabilities for the printer
            *
            *  Arguments:
            *     None
            *
            *  Returns:
            *     Boolean     True if the printer has capabilities it can return
            */
            Boolean PrintHelperClass::HasCapabilities()
            {
                if (m_capabilities)
                {
                    return true;
                }

                if (!HasTicket()) return false;

                ThrowIf(m_ticket->GetCapabilities(m_capabilities.GetAddressOf()));
                if (m_capabilities) return true;

                return false;
            }

            /*  PrintHelperClass::HasTicket
            *
            *  Internal function called to determine if we can get a print schema ticket for the print job
            *
            *  Arguments:
            *     None
            *
            *  Returns:
            *     Boolean     True if there is a print ticket we can return
            */
            Boolean PrintHelperClass::HasTicket()
            {
                if (m_ticket)
                {
                    return true;
                }

                // If the PrinterExtensionContextType is PrinterExtensionContextNativeType then the ticket is unavailable through here
                // and the developer should be using the WinRT Print Ticket library
                if (!m_context || m_initializationType == InitializationType::PrinterExtensionContextNativeType)
                {
                    return false;
                }

                ThrowIf(m_context->get_PrintSchemaTicket(m_ticket.GetAddressOf()));
                if (m_ticket)
                {
                    return true;
                }

                return false;
            }

            /*  PrintHelperClass::OnBidiEvent
            *
            *  Internal function to invoke handlers, if any, for BiDi events
            *
            *  Arguments:
            *     Object ^ helper                              Helper object
            *     PrinterQueueEventHelperEventArgs ^ args      Event arguments
            *
            *  Returns:
            *     Nothing
            */
            void PrintHelperClass::OnBidiEvent(Object ^ helper, PrinterQueueEventHelperEventArgs ^ args)
            {
                OnInkLevelReceived(this, args->Response);
            }

            void PrintHelperClass::GetFeatureOption(String ^ feature, IPrintSchemaOptionCollection ** options)
            {
                if (feature == nullptr || options == nullptr || feature->IsEmpty() || !HasCapabilities()) throw ref new Exception(E_INVALIDARG);
                ComPtr<IPrintSchemaFeature> f;
                ComPtr<IPrintSchemaOptionCollection> oc;
                AutoBSTR b(feature);
                ThrowIf(m_capabilities->GetFeatureByKeyName(b, f.GetAddressOf()));
                ThrowIf(m_capabilities->GetOptions(f.Get(), oc.GetAddressOf()));
                ThrowIf(oc.CopyTo(options));
            }

            /*  PrintHelperClass::FromString
            *
            *  Internal function to convert a String^ to an int32 value
            *
            *  Arguments:
            *       String ^ string    Numeric representation of an int32 value, e.g. "1234"
            *
            *  Returns:
            *       A PrintHelperClass for the printer with the specified device ID
            */
            int32 PrintHelperClass::FromString(String^ string)
            {
                // Use method described in https://blogs.msdn.microsoft.com/devschool/2012/10/09/a-clear-and-simple-way-to-convert-a-string-to-integer-in-ccx/
                if (!string || string->IsEmpty()) throw ref new InvalidArgumentException();
                // Create wstring
                std::wstring ws(string->Data());
                // Create a wstringstream
                std::wstringstream convertor;
                // Create an integer
                int ws_int;
                // Put the string into the converter
                convertor << ws;
                // Get the integer out
                convertor >> ws_int;
                return ws_int;
            }
        }
    }
}