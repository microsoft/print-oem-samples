#include "pch.h"
#include "XpsPageWrapper.h"
#include "XpsPageWrapper.g.cpp"
#include <shcore.h>
#include <strsafe.h>

// XpsRasterizer
#include <Windows.h>
#include <initguid.h>
#include <wincodec.h>
#include <XpsRasSvc.h>

using namespace winrt::Windows::Storage::Streams;

namespace winrt::XpsUtil::implementation
{
    XpsPageWrapper::XpsPageWrapper(com_ptr<IXpsOMPage> page) : m_page(page) { }

    void XpsPageWrapper::ConvertBMPtoStream(_In_ com_ptr<IUnknown> bitmap, _In_ com_ptr<IStream> bitmapStream)
    {
        auto wicFactory = winrt::create_instance<IWICImagingFactory>(CLSID_WICImagingFactory);

        com_ptr<IWICBitmapSource> bitmapSource;
        UINT width, height;
        bitmap->QueryInterface(IID_PPV_ARGS(bitmapSource.put()));
        bitmapSource->GetSize(&width, &height);

        com_ptr<IWICStream> wicStream;
        check_hresult(wicFactory->CreateStream(wicStream.put()));
        check_hresult(wicStream->InitializeFromIStream(bitmapStream.get()));

        com_ptr<IWICBitmapEncoder> encoder;
        check_hresult(wicFactory->CreateEncoder(GUID_ContainerFormatTiff, nullptr, encoder.put()));
        check_hresult(encoder->Initialize(wicStream.get(), WICBitmapEncoderNoCache));

        com_ptr<IWICColorContext> colorContext;
        check_hresult(wicFactory->CreateColorContext(colorContext.put()));
        check_hresult(colorContext->InitializeFromExifColorSpace(1));

        com_ptr<IWICBitmapFrameEncode> bitmapFrame;
        check_hresult(encoder->CreateNewFrame(bitmapFrame.put(), nullptr));
        check_hresult(bitmapFrame->Initialize(nullptr));
        check_hresult(bitmapFrame->SetSize(width, height));

        WICPixelFormatGUID pixelFormat = GUID_WICPixelFormat32bppPBGRA;
        check_hresult(bitmapFrame->SetPixelFormat(&pixelFormat));

        IWICColorContext* colorContexts[] = { colorContext.get() };
        check_hresult(bitmapFrame->SetColorContexts(_countof(colorContexts), colorContexts));

        check_hresult(bitmapFrame->WriteSource(bitmapSource.get(), nullptr));

        check_hresult(bitmapFrame->Commit());
        check_hresult(encoder->Commit());
    }

    XpsUtil::XpsPageWrapper XpsPageWrapper::Clone()
    {
        com_ptr<IXpsOMPage> clonedPage;
        m_page->Clone(clonedPage.put());

        return make<winrt::XpsUtil::implementation::XpsPageWrapper>(clonedPage);
    }

    Windows::Storage::Streams::IRandomAccessStream XpsPageWrapper::RenderPageToBMP()
    {
        auto rasFactory = winrt::create_instance<IXpsRasterizationFactory2>(CLSID_XPSRASTERIZER_FACTORY);

        com_ptr<IXpsRasterizer> rasterizer = nullptr;
        rasFactory->CreateRasterizer(m_page.get(), 96.0f, 96.0f, XPSRAS_RENDERING_MODE_ANTIALIASED,
            XPSRAS_RENDERING_MODE_ANTIALIASED, XPSRAS_PIXEL_FORMAT_32BPP_PBGRA_UINT_SRGB, XPSRAS_BACKGROUND_COLOR_OPAQUE, rasterizer.put());
        com_ptr<IWICBitmap> ppiBitmap;

        XPS_SIZE dim;
        winrt::check_hresult(m_page->GetPageDimensions(&dim));
        wchar_t tchStr[1000];
        StringCchPrintfW(tchStr, ARRAYSIZE(tchStr), L"\n GetPageDimensions called width:%x, height:%x", static_cast<int>(dim.width), static_cast<int>(dim.height));
        OutputDebugString(tchStr);

        HRESULT hr = rasterizer->RasterizeRect(
            0, 0, static_cast<int>(dim.width), static_cast<int>(dim.height), nullptr, ppiBitmap.put());
        if (SUCCEEDED(hr))
        {
            com_ptr<IStream> bmpStream;
            //check_hresult(CreateStreamOnHGlobal(nullptr, TRUE, bmpStream.put()));
            InMemoryRandomAccessStream randomAccessStream;
            winrt::check_hresult(CreateStreamOverRandomAccessStream(static_cast<IUnknown*>(winrt::get_abi(randomAccessStream)), IID_PPV_ARGS(bmpStream.put())));

            ConvertBMPtoStream(ppiBitmap, bmpStream);

            IRandomAccessStream rstream;
            check_hresult(CreateRandomAccessStreamOverStream(bmpStream.get(), BSOS_DEFAULT, guid_of<IRandomAccessStream>(), put_abi(rstream)));
            return rstream;
        }
        else
        {
            StringCchPrintfW(tchStr, ARRAYSIZE(tchStr), L"\n RasterizeRect failed hr:%x", hr);
            OutputDebugString(tchStr);
        }

        throw_hresult(E_FAIL);
    }
    com_ptr<IXpsOMPage> XpsPageWrapper::GetPage()
    {
        return m_page;
    }
}
