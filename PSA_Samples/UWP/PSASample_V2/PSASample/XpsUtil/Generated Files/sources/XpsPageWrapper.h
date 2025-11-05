#pragma once
#include "XpsPageWrapper.g.h"

// Note: Remove this static_assert after copying these generated source files to your project.
// This assertion exists to avoid compiling these generated source files directly.
static_assert(false, "Do not compile generated C++/WinRT source files directly");

namespace winrt::XpsUtil::implementation
{
    struct XpsPageWrapper : XpsPageWrapperT<XpsPageWrapper>
    {
        XpsPageWrapper() = default;

        XpsUtil::XpsPageWrapper Clone();
        Windows::Storage::Streams::IRandomAccessStream RenderPageToBMP();
    };
}
