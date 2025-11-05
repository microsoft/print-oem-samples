#pragma once
#include "XpsSequentialDocument.g.h"

// Note: Remove this static_assert after copying these generated source files to your project.
// This assertion exists to avoid compiling these generated source files directly.
static_assert(false, "Do not compile generated C++/WinRT source files directly");

namespace winrt::XpsUtil::implementation
{
    struct XpsSequentialDocument : XpsSequentialDocumentT<XpsSequentialDocument>
    {
        XpsSequentialDocument() = default;

        XpsSequentialDocument(Windows::Graphics::Printing::Workflow::PrintWorkflowObjectModelSourceFileContent const& sourceFileContent);
        void StartXpsOMGeneration();
        Windows::Storage::Streams::IInputStream GetWatermarkedStream(XpsUtil::XpsPageWatermarker const& watermarker);
        XpsUtil::XpsPageWrapper GetPage(uint32_t pageNumber);
        uint32_t PageCount();
        winrt::event_token PageAdded(Windows::Foundation::TypedEventHandler<XpsUtil::XpsSequentialDocument, uint32_t> const& handler);
        void PageAdded(winrt::event_token const& token) noexcept;
        winrt::event_token DocumentClosed(Windows::Foundation::TypedEventHandler<XpsUtil::XpsSequentialDocument, uint32_t> const& handler);
        void DocumentClosed(winrt::event_token const& token) noexcept;
        winrt::event_token XpsGenerationFailed(Windows::Foundation::TypedEventHandler<XpsUtil::XpsSequentialDocument, uint64_t> const& handler);
        void XpsGenerationFailed(winrt::event_token const& token) noexcept;
    };
}
namespace winrt::XpsUtil::factory_implementation
{
    struct XpsSequentialDocument : XpsSequentialDocumentT<XpsSequentialDocument, implementation::XpsSequentialDocument>
    {
    };
}
