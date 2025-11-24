// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "MainWindow.g.h"
#include <winrt/Windows.Graphics.Printing.Workflow.h>

namespace winrt::PSAAppSdk::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();
        MainWindow(winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobActivatedEventArgs args);
        void InitializeComponent();
        Microsoft::UI::Windowing::AppWindow MyAppWindow() { return m_appWindow; }
        winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobActivatedEventArgs WorkflowJobActivatedArgs() { return m_workflowActivatedArgs; }

    private:
        winrt::Microsoft::UI::Windowing::AppWindow GetAppWindowForCurrentWindow();


        Microsoft::UI::Windowing::AppWindow m_appWindow{nullptr};
        winrt::event_token m_appWindowDestroyingToken;
        winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobActivatedEventArgs m_workflowActivatedArgs{ nullptr };
    };
}

namespace winrt::PSAAppSdk::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
