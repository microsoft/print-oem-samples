#pragma once
#include "XpsPageWatermarker.g.h"

// Note: Remove this static_assert after copying these generated source files to your project.
// This assertion exists to avoid compiling these generated source files directly.
static_assert(false, "Do not compile generated C++/WinRT source files directly");

namespace winrt::XpsUtil::implementation
{
    struct XpsPageWatermarker : XpsPageWatermarkerT<XpsPageWatermarker>
    {
        XpsPageWatermarker() = default;

        void SetWatermarkText(hstring const& text, double fontSize, double xRelativeOffset, double yRelativeOffset);
        void SetWatermarkImage(hstring const& imagePath, double dpiX, double dpiY, int32_t width, int32_t height);
        void SetWatermarkImageEnabled(bool enabled);
        void ApplyWatermarks(XpsUtil::XpsPageWrapper const& page);
    };
}
namespace winrt::XpsUtil::factory_implementation
{
    struct XpsPageWatermarker : XpsPageWatermarkerT<XpsPageWatermarker, implementation::XpsPageWatermarker>
    {
    };
}
