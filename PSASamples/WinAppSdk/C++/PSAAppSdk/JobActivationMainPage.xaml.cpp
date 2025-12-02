#include "pch.h"
#include "JobActivationMainPage.xaml.h"
#if __has_include("JobActivationMainPage.g.cpp")
#include "JobActivationMainPage.g.cpp"
#endif
#include <winrt/Microsoft.UI.Windowing.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Windowing;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::PSAAppSdk::implementation
{
	void JobActivationMainPage::OnNavigatedTo(winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs e)
	{
        Window window = e.Parameter().as<Window>();
        MainWindow mainWindow = window.as<MainWindow>();
        m_appWindow = mainWindow.MyAppWindow();
        m_appWindow.Closing([this](auto&&, auto&&)
            {
                OnWindowClosing();
            });

        StartAsync(mainWindow);

        auto args = mainWindow.WorkflowJobActivatedArgs();
        auto workflowSession = args.Session();
        workflowSession.PdlDataAvailable({ get_strong(), &JobActivationMainPage::OnSessionPdlDataAvailable });
        workflowSession.JobNotification({ get_strong(), &JobActivationMainPage::OnSessionJobNotification });
        workflowSession.VirtualPrinterUIDataAvailable({ get_strong(), &JobActivationMainPage::OnVirtualPdlDataAvailable });
        workflowSession.Start();
	}

    void JobActivationMainPage::InitializeComponent()
    {
        JobActivationMainPageT::InitializeComponent();
    }

    winrt::Windows::Foundation::IAsyncAction JobActivationMainPage::StartAsync(winrt::PSAAppSdk::MainWindow mainWindow)
    {
        co_await winrt::resume_background();
        
    }

    void JobActivationMainPage::CloseDialog()
    {
        OnWindowClosing();
        m_appWindow.Destroy();
        PostQuitMessage(0);
    }

    void JobActivationMainPage::OnWindowClosing()
    {
        if (m_pdlDataAvailableDeferral)
        {
            m_pdlDataAvailableDeferral.Complete();
            m_pdlDataAvailableDeferral = nullptr;
        }
        if (m_jobNotificationDeferral)
        {
            m_jobNotificationDeferral.Complete();
            m_jobNotificationDeferral = nullptr;
        }
    }

    void JobActivationMainPage::OnSessionJobNotification(winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobUISession sender,
        winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobNotificationEventArgs args)
    {
        m_activationKind = JobActivationKind::JobNotification;
        m_jobNotificationEventArgs = args;
        m_jobNotificationDeferral = args.GetDeferral();
        NavigateToPreview();
    }

    void JobActivationMainPage::OnSessionPdlDataAvailable(winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobUISession /*sender*/,
        winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowPdlDataAvailableEventArgs args)
    {
        m_activationKind = JobActivationKind::WorkflowPdl;
        m_pdlDataAvailableDeferral = args.GetDeferral();
        m_pdlDataEventArgs = args;
        NavigateToPreview();
    }

    void JobActivationMainPage::OnVirtualPdlDataAvailable(winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobUISession /*sender*/,
        winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowVirtualPrinterUIEventArgs args)
    {
        m_activationKind = JobActivationKind::WorkflowVirtualPrinterPdl;
        m_virtualPrinterPldDataEventArgs = args;
        m_virtualPrinterPdlDeferral = args.GetDeferral();
        NavigateToPreview();
    }

    winrt::fire_and_forget JobActivationMainPage::NavigateToPreview()
    {
        co_await wil::resume_foreground(this->DispatcherQueue());
        contentFrame().Navigate(xaml_typename <PSAAppSdk::WorkflowWatermarkSample>(), *this);
    }
}
