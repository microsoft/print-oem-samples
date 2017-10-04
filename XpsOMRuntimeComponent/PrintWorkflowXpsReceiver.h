#pragma once
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
    class PrintWorkflowXpsReceiver : public IPrintWorkflowXpsReceiver
    {
    public:
        // Start of IPrintWorkflowXpsReceiver interface methods
        PrintWorkflowXpsReceiver(_In_ ComPtr<IXpsDocumentPackageTarget> &spDocumentPackageTarget);
        virtual HRESULT STDMETHODCALLTYPE QueryInterface(_In_ REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject);
        virtual ULONG STDMETHODCALLTYPE AddRef(void);
        virtual ULONG STDMETHODCALLTYPE Release(void);
        virtual HRESULT STDMETHODCALLTYPE SetDocumentSequencePrintTicket(_In_opt_ IStream *documentSequencePrintTicket);
        virtual HRESULT STDMETHODCALLTYPE SetDocumentSequenceUri(_In_ PCWSTR documentSequenceUri);
        virtual HRESULT STDMETHODCALLTYPE AddDocumentData(_In_ UINT32 documentId, _In_opt_ IStream *documentPrintTicket, _In_ PCWSTR documentUri);
        virtual HRESULT STDMETHODCALLTYPE AddPage(_In_ UINT32 documentId, _In_ UINT32 pageId, _In_opt_ IXpsOMPageReference *pageReference, _In_ PCWSTR pageUri);
        virtual HRESULT STDMETHODCALLTYPE Close();
        // End of IPrintWorkflowXpsReceiver interface methods

        // Start of customizable utility methods to wait until completion, and enable data to be passed in
        bool WaitForOMGenerationCompletion();
        HRESULT AddLogoImageToPage(_In_ ComPtr<IXpsOMPage>  &xpsOMPage, _In_ const XPS_SIZE &pageDimensions);
        HRESULT CreateImageResource(_In_ LPCWSTR imageFileName, _In_ LPCWSTR imagePartName, _Out_ IXpsOMImageResource **imageResource);
        HRESULT AddImageToVisualCollection(_In_ ComPtr<IXpsOMPage> &xpsPage, _In_ IXpsOMImageResource *imageResource, _In_ XPS_SIZE *imageWidthAndHeight,
                                           _In_ FLOAT dotsPerInchX, _In_ FLOAT dotsPerInchY, _In_ XPS_RECT rect, _In_ LPCWSTR shortDescription);
        HRESULT CreateRectanglePath(_In_  const XPS_RECT &rect, _Out_ IXpsOMPath **rectPath);
        HRESULT AddWatermarkTextToPage(_In_ ComPtr<IXpsOMPage>  &xpsOMPage, _In_ const XPS_SIZE &pageDimensions);
        HRESULT AddTextToPage(_In_  IXpsOMFontResource *xpsFont, _In_ float fontEmSize, _In_ IXpsOMSolidColorBrush *xpsBrush,
                              _In_ XPS_POINT *origin, _In_ LPCWSTR unicodeString, _Inout_ IXpsOMPage *xpsPage);
        HRESULT CreateFont(_In_ LPCWSTR fontFileName, _Out_ IXpsOMFontResource **fontResource);
        HRESULT CreateBrush(_Out_ IXpsOMSolidColorBrush **xpsTextColorBrush);
        void SetImageProperties(_In_ LPCWSTR imagePath, _In_ double dpiX, _In_ double dpiY, _In_ int width, _In_ int height);
        bool GetImageProperties(_Out_ LPCWSTR &imagePath, _Out_ double &dpiX, _Out_ double &dpiY, _Out_ int &width, _Out_ int &height);
        LPCWSTR GetWatermarkText();
        void SetWatermarkText(_In_ LPCWSTR imagePath);
        void SendToastNotification(String^ xmlToastMessage, String^ image);
        // End of customizable utility methods

    private:
        ULONG _referenceCount;
        ComPtr<IStream> _documentSequencePrintTicketStream;
        HANDLE _xpsOMGenerationEvent;
        HRESULT _closeHRESULT;
        ComPtr<IXpsDocumentPackageTarget> _xpsOMDocumentPackageTarget;
        ComPtr<IXpsOMPackageWriter> _xpsOMPackageWriter;
        ComPtr<IXpsOMObjectFactory> _xpsOMObjectFactory;

        LPCWSTR _imagePath;
        std::wstring _watermarkText;
        double _imageDpiX;
        double _imageDpiY;
        int _imageWidth;
        int _imageHeight;
        bool suppressToast = false;
    };
}
