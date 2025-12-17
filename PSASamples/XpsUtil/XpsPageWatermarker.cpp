#include "pch.h"
#include "XpsPageWatermarker.h"
#include "XpsPageWatermarker.g.cpp"
#include <XpsPageWrapper.h>
#include <wil/result.h>
#include "strsafe.h"
#include "pathcch.h"

namespace winrt::XpsUtil::implementation
{
    /// <summary>
    /// Create a new XpsPageWatermarker. By default, no change will be applied to the page.
    /// </summary>
    XpsPageWatermarker::XpsPageWatermarker() : m_imagePath(L""), m_watermarkText(L"")
    {
        m_xpsFactory = create_instance<IXpsOMObjectFactory1>(guid_of<XpsOMObjectFactory>());
    }

    /// <summary>
    /// Apply the requested watermarks to the given page wrapper. This will irreversibly modify the underlying page.
    /// </summary>
    /// <param name="page"></param>
    void XpsPageWatermarker::ApplyWatermarks(XpsUtil::XpsPageWrapper const& page)
    {
        com_ptr<IXpsOMPage> xpsPage = get_self<winrt::XpsUtil::implementation::XpsPageWrapper>(page)->GetPage();

        ApplyWatermarksToXpsPage(xpsPage);
    }

    /// <summary>
    /// Apply the requested watermarks to the given IXpsOMPage. This irreversibly modifies the page.
    /// </summary>
    /// <param name="xpsPage"></param>
    void XpsPageWatermarker::ApplyWatermarksToXpsPage(com_ptr<IXpsOMPage> const& xpsPage)
    {
        if (!m_watermarkText.empty()) {
            AddWatermarkText(xpsPage);
        }

        if (m_imageEnabled && !m_imagePath.empty()) {
            AddWatermarkImage(xpsPage);
        }
    }

    /// <summary>
    /// Set the desired attributes for the watermark text
    /// </summary>
    /// <param name="text">String to overlay on the page</param>
    /// <param name="fontSize">Font size of the overlayed text</param>
    /// <param name="xRelativeOffset">Relative offset in the X axis (left-right) of the bottom left corner of the text. Double between 0 and 1.</param>
    /// <param name="yRelativeOffset">Relative offset in the Y axis (up-down) of the bottom left corner of the text. Double between 0 and 1.</param>
    void XpsPageWatermarker::SetWatermarkText(hstring const& text, double fontSize, double xRelativeOffset, double yRelativeOffset)
    {
        if (m_watermarkXRelativeOffset< 0 || m_watermarkXRelativeOffset > 1 || m_watermarkYRelativeOffset < 0 || m_watermarkYRelativeOffset > 1)
        {
            throw hresult_invalid_argument();
        }

        m_watermarkText = text;
        m_watermarkFontSize = fontSize;
        m_watermarkXRelativeOffset = xRelativeOffset;
        m_watermarkYRelativeOffset = yRelativeOffset;
    }

    void XpsPageWatermarker::SetWatermarkImage(hstring const& imagePath, double dpiX, double dpiY, int32_t width, int32_t height)
    {
        SetImageProperties(imagePath, dpiX, dpiY, width, height);
    }

    /// <summary>
    /// Sets whether to apply a watermark image. If false,
    /// </summary>
    /// <param name="enabled"></param>
    void XpsPageWatermarker::SetWatermarkImageEnabled(bool enabled)
    {
        m_imageEnabled = enabled;
    }

    void XpsPageWatermarker::AddWatermarkText(com_ptr<IXpsOMPage> const& page)
    {
        AddTextToPage(page, std::wstring(m_watermarkText), m_watermarkFontSize, m_watermarkXRelativeOffset, m_watermarkYRelativeOffset);
    }

    com_ptr<IXpsOMFontResource> XpsPageWatermarker::CreateFontResource(const std::wstring& fontFilepath)
    {
        try
        {
            // Create the font URI, the URI is the path in the XPS package.
            com_ptr<IOpcPartUri> fontUri;
            THROW_IF_FAILED(m_xpsFactory->CreatePartUri(L"/Resources/Fonts/WatermarkFont.odttf", fontUri.put()));

            // Load an existing font file to an IStream.
            com_ptr<IStream> fontStream;
            THROW_IF_FAILED(m_xpsFactory->CreateReadOnlyStreamOnFile(fontFilepath.c_str(), fontStream.put()));

            // Create the font resource.
            com_ptr<IXpsOMFontResource> fontResource;
            THROW_IF_FAILED(m_xpsFactory->CreateFontResource(fontStream.get(),
                XPS_FONT_EMBEDDING_NORMAL,
                fontUri.get(),
                FALSE, // isObfSourceStream
                fontResource.put()));
            return fontResource;
        }
        catch (...)
        {
            throw;
		}        
    }

    void XpsPageWatermarker::AddTextToPage(com_ptr<IXpsOMPage> const& xpsPage, _In_ std::wstring text, const com_ptr<IXpsOMFontResource>& fontResource, float fontSize, const com_ptr<IXpsOMSolidColorBrush>& colorBrush, XPS_POINT* origin)
    {
        try
        {
            // Create a new Glyphs object and set its properties.
            com_ptr<IXpsOMGlyphs> xpsGlyphs;
            THROW_IF_FAILED(m_xpsFactory->CreateGlyphs(fontResource.get(), xpsGlyphs.put()));
            THROW_IF_FAILED(xpsGlyphs->SetOrigin(origin));
            THROW_IF_FAILED(xpsGlyphs->SetFontRenderingEmSize(fontSize));
            THROW_IF_FAILED(xpsGlyphs->SetFillBrushLocal(colorBrush.get()));

            // Some properties are inter-dependent so they
            // must be changed by using a GlyphsEditor.
            com_ptr<IXpsOMGlyphsEditor> glyphsEditor;
            THROW_IF_FAILED(xpsGlyphs->GetGlyphsEditor(glyphsEditor.put()));
            THROW_IF_FAILED(glyphsEditor->SetUnicodeString(text.c_str()));
            THROW_IF_FAILED(glyphsEditor->ApplyEdits());

            // Add the new Glyphs object to the page
            com_ptr<IXpsOMVisualCollection> pageVisuals;
            THROW_IF_FAILED(xpsPage->GetVisuals(pageVisuals.put()));
            THROW_IF_FAILED(pageVisuals->Append(xpsGlyphs.get()));
        }
        catch (...)
        {
            throw;
        }
        
    }

    std::wstring GetObsolutePath(std::wstring const& relativePath)
    {
        try
        {
            wil::unique_hmodule currentModule;
            THROW_IF_WIN32_BOOL_FALSE(GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<LPCWSTR>(&GetObsolutePath), &currentModule));

            wchar_t filePath[MAX_PATH];
            DWORD res = GetModuleFileName(currentModule.get(), filePath, MAX_PATH);
            THROW_LAST_ERROR_IF(res == 0);

            std::wstring expectedPath = filePath;
            expectedPath = expectedPath.substr(0, expectedPath.find_last_of(L'\\') + 1);
            std::wstring fontFilePath = expectedPath + relativePath;
            return fontFilePath;
        }
        catch   (...)
        {
            throw;
		}      
    }

    void XpsPageWatermarker::AddTextToPage(com_ptr<IXpsOMPage> const& xpsPage, std::wstring const& text, double fontSize, double xRelativeOffset, double yRelativeOffset)
    {
        try
        {
            // Create the color brush for the font.
            XPS_COLOR xpsColorBodyText = {};
            xpsColorBodyText.colorType = XPS_COLOR_TYPE_SRGB;
            // Change the alpha value to make the watermark text transparent.
            xpsColorBodyText.value.sRGB.alpha = 0x40;

            com_ptr<IXpsOMSolidColorBrush> xpsTextColorBrush;
            THROW_IF_FAILED(m_xpsFactory->CreateSolidColorBrush(&xpsColorBodyText, nullptr, xpsTextColorBrush.put()));

            // Create the XPS_POINT for where the text begins.
            XPS_SIZE pageDimensions = { 0 };
            THROW_IF_FAILED(xpsPage->GetPageDimensions(&pageDimensions));
            XPS_POINT textOrigin = { pageDimensions.width * static_cast<float>(xRelativeOffset), pageDimensions.height * static_cast<float>(yRelativeOffset) };

            std::wstring fontPath = L"Fonts\\arial.ttf";
            size_t length = GetSystemWindowsDirectory(nullptr, 0);
            std::wstring pathString(length, 0);
            WCHAR fullPath[MAX_PATH];
            size_t directoryResult = GetSystemWindowsDirectory(pathString.data(), static_cast<UINT>(pathString.length()));

            THROW_LAST_ERROR_IF(directoryResult == 0);

            THROW_IF_FAILED(PathCchCombine(fullPath, ARRAYSIZE(fullPath), pathString.data(), fontPath.data()));

            auto fontResource = CreateFontResource(fullPath);

            AddTextToPage(xpsPage, text, fontResource, static_cast<float>(fontSize), xpsTextColorBrush, &textOrigin);
        }
        catch (...)
        {
            throw;
		}
    }

    void XpsPageWatermarker::AddWatermarkImage(com_ptr<IXpsOMPage> const& xpsPage)
    {
        try
        {
            XPS_SIZE pageDimensions{ };
            THROW_IF_FAILED(xpsPage->GetPageDimensions(&pageDimensions));
            AddImageToPage(xpsPage, pageDimensions);
        }
        catch (...)
        {
            throw;
        }
    }

    void XpsPageWatermarker::AddImageToPage(_In_ com_ptr<IXpsOMPage> const& xpsPage, _In_ const XPS_SIZE& pageDimensions)
    {
        try
        {
            std::wstring imageFileName = L"";
            double dpiX = 0.0L, dpiY = 0.0L;
            int imageHeight = 0, imageWidth = 0;

            if (GetImageProperties(imageFileName, dpiX, dpiY, imageWidth, imageHeight))
            {
                std::wstring imagePartName = L"Assets/Images/PrintSupportImage.jpg";
                std::wstring imageFileShortDescription = L"PSA Added Logo";

                //Dimensions of the image in pixels
                XPS_SIZE imageDim = { static_cast<float>(imageWidth), static_cast<float>(imageHeight) };
                //Image position on Page
                XPS_RECT rectForImage = { 0, 0, pageDimensions.width / 10, pageDimensions.height / 10 };

                com_ptr<IXpsOMImageResource> imageResource;
                THROW_IF_FAILED(CreateImageResource(GetObsolutePath(imageFileName), imagePartName, imageResource.put()));
                THROW_IF_FAILED(AddImageToVisualCollection(
                    xpsPage,
                    imageResource.get(),
                    &imageDim,
                    static_cast<float>(dpiX),
                    static_cast<float>(dpiY),
                    rectForImage,
                    imageFileShortDescription));
            }
        }
        catch (...)
        {
            throw;
        }        
    }

    void XpsPageWatermarker::SetImageProperties(_In_ hstring const& imagePath, _In_ double dpiX, _In_ double dpiY, _In_ int width, _In_ int height)
    {
        m_imagePath = std::wstring(imagePath);
        m_imageDpiX = dpiX;
        m_imageDpiY = dpiY;
        m_imageWidth = width;
        m_imageHeight = height;
    }

    bool XpsPageWatermarker::GetImageProperties(_Out_ std::wstring& imagePath, _Out_ double& dpiX, _Out_ double& dpiY, _Out_ int& width, _Out_ int& height)
    {
        imagePath = m_imagePath;
        dpiX = m_imageDpiX;
        dpiY = m_imageDpiY;
        width = m_imageWidth;
        height = m_imageHeight;

        // Sanity check
        if (!imagePath.empty() && imagePath[0] != L'\0' && dpiX != 0.0L && dpiY != 0.0L && width != 0 && height != 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    HRESULT XpsPageWatermarker::CreateImageResource(_In_ const std::wstring imageFileName, _In_ const std::wstring imagePartName, _Out_ IXpsOMImageResource** imageResource)
    {
        try
        {
            THROW_IF_FAILED(E_INVALIDARG, imageFileName.empty() || imagePartName.empty());

            com_ptr<IStream> imageStream;
            com_ptr<IOpcPartUri> imagePartUri;

            RETURN_IF_FAILED(m_xpsFactory->CreateReadOnlyStreamOnFile(imageFileName.c_str(), imageStream.put()));
            RETURN_IF_FAILED(m_xpsFactory->CreatePartUri(imagePartName.c_str(), imagePartUri.put()));

            XPS_IMAGE_TYPE imageType = XPS_IMAGE_TYPE_JPEG; // set to type of image being read in
            RETURN_IF_FAILED(m_xpsFactory->CreateImageResource(
                imageStream.get(),
                imageType,
                imagePartUri.get(),
                imageResource));
        }
        catch (...)
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        return S_OK;
    }

    HRESULT XpsPageWatermarker::AddImageToVisualCollection(
        com_ptr<IXpsOMPage> const& xpsPage, IXpsOMImageResource* imageResource, XPS_SIZE* imageDim, float dotsPerInchX,
        float dotsPerInchY, XPS_RECT rectForImage, std::wstring shortDescription)
    {
        try
        {
            RETURN_HR_IF_NULL(E_INVALIDARG, imageResource);

            XPS_RECT viewPort = { 0.0, 0.0, 0.0, 0.0 };
            XPS_RECT viewBox = { 0.0, 0.0, 0.0, 0.0 };

            com_ptr<IXpsOMPath> imageRectPath;
            com_ptr<IXpsOMImageBrush> imageBrush;
            com_ptr<IXpsOMVisualCollection> pageVisuals;

            // Describe image source dimensions and set viewbox to be the
            // entire image DPI width of image.
            //  Example:
            //    600 image pixels, 300 dpi -> 2 inches -> 2 * 96 = 192 DIP width
            viewBox.width = imageDim->width * 96.0f / dotsPerInchX;
            viewBox.height = imageDim->height * 96.0f / dotsPerInchY;

            // destination rectangle
            viewPort.x = rectForImage.x;
            viewPort.y = rectForImage.y;
            viewPort.width = rectForImage.width;
            viewPort.height = rectForImage.height;

            // Create the image brush.
            RETURN_IF_FAILED(m_xpsFactory->CreateImageBrush(imageResource, &viewBox, &viewPort, imageBrush.put()));

            // Create the path that describes the outline of the image on the page.
            RETURN_IF_FAILED(CreateRectanglePath(rectForImage, imageRectPath.put()));

            // Set the accessibility description for the path object as required.
            RETURN_IF_FAILED(imageRectPath->SetAccessibilityShortDescription(shortDescription.c_str()));

            // Set the image brush to be the fill brush for this path.
            RETURN_IF_FAILED(imageRectPath->SetFillBrushLocal(imageBrush.get()));

            // Get the list of visuals for this page...
            RETURN_IF_FAILED(xpsPage->GetVisuals(pageVisuals.put()));

            // ...and add the completed path to the list.
            RETURN_IF_FAILED(pageVisuals->Append(imageRectPath.get()));
        }
        catch (...)
        {
            return HRESULT_FROM_WIN32(GetLastError());
		}

        return S_OK;
    }

    HRESULT XpsPageWatermarker::CreateRectanglePath(_In_ const XPS_RECT& rect, _Out_ IXpsOMPath** rectPath)
    {
        try
        {
            RETURN_HR_IF_NULL(E_INVALIDARG, rectPath);

            com_ptr<IXpsOMGeometryFigure> rectFigure;
            com_ptr<IXpsOMGeometry> imageRectGeometry;
            com_ptr<IXpsOMGeometryFigureCollection> geomFigureCollection;

            // Define start point and three of the four sides of the rectangle.
            // The fourth side is implied by setting the path type to CLOSED.
            XPS_POINT startPoint = { rect.x, rect.y };
            XPS_SEGMENT_TYPE segmentTypes[3] = { XPS_SEGMENT_TYPE_LINE,
                                                XPS_SEGMENT_TYPE_LINE,
                                                XPS_SEGMENT_TYPE_LINE };
            FLOAT segmentData[6] = { rect.x, rect.y + rect.height,
                                    rect.x + rect.width, rect.y + rect.height,
                                    rect.x + rect.width, rect.y };
            BOOL segmentStrokes[3] = { TRUE, TRUE, TRUE };

            // Create a closed geometry figure using the three segments defined above.
            RETURN_IF_FAILED(m_xpsFactory->CreateGeometryFigure(&startPoint, rectFigure.put()));
            RETURN_IF_FAILED(rectFigure->SetIsClosed(TRUE));
            RETURN_IF_FAILED(rectFigure->SetIsFilled(TRUE));
            RETURN_IF_FAILED(rectFigure->SetSegments(3, 6, segmentTypes, segmentData, segmentStrokes));

            // Create a geometry that consists of the figure created above.
            RETURN_IF_FAILED(m_xpsFactory->CreateGeometry(imageRectGeometry.put()));
            RETURN_IF_FAILED(imageRectGeometry->GetFigures(geomFigureCollection.put()));
            RETURN_IF_FAILED(geomFigureCollection->Append(rectFigure.get()));

            // Create the path that consists of the geometry created above
            //  and return the pointer in the parameter passed in to the function.
            RETURN_IF_FAILED(m_xpsFactory->CreatePath(reinterpret_cast<IXpsOMPath**>(rectPath)));
            RETURN_IF_FAILED((*rectPath)->SetGeometryLocal(imageRectGeometry.get()));

            // The calling method will release IXpsOMPath when it is done with it.
        }
        catch (...)
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        return S_OK;
    }
}
