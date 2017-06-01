#include "pch.h"
#include "HWDeviceContext.h"

bool HWDeviceContext::initialized = false;

HWDeviceContext::HWDeviceContext(const HWWindow& _window)
{
	version_major = 4;
	version_minor = 3;
	window = &_window;
}

HWDeviceContext::~HWDeviceContext()
{
	if(rendering_context)
	{
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(rendering_context);
	}
	if(device_context)
	{
		ReleaseDC(window->window_handle, device_context);
	}
}

bool HWDeviceContext::CreateContext()
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = bit_depth;
	pfd.cDepthBits = 16;
	pfd.bReserved = PFD_MAIN_PLANE;

	device_context = GetDC(window->window_handle);
	if(!device_context)
	{
		return false;
	}

	pixel_format = ChoosePixelFormat(device_context, &pfd);
	if(!pixel_format)
	{
		return false;
	}

	if(!SetPixelFormat(device_context, pixel_format, &pfd))
	{
		return false;
	}

	rendering_context = wglCreateContext(device_context);
	if(!rendering_context)
	{
		return false;
	}

	if(!wglMakeCurrent(device_context, rendering_context))
	{
		return false;
	}

	if(!initialized)
	{
		if(glewInit() != GLEW_OK)
		{
			return false;
		}
	}

	if(!wglewIsSupported("WGL_ARB_create_context"))
	{
		return false;
	}

	int attribs[] =
	{
        WGL_CONTEXT_MAJOR_VERSION_ARB, version_major,
        WGL_CONTEXT_MINOR_VERSION_ARB, version_minor,
        WGL_CONTEXT_FLAGS_ARB, 0,
        0
    };

	HGLRC temp = rendering_context;
	rendering_context = wglCreateContextAttribsARB(device_context, 0, attribs);
	wglDeleteContext(temp);
	if(!rendering_context)
	{
		return false;
	}

	if(!wglMakeCurrent(device_context, rendering_context))
	{
		return false;
	}

	glClearColor(0.4f, 0.6f, 0.9f, 0.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	return true;
}

void HWDeviceContext::SwapBuffers() const
{
	::SwapBuffers(device_context);
}

void HWDeviceContext::SetVSync(bool vsync)
{
	wglSwapIntervalEXT(vsync ? 1 : 0);
}
