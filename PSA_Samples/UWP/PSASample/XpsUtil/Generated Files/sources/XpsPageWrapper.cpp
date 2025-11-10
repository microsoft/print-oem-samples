#include "pch.h"
#include "XpsPageWrapper.h"
#include "XpsPageWrapper.g.cpp"

// Note: Remove this static_assert after copying these generated source files to your project.
// This assertion exists to avoid compiling these generated source files directly.
static_assert(false, "Do not compile generated C++/WinRT source files directly");

namespace winrt::XpsUtil::implementation
{
    XpsUtil::XpsPageWrapper XpsPageWrapper::Clone()
    {
        throw hresult_not_implemented();
    }
    Windows::Storage::Streams::IRandomAccessStream XpsPageWrapper::RenderPageToBMP()
    {
        throw hresult_not_implemented();
    }
}
