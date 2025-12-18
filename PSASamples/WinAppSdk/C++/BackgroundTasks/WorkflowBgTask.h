#pragma once

#include "WorkflowBgTask.g.h"
#include <winrt\Windows.ApplicationModel.Background.h>
#include <winrt\Windows.Graphics.Printing.PrintSupport.h>
#include <winrt\Windows.Graphics.Printing.Workflow.h>
#include <winrt/XpsUtil.h>

namespace winrt::BackgroundTasks::implementation
{
    struct WorkflowBgTask :WorkflowBgTaskT<WorkflowBgTask>
    {
        WorkflowBgTask() = default;
        void Run(winrt::Windows::ApplicationModel::Background::IBackgroundTaskInstance const& instance);

        void OnJobStarting(winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobBackgroundSession const& sender, winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobStartingEventArgs const& args);
        void OnPdlModificationRequested(winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobBackgroundSession const& sender, winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowPdlModificationRequestedEventArgs const& args);

    private:
        winrt::hstring WorkflowBgTask::GetDocumentFormat(winrt::Windows::Devices::Printers::IppPrintDevice const& printer);
        bool HasPdlConverter(winrt::hstring const& documentFormat);
        winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowPdlConverter GetPdlConverter(
            winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowPdlModificationRequestedEventArgs const& args, winrt::hstring const& documentFormat);
        XpsUtil::XpsPageWatermarker ConfigureWatermarker();

        winrt::event_token m_jobStartToken;
        winrt::event_token m_pdlModificationToken;
        winrt::Windows::ApplicationModel::Background::BackgroundTaskDeferral m_taskDeferral{ nullptr };
        winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobBackgroundSession m_session{ nullptr };
    };
}

namespace winrt::BackgroundTasks::factory_implementation
{
    struct WorkflowBgTask : WorkflowBgTaskT<WorkflowBgTask, implementation::WorkflowBgTask>
    {
    };
}
