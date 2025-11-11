#include "pch.h"
#include "WatermarkPreviewControl.xaml.h"
#if __has_include("WatermarkPreviewControl.g.cpp")
#include "WatermarkPreviewControl.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::VirtualPrinterAppSdk::implementation
{
    winrt::event_token WatermarkPreviewControl::WatermarkPreviewTapped(winrt::Microsoft::UI::Xaml::Input::TappedEventHandler const& handler)
    {
        return m_WatermarkPreviewTappedEvent.add(handler);
    }

    void WatermarkPreviewControl::WatermarkPreviewTapped(winrt::event_token const& token)
    {
        m_WatermarkPreviewTappedEvent.remove(token);
    }

    void WatermarkPreviewControl::WatermarkPreview_Tapped(IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs e)
    {
        if (m_WatermarkPreviewTappedEvent)
        {
            m_WatermarkPreviewTappedEvent(sender, e);
        }
    }

    void WatermarkPreviewControl::StartPreviewRefresh()
    {
        if (m_refreshPreviewAsyncAction)
        {
            m_refreshPreviewAsyncAction.Cancel();
        }

        m_refreshPreviewAsyncAction = RefreshPreviewAsyc();
    }

    winrt::Windows::Foundation::IAsyncAction WatermarkPreviewControl::RefreshPreviewAsyc()
    {
        if (!m_previewPage)
        {
            // Nothing to do; probably the document isn't loaded yet
            return;
        }
        auto strongPtr = get_strong();
        auto cancellation_token{ co_await winrt::get_cancellation_token() };
        
        if (cancellation_token())
        {
            co_return;
        }

        XpsUtil::XpsPageWrapper clonedPage = m_previewPage.Clone();

        if (m_modifyPagePreview)
        {
            m_modifyPagePreview(clonedPage);
        }

        if (cancellation_token())
        {
            co_return;
        }

        auto bmpStream = clonedPage.RenderPageToBMP();
        bmpStream.Seek(0);

        co_await wil::resume_foreground(this->DispatcherQueue());
     
        m_image.SetSource(bmpStream);
        WatermarkPreview().Source(m_image);

    }
}
