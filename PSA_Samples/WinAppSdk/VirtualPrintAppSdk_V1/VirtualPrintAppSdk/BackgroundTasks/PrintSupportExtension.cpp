#include "pch.h"
#include "PrintSupportExtension.h"
#if __has_include("PrintSupportExtension.g.cpp")
#include "PrintSupportExtension.g.cpp"
#endif
#include <winrt\Windows.Foundation.h>
#include <winrt\Windows.Devices.Printers.h>
#include <winrt\Windows.Storage.h>
#include <winrt\Windows.Storage.Streams.h>
#include <winrt\Windows.Data.Xml.Dom.h>
#include <wil\result.h>

namespace winrt::BackgroundTasks::implementation
{
	using namespace winrt::Windows::ApplicationModel::Background;
	using namespace winrt::Windows::Graphics::Printing::PrintSupport;
	using namespace winrt::Windows::Graphics::Printing::Workflow;
	using namespace winrt::Windows::Devices::Printers;
	using namespace winrt::Windows::Data::Xml::Dom;
	using namespace winrt::Windows::Storage;
	using namespace winrt::Windows::Storage::Streams;

	void PrintSupportExtension::Run(IBackgroundTaskInstance const& bgInstance)
	{
		m_taskDeferral = bgInstance.GetDeferral();
		bgInstance.Canceled({ get_weak(), &PrintSupportExtension::OnTaskInstanceCanceled });

		auto triggerDetails = bgInstance.TriggerDetails().as< PrintSupportExtensionTriggerDetails>();
		m_session = triggerDetails.Session();
		m_session.PrintTicketValidationRequested({ get_weak(), &PrintSupportExtension::OnSessionPrintTicketValidationRequested });
		m_session.PrintDeviceCapabilitiesChanged({get_weak(), &PrintSupportExtension ::OnSessionPrintDeviceCapabilitiesChanged});
		m_session.CommunicationErrorDetected({get_weak(), &PrintSupportExtension::OnSessionCommincationErrorDetected});

		m_session.Start();
	}

	void PrintSupportExtension::OnTaskInstanceCanceled(IBackgroundTaskInstance const& /*sender*/, BackgroundTaskCancellationReason const& /*reason*/)
	{
		// Complete task deferral when BG process is cancelled
		m_taskDeferral.Complete();
	}

	void  PrintSupportExtension::OnSessionPrintTicketValidationRequested(PrintSupportExtensionSession const& /*sender*/, PrintSupportPrintTicketValidationRequestedEventArgs const& args)
	{
		// Do custom print ticket validation
		auto cleanup = wil::scope_exit([&] { args.GetDeferral().Complete(); });
		args.SetPrintTicketValidationStatus(WorkflowPrintTicketValidationStatus::Resolved);
	}

	void PrintSupportExtension::OnSessionPrintDeviceCapabilitiesChanged(PrintSupportExtensionSession const& /*sender*/, PrintSupportPrintDeviceCapabilitiesChangedEventArgs const& args)
	{		
		auto cleanup = wil::scope_exit([&] { args.GetDeferral().Complete(); });
		auto pdc = args.GetCurrentPrintDeviceCapabilities();
		pdc.DocumentElement().SetAttribute(L"xmlns:contoso", L"http://schemas.contoso.com/keywords");
		AddCustomMediaType(pdc, L"http://schemas.contoso.com/keywords", L"contoso:ContosoMediaType");
		args.UpdatePrintDeviceCapabilities(pdc);
	}

	void PrintSupportExtension::AddCustomMediaType(XmlDocument const& pdc, winrt::hstring const& namespaceUri, winrt::hstring const& mediaType)
	{
		// See linked below for the XML.Dom.Xmldocument class documentation.
		// https://docs.microsoft.com/en-us/uwp/api/windows.data.xml.dom.xmldocument
		//
		// NOTE: This example does not show error handling for simplicty. You should ensure that the properties being modified exists in the PDC, before trying to set its attributes.
		// Select the 'media-type-supported' XML section.
		auto queryString = L"xmlns:psk=\"http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords\" xmlns:psf2=\"http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2\"";
		auto defaultPageMediaTypeNode = pdc.SelectSingleNodeNS(L"//psk:PageMediaType//*[@psf2:default='true']", winrt::box_value(queryString));

		if (defaultPageMediaTypeNode)
		{
			// Get the owner document so that we can add new elements to the currently selected XML section.
			auto document = defaultPageMediaTypeNode.OwnerDocument();
			// Create the new XML element for our custom media type.
			auto newNode = document.CreateElementNS(winrt::box_value(namespaceUri), mediaType);
			winrt::hstring namesapcePs2 = L"http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2";
			newNode.SetAttributeNS(winrt::box_value(namesapcePs2), L"psf2:psftype", L"Option");
			newNode.SetAttributeNS(winrt::box_value(namesapcePs2), L"psf2:default", L"false");

			auto parent = defaultPageMediaTypeNode.ParentNode();
			parent.AppendChild(newNode);
		}
	}

	void PrintSupportExtension::OnSessionCommincationErrorDetected(PrintSupportExtensionSession const& /*sender*/, PrintSupportCommunicationErrorDetectedEventArgs const& args)
	{
		// Called only for IPP printer, will not be invoked for virtual pritners
		auto cleanup = wil::scope_exit([&] { args.GetDeferral().Complete(); });
	}
}
