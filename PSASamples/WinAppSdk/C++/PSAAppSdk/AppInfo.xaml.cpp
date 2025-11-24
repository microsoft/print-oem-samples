// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "AppInfo.xaml.h"
#if __has_include("AppInfo.g.cpp")
#include "AppInfo.g.cpp"
#endif
using namespace winrt::Windows::ApplicationModel;

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace Windows::ApplicationModel;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::PSAAppSdk::implementation
{
    AppInfo::AppInfo()
    {
        InitializeComponent();
    }

    void AppInfo::OnNavigatedTo(NavigationEventArgs /*e*/)
    {
        PackageFamilyNameTextBox().Text(Package::Current().Id().FamilyName());
    }
}
