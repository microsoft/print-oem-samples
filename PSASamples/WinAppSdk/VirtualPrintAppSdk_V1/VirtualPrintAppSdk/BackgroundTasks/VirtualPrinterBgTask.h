#pragma once

#include "VirtualPrinterBgTask.g.h"

#include <winrt\Windows.ApplicationModel.Background.h>
#include <winrt\Windows.Graphics.Printing.PrintSupport.h>
#include <winrt\Windows.Graphics.Printing.Workflow.h>
#include <winrt/XpsUtil.h>

namespace winrt::BackgroundTasks::implementation
{
    struct VirtualPrinterBgTask :VirtualPrinterBgTaskT<VirtualPrinterBgTask>
    {
        VirtualPrinterBgTask() = default;
        ~VirtualPrinterBgTask();
        void Run(winrt::Windows::ApplicationModel::Background::IBackgroundTaskInstance const& instance);


        void OnPdlDataAvailable(winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowVirtualPrinterSession const& session, winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowVirtualPrinterDataAvailableEventArgs const& args);

    private:
        XpsUtil::XpsPageWatermarker ConfgiureWatermarker();

        winrt::Windows::ApplicationModel::Background::BackgroundTaskDeferral m_taskDeferral{ nullptr };
        winrt::Windows::Devices::Printers::IppPrintDevice m_printDevice{ nullptr };
        winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowVirtualPrinterSession m_session{ nullptr };
        winrt::event_token m_dataAvailToken;
    };
}

namespace winrt::BackgroundTasks::factory_implementation
{
    struct VirtualPrinterBgTask : VirtualPrinterBgTaskT<VirtualPrinterBgTask, implementation::VirtualPrinterBgTask>
    {
    };
}