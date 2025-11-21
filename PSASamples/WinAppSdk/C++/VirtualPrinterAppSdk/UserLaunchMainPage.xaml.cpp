// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "UserLaunchMainPage.xaml.h"
#if __has_include("UserLaunchMainPage.g.cpp")
#include "UserLaunchMainPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Windows::Foundation;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::VirtualPrinterAppSdk::implementation
{
    UserLaunchMainPage::UserLaunchMainPage()
    {
        InitializeComponent();
    }

    void UserLaunchMainPage::NavViewItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView /*sender*/, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs args)
    {
        auto tagg = args.InvokedItemContainer().Tag().try_as<winrt::hstring>();
        if (tagg == L"GetIppPrinterUrl")
        {
            contentFrame().Navigate(xaml_typename<VirtualPrinterAppSdk::AppInfo>(), args);
        }
        else
        {
            contentFrame().Navigate(xaml_typename<VirtualPrinterAppSdk::AppInfo>(), args);
        }
    }

    void UserLaunchMainPage::NavViewLoaded(IInspectable const& /*sender*/, RoutedEventArgs e)
    {
        // NavView doesn't load any page by default, so load the first page.
        NavigationViewControl().SelectedItem(NavigationViewControl().MenuItems().First());
        contentFrame().Navigate(xaml_typename < VirtualPrinterAppSdk::AppInfo>(), e);
    }

    void UserLaunchMainPage::OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs e)
    {
        NavigationViewControl().SelectedItem(NavigationViewControl().MenuItems().First());
        contentFrame().Navigate(xaml_typename < VirtualPrinterAppSdk::AppInfo>(), e);
    }
}
