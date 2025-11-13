#pragma once

#include "JobActivationMainPage.g.h"

namespace winrt::VirtualPrinterAppSdk::implementation
{
    struct JobActivationMainPage : JobActivationMainPageT<JobActivationMainPage>
    {
        JobActivationMainPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }
        void InitializeComponent();

        void OnNavigatedTo(winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs e);
        void CloseDialog();
        winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowPdlDataAvailableEventArgs PdlDataEventArgs()
        {
            return m_pdlDataEventArgs;
        }

        winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobNotificationEventArgs JobNotificationEventArgs()
        {
            return m_jobNotificationEventArgs;
        }

        winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowVirtualPrinterUIEventArgs VirtualPrinterPdlDataEventArgs()
        {
            return m_virtualPrinterPldDataEventArgs;
        }

       JobActivationKind ActivationKind() { return m_activationKind;  }

    private:
        void OnWindowClosing();
        void OnSessionJobNotification(winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobUISession sender,
            winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobNotificationEventArgs args);
        void OnSessionPdlDataAvailable(winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobUISession sender,
                winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowPdlDataAvailableEventArgs args);
        void OnVirtualPdlDataAvailable(winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobUISession sender,
            winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowVirtualPrinterUIEventArgs args);
        winrt::Windows::Foundation::IAsyncAction StartAsync(winrt::VirtualPrinterAppSdk::MainWindow mainWindow);
        winrt::fire_and_forget NavigateToPreview();

        Microsoft::UI::Windowing::AppWindow m_appWindow{ nullptr };
        Windows::Foundation::Deferral m_pdlDataAvailableDeferral{ nullptr };
        Windows::Foundation::Deferral m_jobNotificationDeferral{ nullptr };
        Windows::Foundation::Deferral m_virtualPrinterPdlDeferral{ nullptr };

        winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowPdlDataAvailableEventArgs m_pdlDataEventArgs{ nullptr };
        winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobNotificationEventArgs m_jobNotificationEventArgs{ nullptr };
        winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowVirtualPrinterUIEventArgs m_virtualPrinterPldDataEventArgs{nullptr};
        JobActivationKind m_activationKind = JobActivationKind::WorkflowPdl;
    };
}

namespace winrt::VirtualPrinterAppSdk::factory_implementation
{
    struct JobActivationMainPage : JobActivationMainPageT<JobActivationMainPage, implementation::JobActivationMainPage>
    {
    };
}
