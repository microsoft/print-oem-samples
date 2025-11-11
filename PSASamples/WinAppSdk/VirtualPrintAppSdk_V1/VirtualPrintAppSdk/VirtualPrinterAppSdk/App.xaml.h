// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "App.xaml.g.h"

namespace winrt::VirtualPrinterAppSdk::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);
        void OnActivated(Windows::ApplicationModel::Activation::IActivatedEventArgs args);
        winrt::Windows::Foundation::IAsyncAction App::CreateAppWindow();

    private:
        winrt::Microsoft::UI::Xaml::Window window{ nullptr };
    };
}
