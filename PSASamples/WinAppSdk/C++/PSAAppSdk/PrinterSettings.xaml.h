#pragma once

#include "PrinterSettings.g.h"

namespace winrt::VirtualPrinterAppSdk::implementation
{
    struct PrinterSettings : PrinterSettingsT<PrinterSettings>
    {
        PrinterSettings(winrt::Windows::Graphics::Printing::PrintSupport::PrintSupportSettingsActivatedEventArgs const& args);
        
        void InitializeComponent();
        
    private:
        void IntializePrintTicketElements();

        Microsoft::UI::Xaml::Controls::ComboBox CreatePrintTicketFeatureComboBox(winrt::Windows::Graphics::Printing::PrintTicket::PrintTicketFeature const& feature, bool useDefaultEventHandler);

        Microsoft::UI::Xaml::Controls::NumberBox CreatePrintTicketParameterNumberBox(winrt::Windows::Graphics::Printing::PrintTicket::PrintTicketParameterInitializer const& param, bool useDefaultEventHandler);

        void PrintTicketFeatureComboBoxSelectionChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs e);

        void PrintTicketParameterNumberBoxValueChanged(Microsoft::UI::Xaml::Controls::NumberBox numberBox, Microsoft::UI::Xaml::Controls::NumberBoxValueChangedEventArgs e);

        winrt::hstring GetFeatureDisplayName(winrt::Windows::Graphics::Printing::PrintTicket::PrintTicketFeature const& feature);

        winrt::Windows::Graphics::Printing::PrintSupport::PrintSupportSettingsUISession m_uiSession{ nullptr };
        winrt::Windows::Graphics::Printing::PrintTicket::WorkflowPrintTicket m_printTicket{ nullptr };
        winrt::Windows::Graphics::Printing::PrintTicket::PrintTicketCapabilities m_printCapabilities{ nullptr };
        winrt::Windows::Devices::Printers::IppPrintDevice m_printDevice{ nullptr };

        #define PRINT_TICKET_SCHEMA_NAMESPACE L"http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords"
        #define IPP_PRINT_TICKET_SCHEMA_NAMESPACE L"http://schemas.microsoft.com/windows/2018/04/printing/printschemakeywords/Ipp"
        #define IPP_PRINT_TICKET_VP_NAMESPACE L"http://xml.namespaces.adobe.com/enterprise/print/driver/native/printSchemaPrivateKeywords/2024/05"
    };
}

namespace winrt::VirtualPrinterAppSdk::factory_implementation
{
    struct PrinterSettings : PrinterSettingsT<PrinterSettings, implementation::PrinterSettings>
    {
    };
}
