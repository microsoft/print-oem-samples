// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "UserLaunchMainPage.g.h"

namespace winrt::PSAAppSdk::implementation
{
    struct UserLaunchMainPage : UserLaunchMainPageT<UserLaunchMainPage>
    {
        UserLaunchMainPage();
        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs e);
        void NavViewLoaded(winrt::Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs e);
    };
}

namespace winrt::PSAAppSdk::factory_implementation
{
    struct UserLaunchMainPage : UserLaunchMainPageT<UserLaunchMainPage, implementation::UserLaunchMainPage>
    {
    };
}
