// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "wil/cppwinrt.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include "winrt/Windows.UI.WindowManagement.h"
#include <winrt/windows.ui.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.Xaml.Markup.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <windows.ui.interop.h>
#include <wil/cppwinrt_helpers.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Interop.h>
#include <winrt/Microsoft.UI.Content.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>

#include <ModalWindowHandler.h>
#include "PrinterSettings.xaml.h"
#include <winrt/XpsUtil.h>

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI;
    using namespace Microsoft::UI::Windowing;
    using namespace Microsoft::UI::Dispatching;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Hosting;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Navigation;
    using namespace Windows::Graphics::Printing::PrintSupport;
    using namespace Windows::Graphics::Printing::Workflow;
    using namespace Windows::UI;
    using namespace Windows::System;
    using namespace XpsUtil;
    using namespace VirtualPrinterAppSdk;
    using namespace VirtualPrinterAppSdk::implementation;
}

using namespace winrt;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
    InitializeComponent();

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
    UnhandledException([this](IInspectable const&, UnhandledExceptionEventArgs const& e)
    {
        if (IsDebuggerPresent())
        {
            auto errorMessage = e.Message();
            __debugbreak();
        }
    });
#endif
}

/// <summary>
/// Invoked when the application is launched.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(winrt::LaunchActivatedEventArgs const&)
{
    auto args = winrt::Windows::ApplicationModel::AppInstance::GetActivatedEventArgs();

    if (args.Kind() == winrt::Windows::ApplicationModel::Activation::ActivationKind::PrintSupportSettingsUI)
    {
        auto settingsArgs = args.as<winrt::PrintSupportSettingsActivatedEventArgs>();
        winrt::Microsoft::UI::WindowId OwnerWindowId{ settingsArgs.OwnerWindowId().Value };
        auto printerSettingsPage = winrt::make<winrt::VirtualPrinterAppSdk::implementation::PrinterSettings>(settingsArgs);
        auto modalWindowHandler = winrt::make_self<ModalWindowHandler>(OwnerWindowId, printerSettingsPage);
        modalWindowHandler->Activate();
    }
    else  if (args.Kind() == winrt::Windows::ApplicationModel::Activation::ActivationKind::PrintSupportJobUI)
    {
        auto workflowJobEventArgs = args.as<winrt::PrintWorkflowJobActivatedEventArgs>();
        window = make<MainWindow>(workflowJobEventArgs);
        window.Activate();
    }
    else
    {
        window = make<MainWindow>();
        window.Activate();
    }

}

void App::OnActivated(Windows::ApplicationModel::Activation::IActivatedEventArgs args)
{
    window = make<MainWindow>();
    window.Activate();
}