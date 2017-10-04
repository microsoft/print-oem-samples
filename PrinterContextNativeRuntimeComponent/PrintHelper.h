#pragma once
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

// Namespaces
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;

namespace PrinterContextNativeRuntimeComponent
{
    namespace Printing
    {
        namespace PrinterExtension
        {
            public enum class InitializationType
            {
                PrinterExtensionContextType = 0,
                PrinterExtensionContextNativeType = 1
            };

            public ref class PrintHelperClass sealed
            {
            public:
                PrintHelperClass(InitializationType initializationType, ULONGLONG ptrToContext);
                static PrintHelperClass^ FromDeviceId(String^ deviceId);

            public:
                event EventHandler<String^>^ OnInkLevelReceived;
                void SendInkLevelQuery();
                Boolean FeatureExists(String^ feature);
                void SetFeatureOption(String^ feature, String^ optionIndex);
                Array<String^>^ GetOptionInfo(String^ feature, String^ infoTypeString);
                String^ GetSelectedOptionIndex(String^ feature);
                String^ GetFeatureDisplayName(String^ feature);
                Array<Boolean>^ GetOptionConstraints(String^ feature);
                String^ GetPrinterName();

            private:
                ~PrintHelperClass();
                ComPtr<IPrintSchemaCapabilities> m_capabilities;
                ComPtr<IPrinterExtensionContext> m_context;
                ComPtr<IPrintWorkflowConfigurationNative> m_contextNative;
                ComPtr<IPrinterQueue> m_printerQueue;
                CoreDispatcher^ m_dispatcher;
                PrinterQueueEventHelper^ m_printQueueEvents;
                ComPtr<IPrintSchemaTicket> m_ticket;
                PrintQueueEventHelperHandler^ m_handler;
                void Initialize();
                Boolean HasCapabilities();
                Boolean HasTicket();
                void OnBidiEvent(Object^ helper, PrinterQueueEventHelperEventArgs^ args);
                Windows::Foundation::EventRegistrationToken m_regToken;
                void GetFeatureOption(String^ feature, IPrintSchemaOptionCollection ** options);
                InitializationType m_initializationType = InitializationType::PrinterExtensionContextType;

            public:
                property PrinterExtensionContext^ Context { PrinterExtensionContext^ get() { return ref new PrinterExtensionContext((ULONGLONG)m_context.Get()); }}
                int32 FromString(String^ string);
            };
        }
    }
}





