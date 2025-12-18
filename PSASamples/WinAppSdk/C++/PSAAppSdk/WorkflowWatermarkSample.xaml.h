#pragma once

#include "WorkflowWatermarkSample.g.h"
#include <winrt/XpsUtil.h>

namespace winrt::PSAAppSdk::implementation
{
    struct WatermarkOptions
    {
        hstring Text = L"";
        int TextFontSize{};
        double TextXOffset{};
        double TextYOffset{};

        bool LogoEnabled{false};
        hstring LogoFilePath = L"";
        double LogoDpiX{};
        double LogoDpiY{};
        int LogoWidth{};
        int LogoHeight{};
    };

    struct WorkflowWatermarkSample : WorkflowWatermarkSampleT<WorkflowWatermarkSample>
    {
        WorkflowWatermarkSample()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }
        void InitializeComponent();
        void OnNavigatedTo(winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs e);

        void PreviousPageCommand(IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void NextPageCommand(IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);

        void CancelButton_Click(IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void OnPrintButtonClick(IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void ImageCheckBox_Click(IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void PositionWatermarkButton_Click(IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void WatermarkTextBox_TextChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::TextChangedEventArgs const& args);
        void WatermarkPreview_Tapped(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const& args);
        void TextSizeBox_TextChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::TextChangedEventArgs const& args);

        uint32_t CurrentPage() { return m_currentPage; }
        uint32_t TotalPages() { return m_totalPages; }
        hstring ImageFilePath() { return m_imageFilePath; }
        hstring ImageCheckBoxLabel() { return m_imageCheckBoxLabel; }
        hstring ClickPreviewToPositionWatermarkPrompt() { return m_clickPreviewToPositionWatermarkPrompt; }
        hstring InputErrorPrompt() { return m_inputErrorPrompt; }
        hstring XpsGenerationErrorPrompt() { return m_xpsGenerationErrorPrompt; }
        hstring TextSizeHeader() { return m_textSizeHeader; }
        hstring XPositionHeader() { return m_xPositionHeader; }
        hstring YPositionHeader() { return m_yPositionHeader; }

    private:
        bool UpdateWatermarkOptions();
        winrt::Windows::Foundation::IAsyncOperation<  winrt::Windows::Graphics::Imaging::SoftwareBitmap>
            GetSoftwareBitmapFromFileAsync(winrt::Windows::Storage::StorageFile file);
        void RenderPreview();
        void ApplyModificationsToPreview(XpsUtil::XpsPageWrapper const& previewPage);
        winrt::Windows::Foundation::IAsyncAction FetchImageAttributesAsync();
        winrt::fire_and_forget OnPdlDataAvailableAsync(winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowPdlSourceContent sourceContent);
        void OnPageAdded(XpsUtil::XpsSequentialDocument const& document, uint32_t pageNum);
        void OnDocumentClosed(XpsUtil::XpsSequentialDocument const& document, uint32_t pageCount);
        void OnDocumentError(XpsUtil::XpsSequentialDocument const& document, uint64_t error);

        hstring m_imageFilePath = L"Assets\\contoso_logo.jpg";
        hstring m_imageCheckBoxLabel;
        hstring m_clickPreviewToPositionWatermarkPrompt;
        hstring m_inputErrorPrompt;
        hstring m_xpsGenerationErrorPrompt;
        hstring m_textSizeHeader; 
        hstring m_xPositionHeader;
        hstring m_yPositionHeader;

        winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowConfiguration m_workflowConfig{ nullptr };
        std::atomic<uint32_t> m_currentPage{};
        std::atomic<uint32_t> m_totalPages{};
        std::atomic<double> m_textXOffset{ 0.1 };
        std::atomic<double> m_textYOffset{ 0.1 };
        std::atomic<bool> m_isPositioningWatermark{ false };
        XpsUtil::XpsSequentialDocument m_xpsDocument{ nullptr };
        XpsUtil::XpsPageWatermarker m_waterMarker;
        WatermarkOptions m_waterMarkOptions;
        winrt::PSAAppSdk::JobActivationMainPage m_jobMainPage;
        std::atomic<bool> m_gotFirstPage{ false };
    };
}

namespace winrt::PSAAppSdk::factory_implementation
{
    struct WorkflowWatermarkSample : WorkflowWatermarkSampleT<WorkflowWatermarkSample, implementation::WorkflowWatermarkSample>
    {
    };
}
