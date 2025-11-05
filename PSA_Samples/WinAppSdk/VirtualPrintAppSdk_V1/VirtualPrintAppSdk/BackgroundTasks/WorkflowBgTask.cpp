#include "pch.h"
#include "WorkflowBgTask.h"
#if __has_include("WorkflowBgTask.g.cpp")
#include "WorkflowBgTask.g.cpp"
#endif
#include <winrt\Windows.Foundation.h>
#include <winrt\Windows.Devices.Printers.h>
#include <winrt\Windows.Storage.h>
#include <winrt\Windows.Storage.Streams.h>
#include <wil\result.h>
#include <wincodec.h>


namespace winrt::BackgroundTasks::implementation
{
	using namespace winrt::Windows::ApplicationModel::Background;
	using namespace winrt::Windows::Graphics::Printing::PrintSupport;
	using namespace winrt::Windows::Graphics::Printing::Workflow;
	using namespace winrt::Windows::Devices::Printers;
	using namespace winrt::Windows::Storage;
	using namespace winrt::Windows::Storage::Streams;

	void WorkflowBgTask::Run(IBackgroundTaskInstance const& bgInstance)
	{
		m_taskDeferral = bgInstance.GetDeferral();

		auto triggerDetails = bgInstance.TriggerDetails().as<PrintWorkflowJobTriggerDetails>();
		m_session = triggerDetails.PrintWorkflowJobSession();

		m_jobStartToken = m_session.JobStarting({ get_weak(), &WorkflowBgTask::OnJobStarting });
		m_pdlModificationToken = m_session.PdlModificationRequested({ get_weak(), &WorkflowBgTask::OnPdlModificationRequested });

		m_session.Start();
	}

	void WorkflowBgTask::OnJobStarting(winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobBackgroundSession const& /*sender*/, winrt::Windows::Graphics::Printing::Workflow::PrintWorkflowJobStartingEventArgs const& args)
	{
		auto cleanup = wil::scope_exit([&] { args.GetDeferral().Complete(); });
		args.SetSkipSystemRendering();
	}

	bool WorkflowBgTask::HasPdlConverter(winrt::hstring const& documentFormat)
	{
		if (!_wcsicmp(documentFormat.c_str(), L"image/pwg-raster") ||
			!_wcsicmp(documentFormat.c_str(), L"application/PCLm") ||
			!_wcsicmp(documentFormat.c_str(), L"application/pdf") ||
			!_wcsicmp(documentFormat.c_str(), L"image/tiff"))
		{
			return true;
		}

		return false;
	}

	winrt::hstring WorkflowBgTask::GetDocumentFormat(IppPrintDevice const& printer)
	{
		auto requestedAttributes = winrt::single_threaded_vector<winrt::hstring>({ L"document-format-default", L"document-format-supported" });
		auto attributes = printer.GetPrinterAttributes(requestedAttributes);


		// Lookup the IPP attribute from the map.
		auto defaultFormat = attributes.Lookup(L"document-format-default").GetKeywordArray().First().Current();

		// If the default format does not have a PDL converter, pick the first supported format.
		if (!HasPdlConverter(defaultFormat))
		{
			for (winrt::hstring const documentFormat : attributes.Lookup(L"document-format-supported").GetKeywordArray())
			{
				if (HasPdlConverter(documentFormat))
				{
					return documentFormat;
				}
			}
		}

		return defaultFormat;
	}

	PrintWorkflowPdlConverter WorkflowBgTask::GetPdlConverter(PrintWorkflowPdlModificationRequestedEventArgs const& args, winrt::hstring const& documentFormat)
	{
		if (!_wcsicmp(documentFormat.c_str(), L"application/pdf"))
		{
			return args.GetPdlConverter(PrintWorkflowPdlConversionType::XpsToPdf);
		}
		else if (!_wcsicmp(documentFormat.c_str(), L"application/PCLm"))
		{
			return args.GetPdlConverter(PrintWorkflowPdlConversionType::XpsToPclm);
		}
		else if (!_wcsicmp(documentFormat.c_str(), L"image/pwg-raster"))
		{
			return args.GetPdlConverter(PrintWorkflowPdlConversionType::XpsToPwgr);
		}
		else if (!_wcsicmp(documentFormat.c_str(), L"image/tiff"))
		{
			return args.GetPdlConverter(PrintWorkflowPdlConversionType::XpsToTiff);
		}
		else
		{
			THROW_HR(E_UNEXPECTED);
		}
	}

	XpsUtil::XpsPageWatermarker WorkflowBgTask::ConfgiureWatermarker()
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

	void WorkflowBgTask::OnPdlModificationRequested(PrintWorkflowJobBackgroundSession const& /*sender*/, PrintWorkflowPdlModificationRequestedEventArgs const& args)
	{
		auto cleanup = wil::scope_exit([&] 
			{
				args.GetDeferral().Complete();
				m_taskDeferral.Complete();
			});
		try
		{
			auto UIResult = args.UILauncher().LaunchAndCompleteUIAsync().get();
			if (UIResult != PrintWorkflowUICompletionStatus::Completed)
			{
				if (UIResult == PrintWorkflowUICompletionStatus::UserCanceled)
				{
					// User can canceled priting,Abort print job and return.
					args.Configuration().AbortPrintFlow(PrintWorkflowJobAbortReason::UserCanceled);
					return;
				}
				else
				{
					THROW_HR_MSG(E_UNEXPECTED, "Print UI failed");
				}
			}
			auto documentFormat = GetDocumentFormat(args.PrinterJob().Printer());
			auto targetStream = args.CreateJobOnPrinter(documentFormat);
			
			auto sourceContent = args.SourceContent();
			auto inputStream = sourceContent.GetInputStream();
			if (!_wcsicmp(sourceContent.ContentType().c_str(), L"application/oxps"))
			{
				PrintWorkflowObjectModelSourceFileContent xpsContentObjectModel(inputStream);
				XpsUtil::XpsSequentialDocument xpsDocument(xpsContentObjectModel);
				auto watermarker = ConfgiureWatermarker();
				IInputStream watermarkedStream = xpsDocument.GetWatermarkedStream(watermarker);

				auto pldConverter = args.GetPdlConverter(PrintWorkflowPdlConversionType::XpsToPdf);
				pldConverter.ConvertPdlAsync(args.PrinterJob().GetJobPrintTicket(), watermarkedStream, targetStream.GetOutputStream()).get();
				targetStream.CompleteStreamSubmission(PrintWorkflowSubmittedStatus::Succeeded);
			}
			else
			{
				// If the source content is not OXPS assume that it is of target PDL and write to the target stream
				RandomAccessStream::CopyAndCloseAsync(inputStream, targetStream.GetOutputStream()).get();
				targetStream.CompleteStreamSubmission(PrintWorkflowSubmittedStatus::Succeeded);
			}
		}
		catch (...)
		{
			args.Configuration().AbortPrintFlow(PrintWorkflowJobAbortReason::JobFailed);
		}
	}
}
