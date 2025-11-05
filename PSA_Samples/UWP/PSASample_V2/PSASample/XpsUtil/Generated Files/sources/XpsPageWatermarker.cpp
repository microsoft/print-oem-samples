#include "pch.h"
#include "XpsPageWatermarker.h"
#include "XpsPageWatermarker.g.cpp"

// Note: Remove this static_assert after copying these generated source files to your project.
// This assertion exists to avoid compiling these generated source files directly.
static_assert(false, "Do not compile generated C++/WinRT source files directly");

namespace winrt::XpsUtil::implementation
{
    void XpsPageWatermarker::SetWatermarkText(hstring const& text, double fontSize, double xRelativeOffset, double yRelativeOffset)
    {
        throw hresult_not_implemented();
    }
    void XpsPageWatermarker::SetWatermarkImage(hstring const& imagePath, double dpiX, double dpiY, int32_t width, int32_t height)
    {
        throw hresult_not_implemented();
    }
    void XpsPageWatermarker::SetWatermarkImageEnabled(bool enabled)
    {
        throw hresult_not_implemented();
    }
    void XpsPageWatermarker::ApplyWatermarks(XpsUtil::XpsPageWrapper const& page)
    {
        throw hresult_not_implemented();
    }
}
