#include "pch.h"
#include "WorkflowWatermarkSample.xaml.h"
#if __has_include("WorkflowWatermarkSample.g.cpp")
#include "WorkflowWatermarkSample.g.cpp"
#endif
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/BackgroundTasks.h>
#include <winrt/XpsUtil.h>
#include <WatermarkPreviewControl.Xaml.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Input;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Graphics::Imaging;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Graphics::Printing::Workflow;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::PSAAppSdk::implementation
{
    void WorkflowWatermarkSample::OnNavigatedTo(winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs e)
    {
        m_jobMainPage = e.Parameter().as<winrt::PSAAppSdk::JobActivationMainPage>();
        if (m_jobMainPage.ActivationKind() == JobActivationKind::WorkflowPdl)
        {
            auto args = m_jobMainPage.PdlDataEventArgs();
            m_workflowConfig = args.Configuration();
            OnPdlDataAvailableAsync(args.SourceContent());
        }
        else if (m_jobMainPage.ActivationKind() == JobActivationKind::WorkflowVirtualPrinterPdl)
        {
            auto args = m_jobMainPage.VirtualPrinterPdlDataEventArgs();
            m_workflowConfig = args.Configuration();
            OnPdlDataAvailableAsync(args.SourceContent());
        }
    }

    void WorkflowWatermarkSample::InitializeComponent()
    {
        WorkflowWatermarkSampleT::InitializeComponent();
        DataContext(*this);
        BackgroundTasks::LocalStorageHelper::ResetWatermarkTextAndImage();
        FetchImageAttributesAsync();
        auto waterMarkPreview = winrt::get_self<winrt::PSAAppSdk::implementation::WatermarkPreviewControl>(WatermarkPreview());
        waterMarkPreview->SetPagePreviewHandler([this](XpsUtil::XpsPageWrapper previewPage)
            {
                ApplyModificationsToPreview(previewPage);
            });
    }

    winrt::fire_and_forget  WorkflowWatermarkSample::OnPdlDataAvailableAsync(winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowPdlSourceContent sourceContent)
    {
        co_await winrt::resume_background();
        auto contentType = sourceContent.ContentType();

        if (!_wcsicmp(contentType.c_str(), L"application/OXPS"))
        {
            co_await wil::resume_foreground(this->DispatcherQueue());
            PreviewLoadingProgressRing().IsActive(true);
            PreviewLoadingProgressRing().Visibility(Visibility::Visible);
            PrintButton().IsEnabled(false);

            PreviewPaginator().TotalPages(0);
            PreviewPaginator().CurrentPage(0);

            // Do XPS processing in background thread 
            co_await winrt::resume_background();
            auto xpsContentStream = sourceContent.GetInputStream();
            PrintWorkflowObjectModelSourceFileContent xpsContentObjectModel(xpsContentStream);
            m_xpsDocument = XpsUtil::XpsSequentialDocument(xpsContentObjectModel);
            m_xpsDocument.PageAdded({get_weak(), &WorkflowWatermarkSample::OnPageAdded });
            m_xpsDocument.DocumentClosed({ get_weak(), &WorkflowWatermarkSample::OnDocumentClosed });
            m_xpsDocument.XpsGenerationFailed({ get_weak(), &WorkflowWatermarkSample::OnDocumentError });

            m_xpsDocument.StartXpsOMGeneration();
        }
    }

    void WorkflowWatermarkSample::OnPageAdded(XpsUtil::XpsSequentialDocument const& document, uint32_t pageNum)
    {
        if (pageNum > m_totalPages)
        {
            m_totalPages = pageNum;
            DispatcherQueue().TryEnqueue([strongThis = get_strong(), this]()
                {
                    PreviewPaginator().TotalPages(m_totalPages);
                });
        }

        if (!m_gotFirstPage)
        {
            m_gotFirstPage = true;
            DispatcherQueue().TryEnqueue([strongThis = get_strong(), this, document]()
                {
                    m_currentPage = 1;
                    PreviewPaginator().CurrentPage(m_currentPage);
                    auto page = document.GetPage(m_currentPage);
                    WatermarkPreview().PreviewPage(page);
                    RenderPreview();

                    PrintButton().IsEnabled(true);
                });
        }
    }

    void WorkflowWatermarkSample::OnDocumentClosed(XpsUtil::XpsSequentialDocument const& /*document*/, uint32_t /*pageCount*/)
    {
        DispatcherQueue().TryEnqueue([strongThis = get_strong(), this]()
            {
                PreviewLoadingProgressRing().IsActive(false);
                PreviewLoadingProgressRing().Visibility(Visibility::Collapsed);

                RenderPreview();
            });
    }

    void WorkflowWatermarkSample::OnDocumentError(XpsUtil::XpsSequentialDocument const& /*document*/, uint64_t error)
    {
        wchar_t documentError[MAX_PATH];
        swprintf_s(documentError, ARRAYSIZE(documentError), L"\nError in XPS document parsing :%I64x  ", error);
        OutputDebugString(documentError);
        DispatcherQueue().TryEnqueue([strongThis = get_strong(), this]()
            {
                PreviewLoadingProgressRing().IsActive(false);

                // Disable all controls except "cancel"
                PrintButton().IsEnabled(false);
                WatermarkTextBox().IsEnabled(false);
                TextSizeBox().IsEnabled(false);
                ImageCheckBox().IsEnabled(false);
                PreviewPaginator().IsEnabled(false);
                PositionWatermarkButton().IsEnabled(false);
                PreviewPaginator().CurrentPage(0);
                PreviewPaginator().TotalPages(0);

                XpsGenerationErrorTextBlock().Visibility(Visibility::Visible);
            });
    }

    void WorkflowWatermarkSample::PreviousPageCommand(IInspectable const& /*sender*/, RoutedEventArgs const& /*args*/)
    {
        if (m_currentPage < 2)
        {
            return;
        }
        m_currentPage--;
        PreviewPaginator().CurrentPage(m_currentPage);
        WatermarkPreview().PreviewPage(m_xpsDocument.GetPage(PreviewPaginator().CurrentPage()));

        RenderPreview();
    }

    void WorkflowWatermarkSample::NextPageCommand(IInspectable const& /*sender*/, RoutedEventArgs const& /*args*/)
    {
        if (m_currentPage > (m_totalPages - 1))
        {
            return;
        }
        m_currentPage++;
        PreviewPaginator().CurrentPage(m_currentPage);
        WatermarkPreview().PreviewPage(m_xpsDocument.GetPage(PreviewPaginator().CurrentPage()));

        RenderPreview();
    }

    void WorkflowWatermarkSample::CancelButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*args*/)
    {
        m_workflowConfig.AbortPrintFlow(PrintWorkflowJobAbortReason::UserCanceled);
        m_jobMainPage.CloseDialog();
    }

    void WorkflowWatermarkSample::OnPrintButtonClick(IInspectable const& sender, RoutedEventArgs const& /*args*/)
    {
        auto button = sender.as<Button>();
        winrt::hstring printMessage = L"Sending Job To Printer...";
        button.IsEnabled(false);
        button.Content(winrt::box_value(printMessage));

        if (!m_waterMarkOptions.Text.empty())
        {
            bool isValid = UpdateWatermarkOptions();
            if (isValid)
            {
                BackgroundTasks::LocalStorageHelper::SaveWatermarkTextPropertiesToLocalStorage(m_waterMarkOptions.Text,
                    m_waterMarkOptions.TextFontSize, m_waterMarkOptions.TextXOffset, m_waterMarkOptions.TextXOffset);
            }
            else
            {
                BackgroundTasks::LocalStorageHelper::ResetWatermarkTextAndImage();
            }
        }

        BackgroundTasks::LocalStorageHelper::SaveImagePropertiesToLocalStorage(m_waterMarkOptions.LogoEnabled ? m_waterMarkOptions.LogoFilePath : L"",
            m_waterMarkOptions.LogoDpiX, m_waterMarkOptions.LogoDpiY, m_waterMarkOptions.LogoWidth, m_waterMarkOptions.LogoHeight);

        m_jobMainPage.CloseDialog();
    }

    void WorkflowWatermarkSample::ImageCheckBox_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*args*/)
    {
        RenderPreview();
    }

    void WorkflowWatermarkSample::TextSizeBox_TextChanged(IInspectable const& /*sender*/, TextChangedEventArgs const& /*args*/)
    {
        RenderPreview();
    }

    void WorkflowWatermarkSample::WatermarkTextBox_TextChanged(IInspectable const& /*sender*/, TextChangedEventArgs const& /*args*/)
    {
        RenderPreview();
    }

    void WorkflowWatermarkSample::PositionWatermarkButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*args*/)
    {
        PositionWatermarkPrompt().Visibility(Visibility::Visible);
        PositionWatermarkButton().IsEnabled(false);
        m_isPositioningWatermark = true;
    }

    void WorkflowWatermarkSample::WatermarkPreview_Tapped(IInspectable const& sender, TappedRoutedEventArgs const& args)
    {
        if (m_isPositioningWatermark)
        {
            m_isPositioningWatermark = false;
            PositionWatermarkPrompt().Visibility(Visibility::Collapsed);
            PositionWatermarkButton().IsEnabled(true);

            Image previewImage = sender.as<Image>();
            winrt::Windows::Foundation::Point tapPoint = args.GetPosition(previewImage);

            // Calculate the new offset based on where the user tapped
            m_textXOffset = tapPoint.X / previewImage.ActualWidth();
            m_textYOffset = tapPoint.Y / previewImage.ActualHeight();

            RenderPreview();
        }
    }

    winrt::Windows::Foundation::IAsyncAction WorkflowWatermarkSample::FetchImageAttributesAsync()
    {
        co_await winrt::resume_background();
        auto imageUri = Uri(L"ms-appx:///" + m_imageFilePath);
        auto file = co_await StorageFile::GetFileFromApplicationUriAsync(imageUri);
        SoftwareBitmap contosoLogo = co_await GetSoftwareBitmapFromFileAsync(file);

        m_waterMarkOptions.LogoFilePath = m_imageFilePath;
        m_waterMarkOptions.LogoDpiX = contosoLogo.DpiX();
        m_waterMarkOptions.LogoDpiY = contosoLogo.DpiY();
        m_waterMarkOptions.LogoHeight = contosoLogo.PixelHeight();
        m_waterMarkOptions.LogoWidth = contosoLogo.PixelWidth();

        RenderPreview();
        co_return;
    }

    void WorkflowWatermarkSample::ApplyModificationsToPreview(XpsUtil::XpsPageWrapper const& previewPage)
    {
        m_waterMarker.SetWatermarkText(m_waterMarkOptions.Text, m_waterMarkOptions.TextFontSize, m_waterMarkOptions.TextXOffset, m_waterMarkOptions.TextYOffset);
        m_waterMarker.SetWatermarkImage(m_waterMarkOptions.LogoFilePath, m_waterMarkOptions.LogoDpiX, m_waterMarkOptions.LogoDpiY, m_waterMarkOptions.LogoWidth, m_waterMarkOptions.LogoHeight);
        m_waterMarker.SetWatermarkImageEnabled(m_waterMarkOptions.LogoEnabled);

        m_waterMarker.ApplyWatermarks(previewPage);
    }

    void WorkflowWatermarkSample::RenderPreview()
    {
        // Update the m_waterMarkOptions object based on the current UI state.
            // If invalid, an error message will show and we won't update the preview.
        if (UpdateWatermarkOptions())
        {
            // Kick off the preview refresh.
            // This starts rendering the preview on a background thread and returns immediately.
            // If StartPreviewRefresh is called again before the previous refresh finishes, the previous refresh will be cancelled.
            WatermarkPreview().StartPreviewRefresh();
            InputErrorTextBlock().Visibility(Visibility::Collapsed);
        }
        else
        {
            InputErrorTextBlock().Visibility(Visibility::Visible);
        }
    }

    bool WorkflowWatermarkSample::UpdateWatermarkOptions()
    {
        m_waterMarkOptions.Text = WatermarkTextBox().Text();
        m_waterMarkOptions.TextFontSize = _wtoi(TextSizeBox().Text().c_str());
        if ((INT_MAX == m_waterMarkOptions.TextFontSize || INT_MIN == m_waterMarkOptions.TextFontSize) && ERANGE == errno)
        {
            return false;
        }

        m_waterMarkOptions.TextXOffset = m_textXOffset;
        m_waterMarkOptions.TextYOffset = m_textYOffset;

        if (m_waterMarkOptions.TextXOffset > 1 || m_waterMarkOptions.TextXOffset < 0 || m_waterMarkOptions.TextYOffset < 0 || m_waterMarkOptions.TextYOffset > 1)
        {
            return false;
        }
        
        // Update the image state based on the checkbox status
        m_waterMarkOptions.LogoEnabled = ImageCheckBox().IsChecked().GetBoolean();

        return true;
        

    }

    IAsyncOperation<SoftwareBitmap> WorkflowWatermarkSample::GetSoftwareBitmapFromFileAsync(StorageFile file)
    {
        co_await winrt::resume_background();
        
        auto fileStream = co_await file.OpenAsync(FileAccessMode::Read);
        auto decoder = co_await BitmapDecoder::CreateAsync(fileStream);
        SoftwareBitmap softwareBitmap = co_await decoder.GetSoftwareBitmapAsync();
        co_return softwareBitmap;
    }
}
