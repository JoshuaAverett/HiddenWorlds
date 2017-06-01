#pragma once

#include "pch.h"
#include <string>

#ifdef HW_OS_WINDOWS
// Wrangle the WinAPI
#include <Windows.h>
#undef CreateWindow
#endif

class HWWindow
{
public:
	friend class HWDeviceContext;
	HWWindow(S32 width, S32 height, WNDPROC handler);
	HWWindow(S32 width, S32 height, bool fullscreen, WNDPROC handler);
	HWWindow(const std::string& title, S32 width, S32 height, bool fullscreen, WNDPROC handler);
	HWWindow(const HWWindow&);
	~HWWindow();

	S32 GetWidth();
	S32 GetHeight();
	F32 GetAspectRatio();

	bool CreateWindow();

	void ShowWindow(bool show);
	void SwapBuffers();

private:
	S32 window_width;
	S32 window_height;
	bool full_screen;
	std::string window_title;

#ifdef HW_OS_WINDOWS
	WNDCLASS windows_class;
	ATOM windows_class_atom;
	WNDPROC event_handler;
	HWND window_handle;

	static bool initialized;
	static HINSTANCE application_instance;
	bool Initialize();
#endif
};


