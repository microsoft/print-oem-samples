#include "pch.h"
#include "PreviewPaginationControl.xaml.h"
#if __has_include("PreviewPaginationControl.g.cpp")
#include "PreviewPaginationControl.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::PSAAppSdk::implementation
{
    void PreviewPaginationControl::InitializeComponent()
    {
        PreviewPaginationControlT::InitializeComponent();
        this->IsEnabledChanged([this](IInspectable const& , DependencyPropertyChangedEventArgs const& )
            {
                EvaluateButtonEnabledStates();
            });
    }

    void PreviewPaginationControl::TotalPages(uint32_t pages)
    {
        m_totalPages = pages;
        if (m_totalPages != 0)
        {
            TotalPagesLabel().Text(to_hstring(m_totalPages));
            EvaluateButtonEnabledStates();
        }
    }

    void PreviewPaginationControl::CurrentPage(uint32_t curPage)
    {
        m_currentPage = curPage;
        if (m_currentPage != 0)
        {
            CurrentPageLabel().Text(to_hstring(m_currentPage));
            EvaluateButtonEnabledStates();
        }
    }

    void PreviewPaginationControl::EvaluateButtonEnabledStates()
    {
        PreviousPageButton().IsEnabled(IsEnabled() && m_currentPage > 1);
        NextPageButton().IsEnabled(IsEnabled() && m_currentPage >0 && m_totalPages > 0 && m_currentPage < m_totalPages);
    }

    winrt::event_token PreviewPaginationControl::NextButtonClicked(winrt::Microsoft::UI::Xaml::RoutedEventHandler const& handler)
    {
        return m_nextButtonEventHandler.add(handler);
    }

    void PreviewPaginationControl::NextButtonClicked(winrt::event_token const& token)
    {
       m_nextButtonEventHandler.remove(token);
    }

    winrt::event_token PreviewPaginationControl::PrevButtonClicked(winrt::Microsoft::UI::Xaml::RoutedEventHandler const& handler)
    {
        return m_prevButtonEventHandler.add(handler);
    }

    void PreviewPaginationControl::PrevButtonClicked(winrt::event_token const& token)
    {
        m_prevButtonEventHandler.remove(token);
    }

    void PreviewPaginationControl::PreviousPageButton_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args)
    {
        if (m_nextButtonEventHandler)
        {
            m_nextButtonEventHandler(sender, args);
        }
    }

    void PreviewPaginationControl::NextPageButton_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args)
    {
        if (m_prevButtonEventHandler)
        {
            m_prevButtonEventHandler(sender, args);
        }
    }
}
