#pragma once

#include "WatermarkPreviewControl.g.h"
#include "winrt/XpsUtil.h"
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <functional>

namespace winrt::VirtualPrinterAppSdk::implementation
{
    struct WatermarkPreviewControl : WatermarkPreviewControlT<WatermarkPreviewControl>
    {
        WatermarkPreviewControl()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }
        void SetPagePreviewHandler(std::function<void(XpsUtil::XpsPageWrapper)> modifyPagePreview) { m_modifyPagePreview = modifyPagePreview; };

        winrt::event_token WatermarkPreviewTapped(winrt::Microsoft::UI::Xaml::Input::TappedEventHandler const& handler);
        void WatermarkPreviewTapped(winrt::event_token const& token);

        void WatermarkPreview_Tapped(IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs e);

        void PreviewPage(XpsUtil::XpsPageWrapper const& previewPage) { m_previewPage = previewPage; }
        void StartPreviewRefresh();


    private:
        winrt::Windows::Foundation::IAsyncAction RefreshPreviewAsyc();

        event<winrt::Microsoft::UI::Xaml::Input::TappedEventHandler> m_WatermarkPreviewTappedEvent;
        XpsUtil::XpsPageWrapper m_previewPage{ nullptr };
        Microsoft::UI::Xaml::Media::Imaging::BitmapImage m_image;
        std::function<void(XpsUtil::XpsPageWrapper)> m_modifyPagePreview;
        winrt::Windows::Foundation::IAsyncAction m_refreshPreviewAsyncAction{ nullptr };
    };
}

namespace winrt::VirtualPrinterAppSdk::factory_implementation
{
    struct WatermarkPreviewControl : WatermarkPreviewControlT<WatermarkPreviewControl, implementation::WatermarkPreviewControl>
    {
    };
}
