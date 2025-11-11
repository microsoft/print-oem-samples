#include "pch.h"
#include "PrinterSettings.xaml.h"
#if __has_include("PrinterSettings.g.cpp")
#include "PrinterSettings.g.cpp"
#endif
#include <winrt\Windows.Devices.Printers.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace winrt::Windows::Graphics::Printing::PrintTicket;
using namespace winrt::Windows::Graphics::Printing::PrintSupport;
using namespace Windows::Data::Xml::Dom;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::VirtualPrinterAppSdk::implementation
{
    PrinterSettings::PrinterSettings(winrt::Windows::Graphics::Printing::PrintSupport::PrintSupportSettingsActivatedEventArgs const& args)
    {
        m_uiSession = args.Session();
        m_printDevice = m_uiSession.SessionInfo().Printer();
    }

    void PrinterSettings::InitializeComponent()
    {
        PrinterSettingsT::InitializeComponent();
        IntializePrintTicketElements();
        OutputDebugString(L"PrinterSettings::OnLoaded called");
    }

    void PrinterSettings::IntializePrintTicketElements()
    {
        // WorkflowPrintTicket is the currently set features and properties for the print job.
        m_printTicket = m_uiSession.SessionPrintTicket();
        // PrintTicketCapabilities is all of the possible features and properties for the target printer.
        m_printCapabilities = m_printTicket.GetCapabilities();
        auto pdcString =  m_printCapabilities.XmlNode().GetXml();
        auto ptString = m_printTicket.XmlNode().GetXml();

        OutputDebugString(L"Setting output");
        OutputDebugString(pdcString.c_str());
        OutputDebugString(ptString.c_str());

        // A list of Controls that will be rendered in the UI.
        auto printTicketControls = winrt::single_threaded_observable_vector<Control>();

        //
        // Example 1.1 Basic print ticket features.
        //

        // Create a list of print ticket features.
        // Each feature is a device capability that has an enumerable list of settings.
        auto printTicketFeatures{ winrt::single_threaded_observable_vector<PrintTicketFeature>(
            {
                // Add features defined in m_printTicket properties.
                // These are wrappers for commonly used features.
                m_printTicket.DocumentBindingFeature(),
                m_printTicket.DocumentDuplexFeature(),
                m_printTicket.DocumentHolePunchFeature(),
                m_printTicket.DocumentInputBinFeature(),
                m_printTicket.DocumentNUpFeature(),
                m_printTicket.DocumentStapleFeature(),
                m_printTicket.JobPasscodeFeature(),
                m_printTicket.PageMediaSizeFeature(),
                m_printTicket.PageMediaTypeFeature(),
                m_printTicket.PageOrientationFeature(),
                m_printTicket.PageOutputColorFeature(),
                m_printTicket.PageOutputQualityFeature(),
                m_printTicket.PageResolutionFeature(),
                // Add other print ticket features, not defined in m_printTicket properties.
                m_printTicket.GetFeature(L"JobNUpAllDocumentsContiguously", PRINT_TICKET_SCHEMA_NAMESPACE),
                m_printTicket.GetFeature(L"JobOutputBin", PRINT_TICKET_SCHEMA_NAMESPACE),
                m_printTicket.GetFeature(L"JobPageOrder", PRINT_TICKET_SCHEMA_NAMESPACE),
                m_printTicket.GetFeature(L"PageBorderless", PRINT_TICKET_SCHEMA_NAMESPACE),
                // Add features from a different XML namespace.
                m_printTicket.GetFeature(L"JobNUpPresentationDirection", IPP_PRINT_TICKET_SCHEMA_NAMESPACE),
                m_printTicket.GetFeature(L"JobIdentification", IPP_PRINT_TICKET_VP_NAMESPACE),
            }
        ) };

        for (auto const& feature : printTicketFeatures)
        {
            // If the printer does not support the feature, it will be null.
            if (feature != nullptr)
            {
                printTicketControls.Append(CreatePrintTicketFeatureComboBox(feature, true));
            }
        }

        //
        // Example 1.2 Inter-dependencies between print ticket controls.
        // Scenario: The collateComboBox is enabled only when the copiesNumberBox has a value greater than 1.
        //

        auto collateFeature = m_printTicket.DocumentCollateFeature();
        auto copiesParameter = m_printTicket.GetParameterInitializer(L"JobCopiesAllDocuments", PRINT_TICKET_SCHEMA_NAMESPACE);

        auto collateComboBox = CreatePrintTicketFeatureComboBox(collateFeature, true);
        auto copiesNumberBox = CreatePrintTicketParameterNumberBox(copiesParameter, false);
        if (collateComboBox != nullptr && copiesNumberBox != nullptr)
        {
            // Only enable the collate feature when copies count is greater than 1.
            collateComboBox.IsEnabled(copiesNumberBox.Value() > 1);
            // Add a custom event handler to conditionally enable the collateComboBox.

            copiesNumberBox.ValueChanged(
                [this, collateComboBox](NumberBox numberBox, NumberBoxValueChangedEventArgs eventArgs)
                {
                    collateComboBox.IsEnabled(numberBox.Value() > 1);
                    PrintTicketParameterNumberBoxValueChanged(numberBox, eventArgs);
                });

            printTicketControls.Append(collateComboBox);
            printTicketControls.Append(copiesNumberBox);
        }
        else if (collateComboBox != nullptr)
        {
            // The print ticket does not have the JobCopiesAllDocuments parameter.
            // Don't need to conditionally enable the collateComboBox.
            printTicketControls.Append(collateComboBox);
        }
        else if (copiesNumberBox != nullptr)
        {
            // The print ticket does not have the DocumentCollateFeature.
            // Don't need to conditionally enable the collateComboBox.
            // Use the default value changed event handler.
            copiesNumberBox.ValueChanged({ this, &PrinterSettings::PrintTicketParameterNumberBoxValueChanged });
            printTicketControls.Append(copiesNumberBox);
        }

        // Add all the controls to be rendered in the UI.
        PrintTicketFeatureComboBoxRepeater().ItemsSource(printTicketControls);
    }

    winrt::hstring PrinterSettings::GetFeatureDisplayName(PrintTicketFeature const& feature)
    {
        winrt::hstring dispalyName;
        // try catch is needed if get over display name bug in the API.
        // should be removed after API bug is fixed.
        try
        {
            dispalyName = feature.DisplayName();
        }
        catch (...)
        {
            dispalyName = feature.Name();
        }

        return dispalyName;
    }

    ComboBox PrinterSettings::CreatePrintTicketFeatureComboBox(PrintTicketFeature const& feature, bool useDefaultEventHandler)
    {
        if (feature == nullptr)
        {
            return nullptr;
        }
        auto displayName = GetFeatureDisplayName(feature);

        ComboBox comboBox = ComboBox();
        comboBox.Header(winrt::box_value(displayName));

        // Construct a new List since IReadOnlyList does not support the 'IndexOf' method.
        auto options = winrt::single_threaded_observable_vector<PrintTicketOption>();
        // Provide the combo box with a list of options to select from.
        for (auto const& option : feature.Options())
        {
            options.Append(option);
        }

        comboBox.ItemsSource(options);

        // Set the selected option to the option set in the print ticket.
        auto featureOption = feature.GetSelectedOption();

        uint32_t index = MAXUINT32; // IndexOf() protects against unsigned overflow

        PrintTicketOption selectionOption{ nullptr };
        for (auto const& option : options)
        {
            if (option.Name() == featureOption.Name() && option.XmlNamespace() == featureOption.XmlNamespace() && option.DisplayName() == featureOption.DisplayName())
            {
                selectionOption = option;
                break;
            }
        }
        if (!selectionOption)
        {
            throw winrt::hresult_invalid_argument();
        }

        if (options.IndexOf(selectionOption, index) && index != MAXUINT32)
        {
            comboBox.SelectedIndex(index);
        }

        // Disable the combo box if there is only one selection.
        if (options.Size() == 1)
        {
            comboBox.IsEnabled(false);
        }
        // Set the event handler for when the selection is changed.
        if (useDefaultEventHandler)
        {
            comboBox.SelectionChanged({ this, &PrinterSettings::PrintTicketFeatureComboBoxSelectionChanged });
        }
        // Set the print ticket feature as the data context. This is used in the SelectionChanged event handler.
        comboBox.DataContext(feature);

        return comboBox;
    }

    NumberBox PrinterSettings::CreatePrintTicketParameterNumberBox(PrintTicketParameterInitializer const& param, bool useDefaultEventHandler)
    {
        if (param == nullptr)
        {
            return nullptr;
        }

        auto numberBox = NumberBox();
        numberBox.Value(param.Value().GetValueAsInteger());
        // Set the event handler for when the value is changed.
        if (useDefaultEventHandler)
        {
            numberBox.ValueChanged({ this, &PrinterSettings::PrintTicketParameterNumberBoxValueChanged });
        }
        // Set the print ticket parameter as the data context. This is used in the ValueChanged event handler.
        numberBox.DataContext(param);
        // Get the parameter definitions. 
        PrintTicketParameterDefinition capabilities = m_printCapabilities.GetParameterDefinition(param.Name(), param.XmlNamespace());
        // Get the property display name set in the XML with a XPath.
        IXmlNode displayNameNode = capabilities.XmlNode().SelectSingleNode(L"//*[local-name()='Property'][contains(@name, 'DisplayName')]");
        winrt::hstring displayNameString = displayNameNode.InnerText();

        // Header is displayed in the UI, ontop of the NumberBox.
        numberBox.Header(winrt::box_value(displayNameString));
        // Set the range limits for the NumberBox.
        numberBox.Minimum(capabilities.RangeMin());
        numberBox.Maximum(capabilities.RangeMax());

        return numberBox;
    }
    void PrinterSettings::PrintTicketFeatureComboBoxSelectionChanged(IInspectable const& sender, SelectionChangedEventArgs e)
    {
        auto comboBox = sender.try_as<ComboBox>();
        auto feature = comboBox.DataContext().try_as<PrintTicketFeature>();

        // Disable the ComboBox while saving the print ticket.
        comboBox.IsEnabled(false);
        feature.SetSelectedOption(comboBox.SelectedItem().try_as<PrintTicketOption>());
        m_uiSession.UpdatePrintTicket(m_printTicket);
        m_printDevice.UserDefaultPrintTicket(m_printTicket);

        comboBox.IsEnabled(true);
    }

    void PrinterSettings::PrintTicketParameterNumberBoxValueChanged(NumberBox numberBox, NumberBoxValueChangedEventArgs e)
    {
        auto param = numberBox.DataContext().try_as<PrintTicketParameterInitializer>();

        // Disable the NumberBox while saving the print ticket.
        numberBox.IsEnabled(false);
        m_printTicket.SetParameterInitializerAsInteger(param.Name(), param.XmlNamespace(), (int)numberBox.Value());
        m_uiSession.UpdatePrintTicket(m_printTicket);
        numberBox.IsEnabled(true);
    }
}
