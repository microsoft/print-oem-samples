#include "pch.h"
#include "XpsSequentialDocument.h"
#include "XpsSequentialDocument.g.cpp"

// Note: Remove this static_assert after copying these generated source files to your project.
// This assertion exists to avoid compiling these generated source files directly.
static_assert(false, "Do not compile generated C++/WinRT source files directly");

namespace winrt::XpsUtil::implementation
{
    XpsSequentialDocument::XpsSequentialDocument(Windows::Graphics::Printing::Workflow::PrintWorkflowObjectModelSourceFileContent const& sourceFileContent)
    {
        throw hresult_not_implemented();
    }
    void XpsSequentialDocument::StartXpsOMGeneration()
    {
        throw hresult_not_implemented();
    }
    Windows::Storage::Streams::IInputStream XpsSequentialDocument::GetWatermarkedStream(XpsUtil::XpsPageWatermarker const& watermarker)
    {
        throw hresult_not_implemented();
    }
    XpsUtil::XpsPageWrapper XpsSequentialDocument::GetPage(uint32_t pageNumber)
    {
        throw hresult_not_implemented();
    }
    uint32_t XpsSequentialDocument::PageCount()
    {
        throw hresult_not_implemented();
    }
    winrt::event_token XpsSequentialDocument::PageAdded(Windows::Foundation::TypedEventHandler<XpsUtil::XpsSequentialDocument, uint32_t> const& handler)
    {
        throw hresult_not_implemented();
    }
    void XpsSequentialDocument::PageAdded(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }
    winrt::event_token XpsSequentialDocument::DocumentClosed(Windows::Foundation::TypedEventHandler<XpsUtil::XpsSequentialDocument, uint32_t> const& handler)
    {
        throw hresult_not_implemented();
    }
    void XpsSequentialDocument::DocumentClosed(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }
    winrt::event_token XpsSequentialDocument::XpsGenerationFailed(Windows::Foundation::TypedEventHandler<XpsUtil::XpsSequentialDocument, uint64_t> const& handler)
    {
        throw hresult_not_implemented();
    }
    void XpsSequentialDocument::XpsGenerationFailed(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }
}
