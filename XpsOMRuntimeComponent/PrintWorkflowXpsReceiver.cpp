/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This is a sample implementation of the XPS Object Model Windows Runtime component. It shows how to create a
*   IPrintWorkflowXpsReceiver implementation, which is called on a different thread to the App, and receives the document object model parts.
*   It allows the content to be manipulated before being sent on to the printer.
*
*   Because the PrintWorkflowXpsReceiver methods are not called on the App thread, they must return HRESULTS and not throw.
*   The PrintWorkflowObjectModelSourceFileContentNative methods can throw because exceptioms from those can be caught and handled.
*/
#include "pch.h"
#include "PrintWorkflowXpsReceiver.h"
#include <strsafe.h>

using namespace Windows::Foundation;
using namespace Windows::Globalization;
using namespace Windows::UI::Notifications;
using namespace Windows::Data::Xml::Dom;

namespace XpsOMRuntimeComponent
{
    // Start of IPrintWorkflowXpsReceiver interface methods

    /*  PrintWorkflowXpsReceiver::PrintWorkflowXpsReceiver
    *
    *  Constructor for PrintWorkflowXpsReceiver.
    *
    *  Arguments:
    *        _In_ ComPtr<IXpsDocumentPackageTarget> &xpsDocumentPackageTarget   XPS document package target
    *
    *  Returns:
    *       Nothing
    */
    PrintWorkflowXpsReceiver::PrintWorkflowXpsReceiver(_In_ ComPtr<IXpsDocumentPackageTarget> &xpsDocumentPackageTarget)
        : _referenceCount(1), _xpsOMDocumentPackageTarget(xpsDocumentPackageTarget), _closeHRESULT(S_OK), _imagePath(nullptr),
          _imageDpiX(0.0L), _imageDpiY(0.0L), _imageWidth(0), _imageHeight(0)
    {
        _xpsOMGenerationEvent = ::CreateEvent(nullptr, true, false, nullptr);
        _xpsOMDocumentPackageTarget->GetXpsOMFactory(&_xpsOMObjectFactory);
    }

    /*  PrintWorkflowXpsReceiver::SetDocumentSequencePrintTicket
    *
    *  Set the PS document sequence Print Ticket as a stream.
    *
    *  Arguments:
    *       _In_opt_ IStream *documentSequencePrintTicket   XPS document sequence Print Ticket as a stream
    *
    *  Returns:
    *       S_OK              Success
    */
    HRESULT STDMETHODCALLTYPE PrintWorkflowXpsReceiver::SetDocumentSequencePrintTicket(_In_opt_ IStream *documentSequencePrintTicket)
    {
        _documentSequencePrintTicketStream = documentSequencePrintTicket;

        return S_OK;
    }

    /*  PrintWorkflowXpsReceiver::SetDocumentSequenceUri
    *
    *  Set the document sequence Uri.
    *
    *  Arguments:
    *       _In_ PCWSTR      documentSequenceUri  XPS Document sequence Uri.
    *
    *  Returns:
    *       E_INVALIDARG      Invalid documentSequenceUri
    *       S_OK              Success
    *       Other             HRESULT of error
    */
    HRESULT STDMETHODCALLTYPE PrintWorkflowXpsReceiver::SetDocumentSequenceUri(_In_ PCWSTR documentSequenceUri)
    {
        RETURN_INVALID_ARGUMENT_IF_NULL(documentSequenceUri);

        ComPtr<IOpcPartUri> documentSequencePartName;
        RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreatePartUri(documentSequenceUri, &documentSequencePartName));
        ComPtr<IOpcPartUri> spDiscardControlPartName;
        RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreatePartUri(L"/DiscardControl.xml", &spDiscardControlPartName));
        // Create the XPS OM Package Writer from the DocumentPackageTarget
        RETURN_HR_IF_FAILED(_xpsOMDocumentPackageTarget->GetXpsOMPackageWriter(documentSequencePartName.Get(), spDiscardControlPartName.Get(), &_xpsOMPackageWriter));

        return S_OK;
    }

    /*  PrintWorkflowXpsReceiver::AddDocumentData
    *
    *  Add the document data.
    *
    *  Arguments:
    *       _In_ UINT32       documentId            XPS document ID
    *       _In_opt_ IStream *documentPrintTicket   XPS Document Print Ticket as a stream
    *       _In_ PCWSTR       documentUri           XPS Document sequence Uri
    *
    *  Returns:
    *       E_INVALIDARG      Invalid documentUri
    *       S_OK              Success
    *       Other             HRESULT of error
    */
    HRESULT STDMETHODCALLTYPE PrintWorkflowXpsReceiver::AddDocumentData(_In_ UINT32 documentId,
                                                                        _In_opt_ IStream *documentPrintTicket,
                                                                        _In_ PCWSTR documentUri
    )
    {
        RETURN_INVALID_ARGUMENT_IF_NULL(documentUri);

        ComPtr<IOpcPartUri> documentPartUri;
        RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreatePartUri(documentUri, &documentPartUri));

        // Start writing new document (as soon as received) 
        RETURN_HR_IF_FAILED(_xpsOMPackageWriter->StartNewDocument(documentPartUri.Get(), nullptr, nullptr, nullptr, nullptr));

        return S_OK;
    }

    /*  PrintWorkflowXpsReceiver::AddPage
    *
    *  Add the document data.
    *
    *  Arguments:
    *       _In_ UINT32                   documentId      XPS document ID
    *       _In_ UINT32                   pageId          XPS page ID
    *       _In_opt_ IXpsOMPageReference *pageReference   XPS page reference
    *       _In_ PCWSTR                   documentUri     XPS page Uri
    *
    *  Returns:
    *       E_INVALIDARG      Invalid pageUri
    *       S_OK              Success
    *       Other             HRESULT of error
    */
    HRESULT STDMETHODCALLTYPE PrintWorkflowXpsReceiver::AddPage(_In_ UINT32 documentId,
                                                                _In_ UINT32 pageId,
                                                                _In_opt_ IXpsOMPageReference *pageReference,
                                                                _In_ PCWSTR pageUri
    )
    {
        RETURN_INVALID_ARGUMENT_IF_NULL(pageUri);

        try
        {
            ComPtr<IXpsOMPageReference> xpsOMPageReference(pageReference);

            ComPtr<IOpcPartUri> opcPagePartUri;
            RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreatePartUri(pageUri, &opcPagePartUri));

            // Get the page and its dimensions
            ComPtr<IXpsOMPage> xpsOMPage;
            RETURN_HR_IF_FAILED(xpsOMPageReference->GetPage(&xpsOMPage));
            XPS_SIZE pageDimensions = { 0 };
            RETURN_HR_IF_FAILED(xpsOMPage->GetPageDimensions(&pageDimensions));

            // Add a logo image to the page if one was provided
            AddLogoImageToPage(xpsOMPage, pageDimensions);

            // Add watermark text to page if one was provided 
            AddWatermarkTextToPage(xpsOMPage, pageDimensions);

            // Finished modifying, so add page to package
            RETURN_HR_IF_FAILED(_xpsOMPackageWriter->AddPage(xpsOMPage.Get(), &pageDimensions, nullptr, nullptr, nullptr, nullptr));
        }
        catch (Exception^ ex)
        {
            SendToastNotification("The Background Print Workflow Receiver encountered an exception: " + ex->Message, nullptr);
        }

        return S_OK;
    }

    /*  PrintWorkflowXpsReceiver::SendToastNotification
    *
    *  Handy debugging function to allow background task to display messages.
    *
    *  Arguments:
    *       String^ toastMessage - text message to be display in the toast
    *
    *  Returns:
    *       Nothing
    */
    void  PrintWorkflowXpsReceiver::SendToastNotification(String^ toastMessage, String ^ image)
    {
        if (!suppressToast)
        {
            // Get the time
            Calendar^ calendar = ref new Windows::Globalization::Calendar;
            DateTime dt = calendar->GetDateTime();
            DateTimeFormatting::DateTimeFormatter^ dtf = ref new DateTimeFormatting::DateTimeFormatter("shortdate longtime");
            String^ dateTimeAsString = dtf->Format(dt);
            auto notificationXml = ToastNotificationManager::GetTemplateContent(ToastTemplateType::ToastImageAndText01);
            // Find the "text" node and add the message, prefixed by the time
            auto toastElements = notificationXml->GetElementsByTagName("text");
            toastElements->GetAt(0)->AppendChild(notificationXml->CreateTextNode(dateTimeAsString + " " + toastMessage));
            // If an image has been specified use that else falls back to a default
            if (image == nullptr || image->Length() == 0)
            {
                image = L"Assets\\smiley.jpg";
            }

            // Fnd the image node and insert the required image path
            auto imageElement = notificationXml->GetElementsByTagName("image");
            imageElement->GetAt(0)->Attributes->GetAt(1)->NodeValue = image;
            // Create and display the toast using the filled-out template
            auto toastNotification = ref new ToastNotification(notificationXml);
            ToastNotificationManager::CreateToastNotifier()->Show(toastNotification);
        }
    }

    /*  PrintWorkflowXpsReceiver::Close
    *
    *  Called when the whole XPS package is ready to be printed.
    *
    *  Arguments:
    *       None
    *
    *  Returns:
    *       S_OK              Success
    *       Other             HRESULT of error
    */
    HRESULT STDMETHODCALLTYPE PrintWorkflowXpsReceiver::Close()
    {
        // When this callback comes, the whole XPS is ready, start printing it.
        _closeHRESULT = _xpsOMPackageWriter->Close();
        // Signal the app that the Close() has finished and if successful, the whole package has been written
        SetEvent(_xpsOMGenerationEvent);
        return _closeHRESULT;
    }

    /*  PrintWorkflowXpsReceiver::QueryInterface
    *
    *  COM QueryInterface which will get IUnknown or IPrintWorkflowXpsReceiver
    *
    *  Arguments:
    *      _In_ REFIID                             rii        Required interface ID
    *      _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject  COM object returned  
    *
    *  Returns:
    *       S_OK              Success
    *       E_NOINTERFACE     Requested interface not supported
    */

    HRESULT STDMETHODCALLTYPE PrintWorkflowXpsReceiver::QueryInterface(_In_ REFIID riid,
                                                                       _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
    {
        RETURN_INVALID_ARGUMENT_IF_NULL(ppvObject);

        if (riid == __uuidof(IUnknown))
        {
            *ppvObject = reinterpret_cast<IUnknown*>(this);
        }
        else if (riid == __uuidof(IPrintWorkflowXpsReceiver))
        {
            *ppvObject = reinterpret_cast<IPrintWorkflowXpsReceiver*>(this);
        }
        else
        {
            *ppvObject = nullptr;
            return E_NOINTERFACE;
        }

        AddRef();

        return S_OK;
    }

    /*  PrintWorkflowXpsReceiver::AddRef
    *
    *  COM IUnknown AddRef method to increase reference count on object
    *
    *  Arguments:
    *      None
    *
    *  Returns:
    *       ULONG  The new reference count. Value is intended to be used only for test purposes.
    */
    ULONG STDMETHODCALLTYPE PrintWorkflowXpsReceiver::AddRef(void)
    {
        InterlockedIncrement(&_referenceCount);
        return S_OK;
    }

    /*  PrintWorkflowXpsReceiver::Release
    *
    *  COM IUnknown Release to decrease reference count on object, and delete if it hits zero
    *
    *  Arguments:
    *      None
    *
    *  Returns:
    *       ULONG  The new reference count. Value is intended to be used only for test purposes.
    */
    ULONG STDMETHODCALLTYPE PrintWorkflowXpsReceiver::Release(void)
    {
        ULONG count = InterlockedDecrement(&_referenceCount);
        if (count == 0)
        {
            delete this;
        }

        return count;
    }
    // End of IPrintWorkflowXpsReceiver interface methods

    // Start of customizable utility methods to wait until completion, and enable data to be passed in

    /*  PrintWorkflowXpsReceiver::WaitForOMGenerationCompletion
    *
    *  Wait until the Object Model document generation is complete.
    *
    *  Arguments:
    *      None
    *
    *  Returns:
    *       bool              True if wait object successfully signalled AND the Close() HRESULT was a success
    */
    bool PrintWorkflowXpsReceiver::WaitForOMGenerationCompletion()
    {
        bool succeeded = false;
        DWORD waitResult = WaitForSingleObject(_xpsOMGenerationEvent, INFINITE);
        if (waitResult == WAIT_OBJECT_0  &&  SUCCEEDED(_closeHRESULT))
        {
            succeeded = true;
        }

        return succeeded;
    }

    /*  PrintWorkflowXpsReceiver::CreateImageResource
    *
    *  Create the image resource which will be added to a page.
    *
    *  Arguments:
    *      _In_ LPCWSTR                imageFileName    Image file name 
    *      _In_ LPCWSTR                imagePartName    Image part Uri
    *      _Out_ IXpsOMImageResource **imageResource    And XPS image resource
    *
    *  Returns:
    *       E_INVALIDARG      Invalid imageFileName or imagePartName
    *       S_OK              Success
    *       Other             HRESULT of error
    */
    HRESULT PrintWorkflowXpsReceiver::CreateImageResource(_In_ LPCWSTR imageFileName,
                                                          _In_ LPCWSTR imagePartName,
                                                          _Out_ IXpsOMImageResource **imageResource)
    {
        RETURN_INVALID_ARGUMENT_IF_NULL(imageFileName);
        RETURN_INVALID_ARGUMENT_IF_NULL(imagePartName);

        ComPtr<IStream>     imageStream; // the resulting image stream
        ComPtr<IOpcPartUri> imagePartUri;
        XPS_RECT    rect = { 0.0f, 0.0f, 0.0f, 0.0f }; // set to image size

        RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreateReadOnlyStreamOnFile(imageFileName, &imageStream));
        RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreatePartUri(imagePartName, &imagePartUri));

        XPS_IMAGE_TYPE imageType = XPS_IMAGE_TYPE_JPEG; // set to type of image being read in
        RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreateImageResource(imageStream.Get(),
                                                                     imageType,
                                                                     imagePartUri.Get(),
                                                                     imageResource));
        // imageResource can now be used by other parts in the XPS OM.
        return S_OK;
    }

    /*  PrintWorkflowXpsReceiver::AddLogoImageToPage
    *
    *  If the logo source image is specified, add a logo to the page.
    *
    *  Arguments:
    *      _In_ ComPtr<IXpsOMPage>  &xpsOMPage            XPS Page Object Model
    *      _In_ const XPS_SIZE      &pageDimensions       Page dimensions
    *
    *  Returns:
    *       S_OK              Success
    *       Other             HRESULT of error
    */
    HRESULT PrintWorkflowXpsReceiver::AddLogoImageToPage(_In_ ComPtr<IXpsOMPage>  &xpsOMPage,
                                                         _In_ const XPS_SIZE &pageDimensions)
    {
        // Add an image to the page, if one was provided

        LPCWSTR imageFileName = nullptr; // The location has to be accessible to this process like, for example, "Assets\\contoso_logo.jpg"
        double dpiX = 0.0L, dpiY = 0.0L;
        int imageHeight = 0, imageWidth = 0;

        // Only add image if path isn't empty and dimensions make sense
        if (GetImageProperties(imageFileName, dpiX, dpiY, imageWidth, imageHeight))
        {
            LPCWSTR imagePartName = L"/Resources/Images/WorkflowImage.jpg";
            LPCWSTR imageFileShortDescription = L"Workflow Added Logo";
            // Dimensions of the image in pixels
            XPS_SIZE imageWidthAndHeight = { static_cast<FLOAT>(imageWidth), static_cast<FLOAT>(imageHeight) };
            // Image position on page
            XPS_RECT rectForImage = { 0, 0, pageDimensions.width / 10, pageDimensions.height / 10 };
            ComPtr<IXpsOMImageResource> imageResource;
            RETURN_HR_IF_FAILED(CreateImageResource(imageFileName, imagePartName, imageResource.GetAddressOf()));
            RETURN_HR_IF_FAILED(AddImageToVisualCollection(xpsOMPage,imageResource.Get(),&imageWidthAndHeight,
                                                           static_cast<FLOAT>(dpiX), static_cast<FLOAT>(dpiY),
                                                           rectForImage, imageFileShortDescription));
        }

        return S_OK;
    }

    /*  PrintWorkflowXpsReceiver::AddImageToVisualCollection
    *
    *  If the source image is available in the XPS OM as an image resource, 
    *  it can be used to create an image brush object and added as the fill brush to the path
    *  that describes the image location and size in the page.
    *
    *  Arguments:
    *      _In_ ComPtr<IXpsOMPage>  &xpsOMPage            XPS Page Object Model
    *      _In_ IXpsOMImageResource *imageResource        Image resource to add to the page
    *      _In_ XPS_SIZE            *imageWidthAndHeight  Pixel width and height of image
    *      _In_ FLOAT                dotsPerInchX         Image X resolution in dots per inch    
    *      _In_ FLOAT                dotsPerInchY         Image XY resolution in dots per inch  
    *      _In_ XPS_RECT             rect                 Image size and location
    *      _In_ LPCWSTR              shortDescription     Short text description of image
    *
    *  Returns:
    *       E_INVALIDARG      Invalid imageResource
    *       S_OK              Success
    *       Other             HRESULT of error
    */
    HRESULT PrintWorkflowXpsReceiver::AddImageToVisualCollection(
                                                                 _In_ ComPtr<IXpsOMPage> &xpsOMPage,
                                                                 _In_ IXpsOMImageResource *imageResource,
                                                                 _In_ XPS_SIZE *imageWidthAndHeight,
                                                                 _In_ FLOAT dotsPerInchX,
                                                                 _In_ FLOAT dotsPerInchY,
                                                                 _In_ XPS_RECT rect,
                                                                 _In_ LPCWSTR shortDescription)
    {
        RETURN_INVALID_ARGUMENT_IF_NULL(imageResource);

        // initialize viewport values 
        XPS_RECT    viewPort = { 0.0,0.0,0.0,0.0 };

        // initialize viewbox values
        XPS_RECT    viewBox = { 0.0,0.0,0.0,0.0 };

        // These are part of this code example.
        ComPtr<IXpsOMPath> imageRectPath;
        ComPtr<IXpsOMImageBrush> imageBrush;
        ComPtr<IXpsOMVisualCollection> pageVisuals;

        // Describe image source dimensions and set viewbox to be the 
        // entire image DIP width of image. 
        //  Example: 
        //    600 image pixels, 300 dpi -> 2 inches -> 2 * 96 = 192 DIP width
        viewBox.width = imageWidthAndHeight->width * 96.0f / dotsPerInchX;
        viewBox.height = imageWidthAndHeight->height * 96.0f / dotsPerInchY;

        // destination rectangle
        viewPort.x = rect.x;
        viewPort.y = rect.y;
        viewPort.width = rect.width;
        viewPort.height = rect.height;

        // Create the image brush.
        RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreateImageBrush(imageResource, &viewBox, &viewPort, imageBrush.GetAddressOf()));

        // Create the path that describes the outline of the image on the page.
        RETURN_HR_IF_FAILED(CreateRectanglePath(rect, imageRectPath.GetAddressOf()));

        // Set the accessibility description for the path object as required.
        RETURN_HR_IF_FAILED(imageRectPath->SetAccessibilityShortDescription(shortDescription));

        // Set the image brush to be the fill brush for this path.
        RETURN_HR_IF_FAILED(imageRectPath->SetFillBrushLocal(imageBrush.Get()));

        // Get the list of visuals for this page...
        RETURN_HR_IF_FAILED(xpsOMPage->GetVisuals(&pageVisuals));

        // ...and add the completed path to the list.
        RETURN_HR_IF_FAILED(pageVisuals->Append(imageRectPath.Get()));

        return S_OK;
    }

    /*  PrintWorkflowXpsReceiver::CreateRectanglePath
    *
    *  Takes an XPS_RECT structure and creates a rectangular path, which it is up to the caller to free.
    *
    *  Arguments:
    *      _In_  XPS_RECT     &rect         Input rectangle
    *      _Out_ IXpsOMPath  **rectPath     Returned path
    *
    *  Returns:
    *       E_INVALIDARG      Invalid rectPath
    *       S_OK              Success
    *       Other             HRESULT of error
    */
    HRESULT PrintWorkflowXpsReceiver::CreateRectanglePath(_In_  const XPS_RECT &rect,
                                                          _Out_ IXpsOMPath **rectPath)
    {
        RETURN_INVALID_ARGUMENT_IF_NULL(rectPath);

        ComPtr<IXpsOMGeometryFigure> rectFigure;
        ComPtr<IXpsOMGeometry> imageRectGeometry;
        ComPtr<IXpsOMGeometryFigureCollection> geomFigureCollection;

        // Define start point and three of the four sides of the rectangle.
        // The fourth side is implied by setting the path type to CLOSED.
        XPS_POINT            startPoint = {rect.x, rect.y};
        XPS_SEGMENT_TYPE     segmentTypes[3] = {XPS_SEGMENT_TYPE_LINE,
                                                XPS_SEGMENT_TYPE_LINE,
                                                XPS_SEGMENT_TYPE_LINE};
        FLOAT segmentData[6] = {   rect.x,              rect.y + rect.height,
                                   rect.x + rect.width, rect.y + rect.height,
                                   rect.x + rect.width, rect.y};
        BOOL segmentStrokes[3] = {TRUE, TRUE, TRUE};

        // Create a closed geometry figure using the three segments defined above.
        RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreateGeometryFigure(&startPoint, &rectFigure));
        RETURN_HR_IF_FAILED(rectFigure->SetIsClosed(TRUE));
        RETURN_HR_IF_FAILED(rectFigure->SetIsFilled(TRUE));
        RETURN_HR_IF_FAILED(rectFigure->SetSegments(3, 6, segmentTypes, segmentData, segmentStrokes));

        // Create a geometry that consists of the figure created above.
        RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreateGeometry(&imageRectGeometry));
        RETURN_HR_IF_FAILED(imageRectGeometry->GetFigures(&geomFigureCollection));
        RETURN_HR_IF_FAILED(geomFigureCollection->Append(rectFigure.Get()));

        // Create the path that consists of the geometry created above
        //  and return the pointer in the parameter passed in to the function.
        RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreatePath(reinterpret_cast<IXpsOMPath**>(rectPath)));
        RETURN_HR_IF_FAILED((*rectPath)->SetGeometryLocal(imageRectGeometry.Get()));

        // The calling method will release IXpsOMPath when it is done with it.

        return S_OK;
    }

    /*  PrintWorkflowXpsReceiver::AddWatermarkTextToPage
    *
    *  Add watermark text to the page.
    *
    *  Arguments:
    *      _In_ ComPtr<IXpsOMPage>  &xpsOMPage            XPS Page Object Model
    *      _In_ const XPS_SIZE      &pageDimensions       Page dimensions
    *
    *  Returns:
    *       S_OK              Success
    *       Other             HRESULT of error
    */
    HRESULT PrintWorkflowXpsReceiver::AddWatermarkTextToPage(_In_ ComPtr<IXpsOMPage>  &xpsOMPage,
                                                             _In_ const XPS_SIZE &pageDimensions)
    {
        LPCWSTR watermarkText = GetWatermarkText();
        if (watermarkText != nullptr  &&  *watermarkText != L'\0')
        {
            ComPtr<IXpsOMSolidColorBrush> xpsTextColorBrush;
            RETURN_HR_IF_FAILED(CreateBrush(xpsTextColorBrush.GetAddressOf()));
            ComPtr<IXpsOMFontResource> fontResource;
            // Replace this, and the application Assets, with a sensible font you are licensed to use, e.g. ARIALUNI.TTF 
            LPCWSTR fontFileName = L"Assets\\Symbols.ttf";
            RETURN_HR_IF_FAILED(CreateFont(fontFileName, fontResource.GetAddressOf()));

            float fontEmSize = 90.0;
            XPS_POINT originForTextDrawing = { pageDimensions.width / 25, pageDimensions.height / 2 };
            AddTextToPage(fontResource.Get(), fontEmSize, xpsTextColorBrush.Get(), &originForTextDrawing, watermarkText, xpsOMPage.Get());
        }

        return S_OK;
    }

    /*  PrintWorkflowXpsReceiver::AddTextToPage
    *
    *  Write Text to an XPS OM Page
    *
    *  Arguments:
    *       _In_    IXpsOMFontResource    *xpsFont        The font resource to use for this run
    *       _In_    float                  fontEmSize     The font size
    *       _In_    IXpsOMSolidColorBrush *xpsBrush       The solid color brush to use for the font
    *       _In_    XPS_POINT             *origin         The starting location of this glyph run
    *       _In_    LPCWSTR                unicodeString  The text to use for this run
    *       _Inout_ IXpsOMPage            *xpsOMPage      The page on which to write this glyph run
    *
    *  Returns:
    *       E_INVALIDARG      Invalid xpsFont, xpsBrush, origin or xpsOMPage
    *       S_OK              Success
    *       Other             HRESULT of error
    */
    HRESULT PrintWorkflowXpsReceiver::AddTextToPage(
            _In_    IXpsOMFontResource    *xpsFont,
            _In_    float                  fontEmSize,
            _In_    IXpsOMSolidColorBrush *xpsBrush,
            _In_    XPS_POINT             *origin,
            _In_    LPCWSTR                unicodeString,
            _Inout_ IXpsOMPage            *xpsOMPage
        )
    {
        RETURN_INVALID_ARGUMENT_IF_NULL(xpsFont);
        RETURN_INVALID_ARGUMENT_IF_NULL(xpsBrush);
        RETURN_INVALID_ARGUMENT_IF_NULL(origin);
        RETURN_INVALID_ARGUMENT_IF_NULL(xpsOMPage);

        XPS_POINT glyphsOrigin = { origin->x, origin->y };
        ComPtr<IXpsOMGlyphsEditor> glyphsEditor;
        ComPtr<IXpsOMGlyphs> xpsGlyphs;
        ComPtr<IXpsOMVisualCollection> pageVisuals;

        // Create a new Glyphs object and set its properties.
        RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreateGlyphs(xpsFont, &xpsGlyphs));
        RETURN_HR_IF_FAILED(xpsGlyphs->SetOrigin(&glyphsOrigin));
        RETURN_HR_IF_FAILED(xpsGlyphs->SetFontRenderingEmSize(fontEmSize));
        RETURN_HR_IF_FAILED(xpsGlyphs->SetFillBrushLocal(xpsBrush));

        // Some properties are inter-dependent so they
        //    must be changed by using a GlyphsEditor.
        RETURN_HR_IF_FAILED(xpsGlyphs->GetGlyphsEditor(&glyphsEditor));
        RETURN_HR_IF_FAILED(glyphsEditor->SetUnicodeString(unicodeString));
        RETURN_HR_IF_FAILED(glyphsEditor->ApplyEdits());

        // Add the new Glyphs object to the page
        RETURN_HR_IF_FAILED(xpsOMPage->GetVisuals(&pageVisuals));
        RETURN_HR_IF_FAILED(pageVisuals->Append(xpsGlyphs.Get()));

        return S_OK;
    }


    /*  PrintWorkflowXpsReceiver::CreateFont
    *
    *  Takes the name of the font file and returns a font resource.
    *
    *  Arguments:
    *      _In_  LPCWSTR                 fontFileName        Font file path including name
    *      _Out_ IXpsOMFontResource    **fontResource        XPS font resources
    *
    *  Returns:
    *       E_INVALIDARG      Invalid rect
    *       S_OK              Success
    *       Other             HRESULT of error
    */
    HRESULT PrintWorkflowXpsReceiver::CreateFont(_In_ LPCWSTR fontFileName,
        _Out_ IXpsOMFontResource **fontResource)
    {
        RETURN_INVALID_ARGUMENT_IF_NULL(fontFileName);
        RETURN_INVALID_ARGUMENT_IF_NULL(fontResource);

        WCHAR guidString[MAX_GUID_STRING_LEN] = { 0 };
        WCHAR uriString[INTERNET_MAX_PATH_LENGTH] = { 0 };
        ComPtr<IStream> fontStream;
        ComPtr<IOpcPartUri> fontUri;

        // Create font stream.
        RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreateReadOnlyStreamOnFile(fontFileName, &fontStream));

        // Create new obfuscated part name for this resource using a GUID.
        GUID fontNameGuid = { 0 };
        RETURN_HR_IF_FAILED(CoCreateGuid(&fontNameGuid));
        RETURN_HR_IF_FAILED(StringFromGUID2(fontNameGuid, guidString, ARRAYSIZE(guidString)));

        // Create a URI string for this font resource that will place 
        //  the font part in the /Resources/Fonts folder of the package.
        wcscpy_s(uriString, ARRAYSIZE(uriString), L"/Resources/Fonts/");

        // Create the part name using the GUID string as the name and 
        //  ".odttf" as the extension GUID string start and ends with 
        //  curly braces so they are removed.
        wcsncat_s(uriString, ARRAYSIZE(uriString),
            guidString + 1, wcslen(guidString) - 2);
        wcscat_s(uriString, ARRAYSIZE(uriString), L".odttf");

        // Create the font URI interface.
        RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreatePartUri(uriString, &fontUri));
        // Create the font resource.
        RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreateFontResource(fontStream.Get(),
            XPS_FONT_EMBEDDING_OBFUSCATED,
            fontUri.Get(),
            FALSE,     // isObfSourceStream
            fontResource));

        return S_OK;
    }

    /*  PrintWorkflowXpsReceiver::CreateBrush
    *
    *  Returns a new brush.
    *
    *  Arguments:
    *      _Out_ IXpsOMSolidColorBrush **xpsTextColorBrush   XPS solid color brush
    *
    *  Returns:
    *       E_INVALIDARG      Invalid rect
    *       S_OK              Success
    *       Other             HRESULT of error
    */
    HRESULT PrintWorkflowXpsReceiver::CreateBrush(
        _Out_ IXpsOMSolidColorBrush **xpsTextColorBrush)
    {
        RETURN_INVALID_ARGUMENT_IF_NULL(xpsTextColorBrush);

        XPS_COLOR xpsColorBodyText;

        // Create the brush to use for the font.
        xpsColorBodyText.colorType = XPS_COLOR_TYPE_SRGB;
        xpsColorBodyText.value.sRGB.alpha = 0x40;
        xpsColorBodyText.value.sRGB.red = 0x00;
        xpsColorBodyText.value.sRGB.green = 0x00;
        xpsColorBodyText.value.sRGB.blue = 0x00;

        RETURN_HR_IF_FAILED(_xpsOMObjectFactory->CreateSolidColorBrush(&xpsColorBodyText,
            NULL, // This color type does not use a color profile resource.             
            xpsTextColorBrush));

        return S_OK;
    }

    /*  PrintWorkflowXpsReceiver::SetWatermarkText
    *
    *  Saves the watermark text to be added to each page.
    *
    *  Arguments:
    *      _In_ LPCWSTR   watermarkText        Watermark text to be added to each page
    *
    *  Returns:
    *       Nothing
    */
    void PrintWorkflowXpsReceiver::SetWatermarkText(_In_ LPCWSTR watermarkText)
    {
        _watermarkText = std::wstring(watermarkText);
    }

    /*  PrintWorkflowXpsReceiver::GetWatermarkText
    *
    *  Saves the watermark text to be added to each page.
    *
    *  Arguments:
    *      None
    *
    *  Returns:
    *       LPCWSTR - Watermark text to be added to each page
    */
    LPCWSTR PrintWorkflowXpsReceiver::GetWatermarkText()
    {
        return _watermarkText.c_str();
    }

    /*  PrintWorkflowXpsReceiver::SetImageProperties
    *
    *  Saves the required properties of the image to be added to each page.
    *
    *  Arguments:
    *        _In_ LPCWSTR      imagePath     File path of image to be added to each page
    *        _In_ double       dpiX          Dots per inch X resolution
    *        _In_ double       dpiY          Dots per inch Y resolution
    *        _In_ int          width         Width in pixels
    *        _In_ int          height        Height in pixels
    *
    *
    *  Returns:
    *       Nothing
    */
    void PrintWorkflowXpsReceiver::SetImageProperties(_In_ LPCWSTR imagePath,
                                                      _In_ double dpiX,
                                                      _In_ double dpiY,
                                                      _In_ int width,
                                                      _In_ int height)
    {
        _imagePath = imagePath;
        _imageDpiX = dpiX;
        _imageDpiY = dpiY;
        _imageWidth = width;
        _imageHeight = height;
    }

    /*  PrintWorkflowXpsReceiver::GetImageProperties
    *
    *  Gets the required properties of the image to be added to each page.
    *
    *  Arguments:
    *        _Out_ double &dpiX  Dots per inch X resolution
    *        _Out_ double &dpiY  Dots per inch Y resolution
    *        _Out_ int &width    Width in pixels
    *        _Out_ int &height   Height in pixels    *
    *  Returns:
    *       Nothing
    */
    bool PrintWorkflowXpsReceiver::GetImageProperties(_Out_ LPCWSTR &imagePath,
                                                      _Out_ double &dpiX,
                                                      _Out_ double &dpiY,
                                                      _Out_ int &width,
                                                      _Out_ int &height)
    {
        imagePath = _imagePath;
        dpiX = _imageDpiX;
        dpiY = _imageDpiY;
        width = _imageWidth;
        height = _imageHeight;

        // Sanity check
        if (imagePath != nullptr  &&  imagePath[0] != L'\0'  && dpiX != 0.0L  && dpiY != 0.0L  &&  width != 0 && height != 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    // End of customizable utility methods
}
