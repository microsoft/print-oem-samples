// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "AppInfo.g.h"

namespace winrt::VirtualPrinterAppSdk::implementation
{
    struct AppInfo : AppInfoT<AppInfo>
    {
        AppInfo();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs e);

    };
}

namespace winrt::VirtualPrinterAppSdk::factory_implementation
{
    struct AppInfo : AppInfoT<AppInfo, implementation::AppInfo>
    {
    };
}
