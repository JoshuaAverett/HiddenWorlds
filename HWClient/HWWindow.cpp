#include "pch.h"
#include "HWWindow.h"

#ifdef HW_OS_WINDOWS
bool HWWindow::initialized = false;
HINSTANCE HWWindow::application_instance = nullptr;
#endif

HWWindow::HWWindow(S32 width, S32 height, WNDPROC handler)
{
	window_width = width;
	window_height = height;
	full_screen = false;
	window_title = std::string();
	event_handler = handler;
}

HWWindow::HWWindow(S32 width, S32 height, bool fullscreen, WNDPROC handler)
{
	window_width = width;
	window_height = height;
	full_screen = fullscreen;
	window_title = std::string();
	event_handler = handler;
}

HWWindow::HWWindow(const std::string& title, S32 width, S32 height, bool fullscreen, WNDPROC handler)
{
	window_width = width;
	window_height = height;
	full_screen = false;
	window_title = title;
	event_handler = handler;
}

HWWindow::~HWWindow()
{
	if(window_handle)
	{
		DestroyWindow(window_handle);
	}
}

bool HWWindow::CreateWindow()
{
#ifdef HW_OS_WINDOWS
	if(!initialized)
	{
		if(!Initialize()) return false;
	}

	RECT window_rect;
	window_rect.left   = 0;
	window_rect.right  = window_width;
	window_rect.top    = 0;
	window_rect.bottom = window_height;

	DWORD dwExStyle = 0;
	DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if(full_screen)
	{

	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	if(!AdjustWindowRectEx(&window_rect, dwStyle, FALSE, dwExStyle))
	{
		return false;
	}

	std::wstring str2(window_title.length(), L' ');
	std::copy(window_title.begin(), window_title.end(), str2.begin());

	window_handle = CreateWindowEx(
		dwExStyle,
		windows_class.lpszClassName,
		str2.c_str(),
		dwStyle,
		0, 0,
		window_rect.right  - window_rect.left,
		window_rect.bottom - window_rect.top,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	if(!window_handle)
	{
		return false;
	}

	return true;
#endif
}

#ifdef HW_OS_WINDOWS
bool HWWindow::Initialize()
{
	application_instance		= GetModuleHandle(NULL);
	windows_class.style		    = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windows_class.lpfnWndProc   = event_handler;
	windows_class.cbClsExtra    = 0;
	windows_class.cbWndExtra    = 0;
	windows_class.hInstance	    = application_instance;
	windows_class.hIcon	        = LoadIcon(NULL, IDI_WINLOGO);
	windows_class.hCursor       = LoadCursor(NULL, IDC_ARROW);
	windows_class.hbrBackground = NULL;
	windows_class.lpszMenuName	= NULL;
	windows_class.lpszClassName = L"HiddenWorlds";

	windows_class_atom = RegisterClass(&windows_class);
	initialized = windows_class_atom != 0;
	return initialized;
}
#endif

void HWWindow::ShowWindow(bool show)
{
#ifdef HW_OS_WINDOWS
	::ShowWindow(window_handle, show ? SW_SHOW : SW_HIDE);
#endif
}

S32 HWWindow::GetWidth()
{
	return window_width;
}

S32 HWWindow::GetHeight()
{
	return window_height;
}

F32 HWWindow::GetAspectRatio()
{
	return (F32)GetWidth() / (F32)GetHeight();
}