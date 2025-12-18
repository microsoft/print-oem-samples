#include "pch.h"
#include "ModalWindowHandler.h"
#include "wil/cppwinrt.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include "winrt/Windows.UI.WindowManagement.h"
#include <winrt/windows.ui.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.Xaml.Markup.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <windows.ui.interop.h>
#include <wil/cppwinrt_helpers.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Interop.h>
#include <winrt/Microsoft.UI.Content.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Windows.Foundation.h>
#include "PrinterSettings.xaml.h"

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI;
    using namespace Microsoft::UI::Windowing;
    using namespace Microsoft::UI::Dispatching;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Hosting;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Navigation;
    using namespace Windows::Graphics::Printing::PrintSupport;
    using namespace Windows::Graphics::Printing::Workflow;
    using namespace Windows::UI;
    using namespace PSAAppSdk;
    using namespace PSAAppSdk::implementation;
}

namespace winrt::PSAAppSdk::implementation
{
	ModalWindowHandler::ModalWindowHandler(winrt::Microsoft::UI::WindowId const& ownerwindowId, winrt::Microsoft::UI::Xaml::UIElement const& xamlElement)
        : m_xamlElement(xamlElement)
	{
        m_ownerWindow = winrt::Microsoft::UI::GetWindowFromWindowId(ownerwindowId);

        POINT startPoint{};
        if (!IsDumyWindow(m_ownerWindow))
        {
            RECT ownerRect;
            if (!::GetWindowRect(m_ownerWindow, &ownerRect))
            {
                winrt::throw_last_error();
            }

            long ownerWindowWidth = ownerRect.right - ownerRect.left;
            long ownerWindowHeight = ownerRect.bottom - ownerRect.top;
            POINT ownerMidPoint{ static_cast<long>(ownerRect.left + (ownerWindowWidth) / 2.0f), static_cast<long>(ownerRect.top + (ownerWindowHeight) / 2.0f) };
            startPoint = { static_cast<long>(ownerMidPoint.x - (sc_windowWidth) / 2.0f),  static_cast<long>(ownerMidPoint.y - (sc_windowHeight) / 2.0f) };
        }
        else
        {
            startPoint = { CW_USEDEFAULT, CW_USEDEFAULT };
        }
        ResiterClass(sc_windowClassName);
        CreateWindowEx(WS_EX_WINDOWEDGE, sc_windowClassName, 0, WS_OVERLAPPEDWINDOW,
            startPoint.x, startPoint.y, sc_windowWidth, sc_windowHeight, m_ownerWindow, nullptr, GetModuleHandleW(nullptr), this);
        THROW_LAST_ERROR_IF_NULL(m_modalWindow);
        m_selfRef.copy_from(this);
	}

	void ModalWindowHandler::Activate()
	{
        ShowWindow(m_modalWindow, SW_SHOW);
	}

    bool ModalWindowHandler::IsDumyWindow(HWND hwnd)
    {
        auto style = GetWindowLong(hwnd, GWL_STYLE);

        if ((style & WS_VISIBLE) == 0)
        {
            // return true if parent window is not visible
            return true;
        }

        RECT rect;
        if (!::GetWindowRect(hwnd, &rect))
        {
            winrt::throw_last_error();
        }

        return false;
    }

	LRESULT ModalWindowHandler::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
        switch (message)
        {
        case WM_CREATE:
        {
            // Create our DesktopWindowXamlSource and attach it to our hwnd.  This is our "island".
            m_windowXamlSource = winrt::DesktopWindowXamlSource{};
            m_windowXamlSource.Initialize(winrt::GetWindowIdFromWindow(hWnd));

            // Enable the DesktopWindowXamlSource to be a tab stop.
            ::SetWindowLong(winrt::GetWindowFromWindowId(m_windowXamlSource.SiteBridge().WindowId()),
                GWL_STYLE, WS_TABSTOP | WS_CHILD | WS_VISIBLE);

            // Put a new instance of our Xaml "MainPage" into our island.  This is our UI content.
            m_windowXamlSource.Content(m_xamlElement);
        }
        break;
        case WM_SIZE:
        {
            const int width = LOWORD(lParam);
            const int height = HIWORD(lParam);

            if (m_windowXamlSource)
            {
                m_windowXamlSource.SiteBridge().MoveAndResize({ 0, 0, width, height});
            }
        }
        break;
        case WM_ACTIVATE:
        {
        }
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            UNREFERENCED_PARAMETER(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_NCDESTROY:
            ::SetWindowLong(hWnd, GWLP_USERDATA, NULL);
            m_selfRef = nullptr;
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
	}

	LRESULT ModalWindowHandler::s_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
        if (WM_NCCREATE == uMsg)
        {
            ModalWindowHandler* modalWindowHandler = reinterpret_cast<ModalWindowHandler*>((reinterpret_cast<LPCREATESTRUCT>(lParam))->lpCreateParams);
            if (modalWindowHandler)
            {
                modalWindowHandler->m_modalWindow = hwnd;
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(modalWindowHandler));

                // Even if pThis->vWndProc fails the create, USER will always
                // send us a WM_NCDESTROY so we always get a chance to clean up
                return modalWindowHandler->WndProc(hwnd, uMsg, wParam, lParam);
            }
            return FALSE;
        }
        else
        {
            ModalWindowHandler* modalWindowHandler = reinterpret_cast<ModalWindowHandler*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            LRESULT lres;

            if (modalWindowHandler)
            {
                winrt::com_ptr<ModalWindowHandler> strongRef;
                strongRef.copy_from(modalWindowHandler);
                lres = modalWindowHandler->WndProc(hwnd, uMsg, wParam, lParam);

                if (uMsg == WM_NCDESTROY)
                {
                    SetWindowLongPtr(hwnd, 0, NULL);
                    modalWindowHandler->m_modalWindow = nullptr;
                }
            }
            else
            {
                lres = DefWindowProcW(hwnd, uMsg, wParam, lParam);
            }

            return lres;
        }
	}

	void ModalWindowHandler::ResiterClass(PWSTR className)
	{
		WNDCLASSEXW wcex{};

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = (WNDPROC)ModalWindowHandler::s_WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = GetModuleHandleW(nullptr);
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszClassName = className;

		winrt::check_bool(RegisterClassExW(&wcex) != 0);
	}
}
