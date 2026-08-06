using System;
using System.Runtime.InteropServices;
using Microsoft.UI;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Hosting;
using Windows.Graphics;

namespace PrintSupportApplication
{
    /// <summary>
    /// Hosts a XAML <see cref="UIElement"/> inside a native Win32 window that is owned by
    /// the print dialog's owner window, giving the "More settings" UI modal behavior.
    ///
    /// This is the C# counterpart of the C++/WinRT sample's ModalWindowHandler. A plain
    /// WinUI <c>Window</c> is not owned by, or modal to, the caller-supplied owner window;
    /// creating an owned overlapped Win32 window and hosting XAML in it via
    /// <see cref="DesktopWindowXamlSource"/> reproduces the modal experience.
    /// </summary>
    internal sealed class ModalWindowHandler
    {
        private const string WindowClassName = "PrinterSettingsWindow";
        private const int WindowWidth = 600;
        private const int WindowHeight = 600;

        // Keep the managed WndProc delegate alive for the lifetime of the class so the
        // function pointer handed to RegisterClassExW never dangles.
        private static readonly WndProcDelegate s_wndProc = StaticWndProc;
        private static bool s_classRegistered;

        private readonly UIElement m_xamlElement;
        private readonly IntPtr m_ownerWindow;
        private IntPtr m_modalWindow;
        private DesktopWindowXamlSource? m_windowXamlSource;

        // Strong handle to this instance, stored in the window's GWLP_USERDATA so the
        // static WndProc can route messages back to us. Freed on WM_NCDESTROY.
        private GCHandle m_selfHandle;

        public ModalWindowHandler(WindowId ownerWindowId, UIElement xamlElement)
        {
            m_xamlElement = xamlElement;
            m_ownerWindow = Win32Interop.GetWindowFromWindowId(ownerWindowId);

            int startX;
            int startY;
            if (!IsDummyWindow(m_ownerWindow))
            {
                if (!GetWindowRect(m_ownerWindow, out RECT ownerRect))
                {
                    throw new InvalidOperationException("GetWindowRect failed for owner window.");
                }

                int ownerWidth = ownerRect.right - ownerRect.left;
                int ownerHeight = ownerRect.bottom - ownerRect.top;
                int ownerMidX = ownerRect.left + ownerWidth / 2;
                int ownerMidY = ownerRect.top + ownerHeight / 2;
                startX = ownerMidX - WindowWidth / 2;
                startY = ownerMidY - WindowHeight / 2;
            }
            else
            {
                startX = CW_USEDEFAULT;
                startY = CW_USEDEFAULT;
            }

            RegisterWindowClass();

            m_selfHandle = GCHandle.Alloc(this);

            m_modalWindow = CreateWindowExW(
                WS_EX_WINDOWEDGE,
                WindowClassName,
                null,
                WS_OVERLAPPEDWINDOW,
                startX,
                startY,
                WindowWidth,
                WindowHeight,
                m_ownerWindow,
                IntPtr.Zero,
                GetModuleHandleW(null),
                GCHandle.ToIntPtr(m_selfHandle));

            if (m_modalWindow == IntPtr.Zero)
            {
                if (m_selfHandle.IsAllocated)
                {
                    m_selfHandle.Free();
                }

                throw new InvalidOperationException("CreateWindowExW failed for the modal settings window.");
            }
        }

        public void Activate()
        {
            ShowWindow(m_modalWindow, SW_SHOW);
        }

        private static bool IsDummyWindow(IntPtr hwnd)
        {
            long style = GetWindowLongPtr(hwnd, GWL_STYLE).ToInt64();

            // Treat an invisible owner as a placeholder: center-on-owner math would be meaningless.
            return (style & WS_VISIBLE) == 0;
        }

        private static void RegisterWindowClass()
        {
            if (s_classRegistered)
            {
                return;
            }

            var wcex = new WNDCLASSEXW
            {
                cbSize = (uint)Marshal.SizeOf<WNDCLASSEXW>(),
                style = CS_HREDRAW | CS_VREDRAW,
                lpfnWndProc = Marshal.GetFunctionPointerForDelegate(s_wndProc),
                cbClsExtra = 0,
                cbWndExtra = 0,
                hInstance = GetModuleHandleW(null),
                hCursor = LoadCursorW(IntPtr.Zero, (IntPtr)IDC_ARROW),
                hbrBackground = (IntPtr)(COLOR_WINDOW + 1),
                lpszClassName = WindowClassName,
            };

            if (RegisterClassExW(ref wcex) == 0)
            {
                throw new InvalidOperationException("RegisterClassExW failed for the modal settings window class.");
            }

            s_classRegistered = true;
        }

        private static IntPtr StaticWndProc(IntPtr hWnd, uint message, IntPtr wParam, IntPtr lParam)
        {
            if (message == WM_NCCREATE)
            {
                // CREATESTRUCT.lpCreateParams is the first field; it holds our GCHandle pointer.
                IntPtr createParams = Marshal.ReadIntPtr(lParam);
                SetWindowLongPtr(hWnd, GWLP_USERDATA, createParams);

                var handler = (ModalWindowHandler?)GCHandle.FromIntPtr(createParams).Target;
                if (handler != null)
                {
                    handler.m_modalWindow = hWnd;
                    return handler.WndProc(hWnd, message, wParam, lParam);
                }

                return (IntPtr)0;
            }

            IntPtr userData = GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (userData != IntPtr.Zero)
            {
                var handler = (ModalWindowHandler?)GCHandle.FromIntPtr(userData).Target;
                if (handler != null)
                {
                    IntPtr result = handler.WndProc(hWnd, message, wParam, lParam);

                    if (message == WM_NCDESTROY)
                    {
                        SetWindowLongPtr(hWnd, GWLP_USERDATA, IntPtr.Zero);
                        handler.m_modalWindow = IntPtr.Zero;
                        if (handler.m_selfHandle.IsAllocated)
                        {
                            handler.m_selfHandle.Free();
                        }
                    }

                    return result;
                }
            }

            return DefWindowProcW(hWnd, message, wParam, lParam);
        }

        private IntPtr WndProc(IntPtr hWnd, uint message, IntPtr wParam, IntPtr lParam)
        {
            switch (message)
            {
                case WM_CREATE:
                {
                    // Create the DesktopWindowXamlSource "island" and attach it to our hwnd.
                    m_windowXamlSource = new DesktopWindowXamlSource();
                    m_windowXamlSource.Initialize(Win32Interop.GetWindowIdFromWindow(hWnd));

                    // Make the island a visible, tab-stoppable child of the modal window.
                    IntPtr bridgeHwnd = Win32Interop.GetWindowFromWindowId(m_windowXamlSource.SiteBridge.WindowId);
                    SetWindowLongPtr(bridgeHwnd, GWL_STYLE, (IntPtr)(WS_TABSTOP | WS_CHILD | WS_VISIBLE));

                    // Host the supplied XAML content inside the island.
                    m_windowXamlSource.Content = m_xamlElement;
                    return (IntPtr)0;
                }

                case WM_SIZE:
                {
                    int width = (int)((uint)lParam.ToInt64() & 0xFFFF);
                    int height = (int)(((uint)lParam.ToInt64() >> 16) & 0xFFFF);

                    m_windowXamlSource?.SiteBridge.MoveAndResize(new RectInt32(0, 0, width, height));
                    return (IntPtr)0;
                }

                case WM_DESTROY:
                    PostQuitMessage(0);
                    return (IntPtr)0;

                default:
                    return DefWindowProcW(hWnd, message, wParam, lParam);
            }
        }

        #region Win32 interop

        [UnmanagedFunctionPointer(CallingConvention.Winapi)]
        private delegate IntPtr WndProcDelegate(IntPtr hWnd, uint message, IntPtr wParam, IntPtr lParam);

        private const int GWL_STYLE = -16;
        private const int GWLP_USERDATA = -21;

        private const uint WS_OVERLAPPEDWINDOW = 0x00CF0000;
        private const uint WS_CHILD = 0x40000000;
        private const uint WS_VISIBLE = 0x10000000;
        private const uint WS_TABSTOP = 0x00010000;
        private const uint WS_EX_WINDOWEDGE = 0x00000100;

        private const int CW_USEDEFAULT = unchecked((int)0x80000000);
        private const int SW_SHOW = 5;

        private const uint CS_VREDRAW = 0x0001;
        private const uint CS_HREDRAW = 0x0002;

        private const int COLOR_WINDOW = 5;
        private const int IDC_ARROW = 32512;

        private const uint WM_CREATE = 0x0001;
        private const uint WM_DESTROY = 0x0002;
        private const uint WM_SIZE = 0x0005;
        private const uint WM_NCCREATE = 0x0081;
        private const uint WM_NCDESTROY = 0x0082;

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        private struct WNDCLASSEXW
        {
            public uint cbSize;
            public uint style;
            public IntPtr lpfnWndProc;
            public int cbClsExtra;
            public int cbWndExtra;
            public IntPtr hInstance;
            public IntPtr hIcon;
            public IntPtr hCursor;
            public IntPtr hbrBackground;
            [MarshalAs(UnmanagedType.LPWStr)] public string? lpszMenuName;
            [MarshalAs(UnmanagedType.LPWStr)] public string lpszClassName;
            public IntPtr hIconSm;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct RECT
        {
            public int left;
            public int top;
            public int right;
            public int bottom;
        }

        [DllImport("user32.dll", SetLastError = true)]
        private static extern ushort RegisterClassExW(ref WNDCLASSEXW lpwcx);

        [DllImport("user32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern IntPtr CreateWindowExW(
            uint dwExStyle,
            string lpClassName,
            string? lpWindowName,
            uint dwStyle,
            int x,
            int y,
            int nWidth,
            int nHeight,
            IntPtr hWndParent,
            IntPtr hMenu,
            IntPtr hInstance,
            IntPtr lpParam);

        [DllImport("user32.dll")]
        private static extern IntPtr DefWindowProcW(IntPtr hWnd, uint msg, IntPtr wParam, IntPtr lParam);

        [DllImport("user32.dll")]
        private static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool GetWindowRect(IntPtr hWnd, out RECT lpRect);

        [DllImport("user32.dll")]
        private static extern void PostQuitMessage(int nExitCode);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern IntPtr LoadCursorW(IntPtr hInstance, IntPtr lpCursorName);

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern IntPtr GetModuleHandleW(string? lpModuleName);

        // GetWindowLongPtr / SetWindowLongPtr only exist as *Ptr on 64-bit; fall back to the
        // 32-bit LONG entry points when running as x86.
        private static IntPtr GetWindowLongPtr(IntPtr hWnd, int nIndex)
            => IntPtr.Size == 8 ? GetWindowLongPtrW(hWnd, nIndex) : (IntPtr)GetWindowLongW(hWnd, nIndex);

        private static IntPtr SetWindowLongPtr(IntPtr hWnd, int nIndex, IntPtr dwNewLong)
            => IntPtr.Size == 8
                ? SetWindowLongPtrW(hWnd, nIndex, dwNewLong)
                : (IntPtr)SetWindowLongW(hWnd, nIndex, dwNewLong.ToInt32());

        [DllImport("user32.dll", EntryPoint = "GetWindowLongW")]
        private static extern int GetWindowLongW(IntPtr hWnd, int nIndex);

        [DllImport("user32.dll", EntryPoint = "GetWindowLongPtrW")]
        private static extern IntPtr GetWindowLongPtrW(IntPtr hWnd, int nIndex);

        [DllImport("user32.dll", EntryPoint = "SetWindowLongW")]
        private static extern int SetWindowLongW(IntPtr hWnd, int nIndex, int dwNewLong);

        [DllImport("user32.dll", EntryPoint = "SetWindowLongPtrW")]
        private static extern IntPtr SetWindowLongPtrW(IntPtr hWnd, int nIndex, IntPtr dwNewLong);

        #endregion
    }
}
