#include "pch.h"
#include "VirtualPrinterBgTask.h"
#if __has_include("VirtualPrinterBgTask.g.cpp")
#include "VirtualPrinterBgTask.g.cpp"
#endif

#include <winrt\Windows.Foundation.h>
#include <winrt\Windows.Devices.Printers.h>
#include <winrt\Windows.Storage.h>
#include <winrt\Windows.Storage.Streams.h>
#include <winrt\Windows.Data.Xml.Dom.h>
#include <winrt\Windows.Graphics.Printing.PrintTicket.h>
#include <wil/result.h>
#include <LocalStorageHelper.h>


namespace winrt::BackgroundTasks::implementation
{
	using namespace winrt::Windows::ApplicationModel::Background;
	using namespace winrt::Windows::Graphics::Printing::PrintSupport;
	using namespace winrt::Windows::Graphics::Printing::Workflow;
	using namespace winrt::Windows::Graphics::Printing::PrintTicket;
	using namespace winrt::Windows::Storage;
	using namespace winrt::Windows::Storage::Streams;
	using namespace winrt::Windows::Data::Xml::Dom;

	void VirtualPrinterBgTask::Run(IBackgroundTaskInstance const& bgInstance)
	{
		m_taskDeferral = bgInstance.GetDeferral();

		auto triggerDetails = bgInstance.TriggerDetails().as<PrintWorkflowVirtualPrinterTriggerDetails>();
		m_session = triggerDetails.VirtualPrinterSession();

		m_dataAvailToken = m_session.VirtualPrinterDataAvailable({ get_weak(), &VirtualPrinterBgTask::OnPdlDataAvailable });
		m_printDevice = m_session.Printer();
		m_session.Start();
	}

	VirtualPrinterBgTask::~VirtualPrinterBgTask()
	{
		m_session.VirtualPrinterDataAvailable(m_dataAvailToken);
	}

	XpsUtil::XpsPageWatermarker VirtualPrinterBgTask::ConfgiureWatermarker()
	{
		XpsUtil::XpsPageWatermarker waterMarker;
		winrt::hstring watermarkText;
		int32_t fontSize{};
		double xOffset{};
		double yOffset{};
		LocalStorageHelper::GetWatermarkTextPropertiesFromLocalStorage(watermarkText, fontSize, xOffset, yOffset);
		waterMarker.SetWatermarkText(watermarkText, fontSize, xOffset, yOffset);

		winrt::hstring imageFilePath;
		double dpiX;
		double dpiY;
		int32_t imageWidth;
		int32_t imageHeight;
		bool usingImage = LocalStorageHelper::GetImagePropertiesFromLocalStorage(imageFilePath, dpiX, dpiY, imageWidth, imageHeight);

		if (usingImage && !imageFilePath.empty())
		{
			waterMarker.SetWatermarkImage(imageFilePath, dpiX, dpiY, imageWidth, imageHeight);
			waterMarker.SetWatermarkImageEnabled(true);
		}

		return waterMarker;
	}

	void VirtualPrinterBgTask::OnPdlDataAvailable(PrintWorkflowVirtualPrinterSession const& /*sender*/, PrintWorkflowVirtualPrinterDataAvailableEventArgs const& args)
	{
		PrintWorkflowSubmittedStatus jobStatus = PrintWorkflowSubmittedStatus::Failed;

		auto cleaup = wil::scope_exit([&]() {
				args.CompleteJob(jobStatus);
				m_taskDeferral.Complete();
			});

		auto uri = m_printDevice.PrinterUri();
		if (!_wcsicmp(uri.Path().c_str(), L"print-to-clould"))
		{
			// This is just a sample for print clould without target file
			// Put could print code here, samples cannot show cloud communication
			return;
		}

		auto sourceContent = args.SourceContent();
		StorageFile targetFile = args.GetTargetFileAsync().get();
		IRandomAccessStream outputStream = targetFile.OpenAsync(FileAccessMode::ReadWrite).get();
		auto outStream = outputStream.GetOutputStreamAt(0);
		

		auto inputStream = sourceContent.GetInputStream();
		if (!_wcsicmp(uri.Path().c_str(), L"print-to-pdf"))
		{
			if (!_wcsicmp(sourceContent.ContentType().c_str(), L"application/oxps"))
			{
				auto UIResult = args.UILauncher().LaunchAndCompleteUIAsync().get();
				if (UIResult != PrintWorkflowUICompletionStatus::Completed)
				{
					if (UIResult == PrintWorkflowUICompletionStatus::UserCanceled)
					{
						jobStatus = PrintWorkflowSubmittedStatus::Canceled;
						return;
					}
					else
					{
						THROW_HR_MSG(E_UNEXPECTED, "Print UI failed");
					}
				}
				PrintWorkflowObjectModelSourceFileContent xpsContentObjectModel(inputStream);
				XpsUtil::XpsSequentialDocument xpsDocument(xpsContentObjectModel);
				auto watermarker = ConfgiureWatermarker();
				IInputStream watermarkedStream = xpsDocument.GetWatermarkedStream(watermarker);

				PrintWorkflowPdlConverter converter = args.GetPdlConverter(PrintWorkflowPdlConversionType::XpsToPdf);
				auto printTicket = args.GetJobPrintTicket();
				auto xmlString = printTicket.XmlNode().GetXml();
				OutputDebugString(L"Background task Pirn ticket\n");
				OutputDebugString(xmlString.c_str());

				converter.ConvertPdlAsync(printTicket, watermarkedStream, outStream).get();
				jobStatus = PrintWorkflowSubmittedStatus::Succeeded;
			}
			else
			{
				auto printTicket = args.GetJobPrintTicket();
				auto xmlString = printTicket.XmlNode().GetXml();
				OutputDebugString(L"Background task Pirn ticket\n");
				OutputDebugString(xmlString.c_str());
				RandomAccessStream::CopyAndCloseAsync(inputStream, outputStream).get();
				jobStatus = PrintWorkflowSubmittedStatus::Succeeded;
			}
		}
		else if (!_wcsicmp(uri.Path().c_str(), L"print-to-ps"))
		{
			auto printTicket = args.GetJobPrintTicket();
			auto xmlString = printTicket.XmlNode().GetXml();
			OutputDebugString(L"Background task Pirn ticket\n");
			OutputDebugString(xmlString.c_str());
			RandomAccessStream::CopyAndCloseAsync(inputStream, outputStream).get();
			jobStatus = PrintWorkflowSubmittedStatus::Succeeded;
		}
		else
		{
			THROW_HR_MSG(E_UNEXPECTED, "Called with unexpected printer: %ws", uri.AbsoluteUri().c_str());
		}
	}
}