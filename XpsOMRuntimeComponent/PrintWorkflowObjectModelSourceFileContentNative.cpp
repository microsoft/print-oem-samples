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
#include "PrintWorkflowObjectModelSourceFileContentNative.h"

namespace XpsOMRuntimeComponent
{
    // Start of PrintWorkflowObjectModelSourceFileContentNative methods

    /*  PrintWorkflowObjectModelSourceFileContentNative::PrintWorkflowObjectModelSourceFileContentNative
    *
    *  Constructor for PrintWorkflowObjectModelSourceFileContentNative.
    *
    *  Arguments:
    *        _In_ PrintWorkflowObjectModelSourceFileContent^   content     Source file content
    *        _In_ PrintWorkflowObjectModelTargetPackage^       package     Object model package
    *
    *  Returns:
    *       Nothing
    */
    PrintWorkflowObjectModelSourceFileContentNative::PrintWorkflowObjectModelSourceFileContentNative(_In_ PrintWorkflowObjectModelSourceFileContent^ content,
                                                                                                     _In_ PrintWorkflowObjectModelTargetPackage^ package)
     :  _imagePath(nullptr), _watermarkText(nullptr), _imageDpiX(0.0L), _imageDpiY(0.0L), _imageWidth(0), _imageHeight(0)
    {
        _workflowSourceFileContent = content;
        _workflowOMTargetPackage = package;
    }

    /*  PrintWorkflowObjectModelSourceFileContentNative::StartXpsOMGeneration
    *
    *  Start the XPS object model generation.
    *
    *  Arguments:
    *       None
    *
    *  Returns:
    *       Nothing
    */
    void PrintWorkflowObjectModelSourceFileContentNative::StartXpsOMGeneration()
    {
        // This is called from the Workflow app so we can throw if something fails
        ComPtr<IPrintWorkflowXpsObjectModelTargetPackageNative> workflowXpsOMTargetPackageNative;
        THROW_IF_FAILED(reinterpret_cast<IInspectable*>(_workflowOMTargetPackage)->QueryInterface<IPrintWorkflowXpsObjectModelTargetPackageNative>(&workflowXpsOMTargetPackageNative));
        ComPtr<IPrintWorkflowObjectModelSourceFileContentNative> workflowOMSourceFileContentNative;
        THROW_IF_FAILED(reinterpret_cast<IInspectable*>(_workflowSourceFileContent)->QueryInterface<IPrintWorkflowObjectModelSourceFileContentNative>(&workflowOMSourceFileContentNative));

        THROW_IF_FAILED(workflowXpsOMTargetPackageNative->get_DocumentPackageTarget(&_xpsOMDocumentPackageTarget));
        ComPtr<PrintWorkflowXpsReceiver> xpsReceiver = new (std::nothrow)PrintWorkflowXpsReceiver(_xpsOMDocumentPackageTarget);
        if (xpsReceiver.Get() == nullptr)
        {
            THROW_IF_FAILED(E_FAIL);
        }

        _workflowXpsReceiver = xpsReceiver;

        // Initialize any data that needs to be passed to the XpsReceiver
        _workflowXpsReceiver->SetWatermarkText(_watermarkText->Data());
        _workflowXpsReceiver->SetImageProperties(_imagePath->Data(), _imageDpiX, _imageDpiY, _imageWidth, _imageHeight);

        THROW_IF_FAILED(workflowOMSourceFileContentNative->StartXpsOMGeneration(_workflowXpsReceiver.Get()));
    }

    /*  PrintWorkflowObjectModelSourceFileContentNative::WaitForOMGeneration
    *
    *  Wait until the Object Model document generation is complete.
    *
    *  Arguments:
    *      None
    *
    *  Returns:
    *       bool              True if Successfully generated the output and Close()-ed the file
    */
    bool PrintWorkflowObjectModelSourceFileContentNative::WaitForOMGeneration()
    {
        return _workflowXpsReceiver->WaitForOMGenerationCompletion();
    }

    /*  PrintWorkflowObjectModelSourceFileContentNative::SetWatermarkText
    *
    *  Saves the watermark text to be added to each page.
    *
    *  Arguments:
    *      Platform::String^  watermarkText        Watermark text to be added to each page
    *
    *  Returns:
    *       Nothing
    */
    void PrintWorkflowObjectModelSourceFileContentNative::SetWatermarkText(_In_ Platform::String^ watermarkText)
    {
        // Just pass this on to the PrintWorkflowXpsReceiver
        _watermarkText = watermarkText;
    }

    /*  PrintWorkflowObjectModelSourceFileContentNative::SetImageProperties
    *
    *  Saves the required properties of the image to be added to each page.
    *
    *  Arguments:
    *        _In_ Platform::String^  imagePath     File path of image to be added to each page
    *        _In_ double             dpiX          Dots per inch X resolution
    *        _In_ double             dpiY          Dots per inch Y resolution
    *        _In_ int                width         Width in pixels
    *        _In_ int                height        Height in pixels
    *
    *  Returns:
    *       Nothing
    */
    void PrintWorkflowObjectModelSourceFileContentNative::SetImageProperties(_In_ Platform::String^ imagePath,
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
    // End of PrintWorkflowObjectModelSourceFileContentNative methods
}
