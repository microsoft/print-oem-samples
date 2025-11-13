// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <winrt/Microsoft.UI.Interop.h>
#include <microsoft.ui.xaml.window.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Windows.Foundation.h>

using namespace winrt;
//using namespace Microsoft::UI::Xaml;

using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Graphics::Printing::PrintSupport;
using namespace Windows::Graphics::Printing::Workflow;
using namespace winrt::Microsoft::UI::Windowing;
using namespace winrt;
using namespace Microsoft::UI;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;


// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::VirtualPrinterAppSdk::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
        m_appWindow = GetAppWindowForCurrentWindow();
        
        rootFrame().Navigate(xaml_typename<VirtualPrinterAppSdk::UserLaunchMainPage>());
    }

    MainWindow::MainWindow(winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobActivatedEventArgs args) : m_workflowActivatedArgs(args)
    {  
    }
    void MainWindow::InitializeComponent()
    {
        MainWindowT::InitializeComponent();
        if (m_workflowActivatedArgs)
        {
            m_appWindow = GetAppWindowForCurrentWindow();
            rootFrame().Navigate(xaml_typename<VirtualPrinterAppSdk::JobActivationMainPage>(), *this);
        }
    }

    AppWindow MainWindow::GetAppWindowForCurrentWindow()
    {      
        // Get access to IWindowNative
        winrt::VirtualPrinterAppSdk::MainWindow thisWindow = *this;
        winrt::com_ptr<IWindowNative> windowNative = thisWindow.as<IWindowNative>();

        //Get the HWND for the XAML Window
        HWND hWnd;
        winrt::check_hresult(windowNative->get_WindowHandle(&hWnd));

        // Get the WindowId for our window
        WindowId windowId;
        windowId = ::GetWindowIdFromWindow(hWnd);

        // Get the AppWindow for the WindowId
        Microsoft::UI::Windowing::AppWindow appWindow = Microsoft::UI::Windowing::AppWindow::GetFromWindowId(windowId);

        return appWindow;
    }
}
