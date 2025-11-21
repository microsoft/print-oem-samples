#pragma once
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include "PrinterSettings.g.h"
namespace winrt::VirtualPrinterAppSdk::implementation
{
	struct ModalWindowHandler : public winrt::implements<ModalWindowHandler, ::IUnknown>
	{
		ModalWindowHandler(winrt::Microsoft::UI::WindowId const& ownerwindowId, winrt::Microsoft::UI::Xaml::UIElement const& xamlElement);
		void Activate();

	private:
		static LRESULT s_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void ResiterClass(PWSTR className);
		bool IsDumyWindow(HWND hwnd);

		winrt::Microsoft::UI::Xaml::Hosting::DesktopWindowXamlSource m_windowXamlSource{ nullptr };
		winrt::Microsoft::UI::Xaml::UIElement m_xamlElement{ nullptr };
		HWND m_ownerWindow{};
		HWND m_modalWindow{};
		winrt::com_ptr<ModalWindowHandler> m_selfRef{};
		static constexpr wchar_t* sc_windowClassName = L"PrinterSettingsWindow";
		static constexpr uint32_t sc_windowWidth = 600;
		static constexpr uint32_t sc_windowHeight = 600;

	};
}

