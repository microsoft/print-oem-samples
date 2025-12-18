#pragma once

#include "PreviewPaginationControl.g.h"

namespace winrt::PSAAppSdk::implementation
{
    struct PreviewPaginationControl : PreviewPaginationControlT<PreviewPaginationControl>
    {
        PreviewPaginationControl()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        void InitializeComponent();

        void EvaluateButtonEnabledStates();

        winrt::event_token NextButtonClicked(winrt::Microsoft::UI::Xaml::RoutedEventHandler const& handler);
        void NextButtonClicked(winrt::event_token const& token);

        winrt::event_token PrevButtonClicked(winrt::Microsoft::UI::Xaml::RoutedEventHandler const& handler);
        void PrevButtonClicked(winrt::event_token const& token);

        void PreviousPageButton_Click(IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void NextPageButton_Click(IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);


        uint32_t TotalPages() { return m_totalPages;  }
        void TotalPages(uint32_t pages); 
        
        uint32_t CurrentPage() { return m_currentPage; }
        void CurrentPage(uint32_t curPage);
    private:
        event<winrt::Microsoft::UI::Xaml::RoutedEventHandler> m_nextButtonEventHandler{};
        event<winrt::Microsoft::UI::Xaml::RoutedEventHandler> m_prevButtonEventHandler{};

        uint32_t m_totalPages{};
        uint32_t m_currentPage{};
    };
}

namespace winrt::PSAAppSdk::factory_implementation
{
    struct PreviewPaginationControl : PreviewPaginationControlT<PreviewPaginationControl, implementation::PreviewPaginationControl>
    {
    };
}
