#pragma once
#include "PrintWorkflowXpsReceiver.h"
/*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
*   PARTICULAR PURPOSE.
*
*   This is a header for the sample implementation of the XPS Object Model Windows Runtime component. It shows how to create a
*   IPrintWorkflowXpsReceiver implementation, which is called on a different thread to the App, and receives the document object model parts.
*   It allows the content to be manipulated before being sent on to the printer.
*
*   Because the PrintWorkflowXpsReceiver methods are not called on the App thread, they must return HRESULTS and not throw.
*   The PrintWorkflowObjectModelSourceFileContentNative methods can throw because exceptioms from those can be caught and handled.
*/
namespace XpsOMRuntimeComponent
{
    public ref class PrintWorkflowObjectModelSourceFileContentNative sealed
    {
    public:
        // Start of PrintWorkflowObjectModelSourceFileContentNative methods
        PrintWorkflowObjectModelSourceFileContentNative(_In_ PrintWorkflowObjectModelSourceFileContent^ content, _In_ PrintWorkflowObjectModelTargetPackage^ package);
        void StartXpsOMGeneration();
        bool WaitForOMGeneration();
        void SetImageProperties(_In_ Platform::String^ imagePath, _In_ double dpiX, _In_ double dpiY, _In_ int width, _In_ int height);
        void SetWatermarkText(_In_ Platform::String^ imagePath);
        // End of PrintWorkflowObjectModelSourceFileContentNative methods

    private:
        PrintWorkflowObjectModelSourceFileContent^ _workflowSourceFileContent;
        PrintWorkflowObjectModelTargetPackage^ _workflowOMTargetPackage;
        ComPtr<PrintWorkflowXpsReceiver> _workflowXpsReceiver;
        ComPtr<IXpsDocumentPackageTarget> _xpsOMDocumentPackageTarget;
        Platform::String^ _imagePath;
        Platform::String^ _watermarkText;
        double _imageDpiX;
        double _imageDpiY;
        int _imageWidth;
        int _imageHeight;
    };
}
