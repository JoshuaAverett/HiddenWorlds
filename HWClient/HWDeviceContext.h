#pragma once

#include "HWWindow.h"
#include "HWGL.h"

class HWDeviceContext
{
public:
	HWDeviceContext(const HWWindow& _window);
	~HWDeviceContext();

	bool CreateContext();
	void SwapBuffers() const;
	void SetVSync(bool vsync);

private:
	GLuint pixel_format;
	S32 version_major;
	S32 version_minor;
	GLuint vao;

#ifdef HW_OS_WINDOWS
	BYTE bit_depth;
	HDC device_context;
	const HWWindow* window;
	HGLRC rendering_context;

	static bool initialized;
#endif
};
