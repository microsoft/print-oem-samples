#pragma once

#include "PrintSupportExtension.g.h"
#include <winrt\Windows.ApplicationModel.Background.h>
#include <winrt\Windows.Graphics.Printing.PrintSupport.h>
#include <winrt\Windows.Graphics.Printing.Workflow.h>

namespace winrt::BackgroundTasks::implementation
{
    struct PrintSupportExtension : PrintSupportExtensionT<PrintSupportExtension>
    {
        PrintSupportExtension() = default;
        void Run(winrt::Windows::ApplicationModel::Background::IBackgroundTaskInstance const& instance);
    private:
        void OnTaskInstanceCanceled(winrt::Windows::ApplicationModel::Background::IBackgroundTaskInstance const& sender,
            winrt::Windows::ApplicationModel::Background::BackgroundTaskCancellationReason const& reason);
        void  OnSessionPrintTicketValidationRequested(winrt::Windows::Graphics::Printing::PrintSupport::PrintSupportExtensionSession const& sender,
            winrt::Windows::Graphics::Printing::PrintSupport::PrintSupportPrintTicketValidationRequestedEventArgs const& args);
        void OnSessionPrintDeviceCapabilitiesChanged(winrt::Windows::Graphics::Printing::PrintSupport::PrintSupportExtensionSession const& sender,
            winrt::Windows::Graphics::Printing::PrintSupport::PrintSupportPrintDeviceCapabilitiesChangedEventArgs const& args);
        void OnSessionCommincationErrorDetected(winrt::Windows::Graphics::Printing::PrintSupport::PrintSupportExtensionSession const& sender,
            winrt::Windows::Graphics::Printing::PrintSupport::PrintSupportCommunicationErrorDetectedEventArgs const& args);

        void AddCustomMediaType(winrt::Windows::Data::Xml::Dom::XmlDocument const& pdc, winrt::hstring const& namespaceUri, winrt::hstring const& mediaType);

        winrt::event_token m_ptValidationToken;
        winrt::event_token m_pdcChangedToken;
        winrt::Windows::ApplicationModel::Background::BackgroundTaskDeferral m_taskDeferral{ nullptr };
        winrt::Windows::Graphics::Printing::PrintSupport::PrintSupportExtensionSession m_session{ nullptr };
    };
}

namespace winrt::BackgroundTasks::factory_implementation
{
    struct PrintSupportExtension : PrintSupportExtensionT<PrintSupportExtension, implementation::PrintSupportExtension>
    {
    };
}
