// Window.cpp
// KlayGE Window�� ʵ���ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2007-2008
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// ʵ���Ե�linux֧�� (2008.5.19)
//
// 3.6.0
// ���ν��� (2007.6.26)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>

#include <KlayGE/Window.hpp>

namespace KlayGE
{
#ifdef KLAYGE_PLATFORM_WINDOWS
	LRESULT Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Window* win = reinterpret_cast<Window*>(::GetWindowLongPtrW(hWnd, GWLP_USERDATA));
		if (win != NULL)
		{
			return win->MsgProc(hWnd, uMsg, wParam, lParam);
		}
		else
		{
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	Window::Window(std::string const & name, int32_t left, int32_t top,
			uint32_t width, uint32_t height)
	{
		HINSTANCE hInst = ::GetModuleHandle(NULL);

		std::wstring wname;
		Convert(wname, name);

		// Register the window class
#ifdef KLAYGE_COMPILER_GCC
		WNDCLASSEXA wc;
#else
		WNDCLASSEXW wc;
#endif
		wc.cbSize			= sizeof(wc);
		wc.style			= CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc		= WndProc;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= sizeof(this);
		wc.hInstance		= hInst;
		wc.hIcon			= NULL;
		wc.hCursor			= ::LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground	= static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
		wc.lpszMenuName		= NULL;
#ifdef KLAYGE_COMPILER_GCC
		wc.lpszClassName	= name.c_str();
#else
		wc.lpszClassName	= wname.c_str();
#endif
		wc.hIconSm			= NULL;
#ifdef KLAYGE_COMPILER_GCC
		::RegisterClassExA(&wc);
#else
		::RegisterClassExW(&wc);
#endif

		RECT rc = { 0, 0, width, height };
		::AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

		// Create our main window
		// Pass pointer to self
#ifdef KLAYGE_COMPILER_GCC
		wnd_ = ::CreateWindowA(name.c_str(), name.c_str(),
			WS_OVERLAPPEDWINDOW, left, top,
			rc.right - rc.left, rc.bottom - rc.top, 0, 0, hInst, NULL);
#else
		wnd_ = ::CreateWindowW(wname.c_str(), wname.c_str(),
			WS_OVERLAPPEDWINDOW, left, top,
			rc.right - rc.left, rc.bottom - rc.top, 0, 0, hInst, NULL);
#endif

		::GetClientRect(wnd_, &rc);
		left_ = left;
		top_ = top;
		width_ = rc.right - rc.left;
		height_ = rc.bottom - rc.top;

		::SetWindowLongPtrW(wnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		::ShowWindow(wnd_, SW_SHOWNORMAL);
		::UpdateWindow(wnd_);
	}

	Window::~Window()
	{
		if (wnd_ != NULL)
		{
			::DestroyWindow(wnd_);
			wnd_ = NULL;
		}
	}

	LRESULT Window::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_ACTIVATE:
			if (WA_INACTIVE == LOWORD(wParam))
			{
				this->OnActive()(*this, false);
			}
			else
			{
				this->OnActive()(*this, true);
			}
			break;

		case WM_PAINT:
			this->OnPaint()(*this);
			break;

		case WM_ENTERSIZEMOVE:
			// Previent rendering while moving / sizing
			this->OnEnterSizeMove()(*this);
			break;

		case WM_EXITSIZEMOVE:
			this->OnExitSizeMove()(*this);
			break;

		case WM_SIZE:
			// Check to see if we are losing or gaining our window.  Set the
			// active flag to match
			if ((SIZE_MAXHIDE == wParam) || (SIZE_MINIMIZED == wParam))
			{
				this->OnSize()(*this, false);
			}
			else
			{
				this->OnSize()(*this, true);
			}
			break;

		case WM_GETMINMAXINFO:
			// Prevent the window from going smaller than some minimu size
			reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.x = 100;
			reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.y = 100;
			break;

		case WM_SETCURSOR:
		    this->OnSetCursor()(*this);
			break;

		case WM_CHAR:
			this->OnChar()(*this, static_cast<wchar_t>(wParam));
			break;

		case WM_KEYDOWN:
			this->OnKeyDown()(*this, static_cast<wchar_t>(wParam));
			break;

		case WM_KEYUP:
			this->OnKeyUp()(*this, static_cast<wchar_t>(wParam));
			break;

		case WM_CLOSE:
			this->OnClose()(*this);
			::PostQuitMessage(0);
			return 0;
		}

		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
#elif defined KLAYGE_PLATFORM_LINUX
	Window::Window(std::string const & name, int32_t left, int32_t top,
		uint32_t width, uint32_t height)
	{
		x_display_ = XOpenDisplay(NULL);

		XSetWindowAttributes attr;
		attr.colormap     = DefaultColormapOfScreen(DefaultScreenOfDisplay(x_display_));
		attr.border_pixel = 0;
		attr.event_mask   = ExposureMask
								| VisibilityChangeMask
								| KeyPressMask
								| KeyReleaseMask
								| ButtonPressMask
								| ButtonReleaseMask
								| PointerMotionMask
								| StructureNotifyMask
								| SubstructureNotifyMask
								| FocusChangeMask
								| ResizeRedirectMask;
		x_window_ = XCreateWindow(x_display_, XDefaultRootWindow(x_display_),
					left, top, width, height, 0, XDefaultDepth(x_display_, 0),
					InputOutput, CopyFromParent, CWBorderPixel | CWColormap | CWEventMask, &attr);
		XStoreName(x_display_, x_window_, name.c_str());

		XMapWindow(x_display_, x_window_);
		XFlush(x_display_);

		XWindowAttributes win_attr;
		XGetWindowAttributes(x_display_, x_window_, &win_attr);
		left_ = win_attr.x;
		top_ = win_attr.y;
		width_ = win_attr.width;
		height_ = win_attr.height;

		wm_delete_window_ = XInternAtom(x_display_, "WM_DELETE_WINDOW", false);
		XSetWMProtocols(x_display_, x_window_, &wm_delete_window_, 1);
	}

	Window::~Window()
	{
		XDestroyWindow(x_display_, x_window_);
		XCloseDisplay(x_display_);
	}

	void Window::MsgProc(XEvent const & event)
	{
		switch (event.type)
		{
		case FocusIn:
			this->OnActive()(*this, true);
			break;

		case FocusOut:
			this->OnActive()(*this, false);
			break;

		case Expose:
			this->OnPaint()(*this);
			break;

		case ResizeRequest:
			{
				XResizeRequestEvent const & resize_ev = reinterpret_cast<XResizeRequestEvent const &>(event);
				if ((0 == resize_ev.width) || (0 == resize_ev.height))
				{
					this->OnSize()(*this, false);
				}
				else
				{
					this->OnSize()(*this, true);
				}
			}
			break;

		case KeyPress:
			{
				XKeyEvent const & key_ev = reinterpret_cast<XKeyEvent const &>(event);
				this->OnKeyDown()(*this, static_cast<wchar_t>(key_ev.keycode));
			}
			break;

		case KeyRelease:
			{
				XKeyEvent const & key_ev = reinterpret_cast<XKeyEvent const &>(event);
				this->OnKeyUp()(*this, static_cast<wchar_t>(key_ev.keycode));
			}
			break;

		case ClientMessage:
			if (wm_delete_window_ == event.xclient.data.l[0])
			{
				this->OnClose()(*this);
				XDestroyWindow(x_display_, x_window_);
				XCloseDisplay(x_display_);
				exit(0);
			}
			break;
		}
	}
#endif
}
