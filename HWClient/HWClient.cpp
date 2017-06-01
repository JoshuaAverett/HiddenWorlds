#include "pch.h"
#include "HWWindow.h"
#include "HWDeviceContext.h"
#include "HWTexture.h"
#include "HWHeightMapRenderer.h"
#include "../HWLib/HWHeightMap.h"
#include "HWCamera.h"
#include "HWFont.h"
#include "HWText.h"

#include <Windows.h>
#include <sstream>
#include <stdio.h>

HWCamera camera;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostMessage(hWnd, WM_QUIT, 0 ,0);
		return 0;
		break;
	case WM_KEYDOWN:
		return 0;
		break;
	case WM_KEYUP:
		return 0;
		break;
	case WM_MOUSEHWHEEL:
		return 0;
		break;
	case WM_SIZE:
		camera.SetScreenSize(lParam & 0xFFFF, lParam >> 16);
		return 0;
	case WM_SYSCOMMAND:
		switch(wParam)
		{
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
			return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Draw(HWText* text, HWHeightMapRenderer& hmr, HWShaderProgram& program, const HWTexture& tex, S32 frame)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.UseProgram();

	F32 radius = 5.f;
	F32 z = 3.f;
	F32 angle = frame / 1500.0f;
	glm::vec3 center = glm::vec3(4.0f, 4.0f, 0.f);
	camera.SetPosition(glm::vec3(cos(angle) * radius, sin(angle) * radius, z) + center);
	camera.SetLookAt(center);

	hmr.Render(&camera, &program);
	text->Render();
}

#pragma warning(suppress: 28251)
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HWWindow window = HWWindow("Hidden Worlds", 1024, 768, false, WndProc);
    window.CreateWindow();
    HWDeviceContext device = HWDeviceContext(window);
    device.CreateContext();
    window.ShowWindow(true);
    device.SetVSync(false);

	HWHeightMap map = HWHeightMap(8, 8);
	U8 heightData[8*8] = {
		 0,  0,  0,  0,  0,
	     0, 40, 40, 40,  0,
		 0, 40, 80, 40,  0,
		40, 20, 80, 20, 15,
		80, 90, 120, 100, 80,
		80, 90, 120, 100, 80,
		80, 90, 120, 100, 80,
		40,50,50,50,
		80, 90, 120, 100, 80,
		80, 90, 120, 100, 80,
		80, 90, 120, 100, 80,
		80, 90, 120, 100, 80,
		80, 90, 120, 100, 80,
	};

	U8 tileData[8*8] = {
		0, 1, 2, 3, 4,
		5, 6, 7, 8, 9,
		10,11,12,13,14,
		15,16,17,18,19,
		20,21,22,23,24,
		20,21,22,23,24,
		20,21,22,23,24,
		40,50,60,70,
		20,21,22,23,24,
		20,21,22,23,24,
		30,31,32,33,34,
		30,31,32,33,34,
		30,31,32,33,34,
	};

	map.SetData(tileData, heightData);

    HWShader vtx = HWShader(HWShaderType::VertexShader);
    vtx.ReadSource("shader.vtx");

    HWShader frag = HWShader(HWShaderType::FragmentShader);
    frag.ReadSource("shader.frag");

    HWShaderProgram program = HWShaderProgram();
    program.AddShader(vtx);
    program.AddShader(frag);
    program.CompileAndLink();

    HWTexture tex = HWTexture();
    tex.Read("test_grid.bmp");

	HWTexture tex2 = HWTexture();
    tex2.Read("marble.bmp");

	HWTexture tex3 = HWTexture();
    tex3.Read("glow.bmp");

	HWTileSet tiles = HWTileSet(&tex, &tex3, 10, 10);

	HWHeightMapRenderer hmr = HWHeightMapRenderer(&map, &tiles);

	HWVertexBuffer charVecs = HWVertexBuffer(HWBufferType::Static);
	HWIndexBuffer charIdx = HWIndexBuffer(HWIndexType::UnsignedShort);
	HWModel charModel = HWModel(charVecs, charIdx);
	charModel.ReadOBJ("teapot.obj");
	charModel.SetScale(glm::vec3(0.03f));
	charModel.SetRotation(glm::quat(glm::vec3(1.571,0,0)));
	charModel.SetTexture(0, &tex2);

	HWVertexFormat form = HWVertexFormat(2);
    form.AddComponent(HWVertexComponent(program, "in_vertex", HWVertexComponentType::Single, 3));
    form.AddComponent(HWVertexComponent(program, "in_uv", HWVertexComponentType::Single, 2));
	program.UseProgram();
	charVecs.SetFormat(form);

	HWTileCharacter player = HWTileCharacter(&charModel, 3, 3, 2, 50, 50);
	HWTileCharacter player2 = HWTileCharacter(&charModel, 5, 5, 2, 50, 50);
	hmr.AddCharacter(&player);
	hmr.AddCharacter(&player2);
	player.Select(true);
	hmr.Prepare(program);

	HWFont* font = new HWFont("vera.ttf", 48.0f, 512, 512);
	font->LoadGlyphs("0123456789");

	HWText* text = new HWText(font, "", glm::vec2(-1.f,.9f), glm::vec4(1,0,0,1));

	MSG msg;
	bool running = true;
	S32 frames = 0;
	S64 begin, end, freq;

	QueryPerformanceCounter((LARGE_INTEGER*)&begin);
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	while(running)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				running = false;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&end);
			F64 time = (F64)(end - begin) / (F64)freq;
			F64 fps = frames / time;

			std::ostringstream ss;
			ss << fps;
			text->SetText(ss.str());

			Draw(text, hmr, program, tex, frames);
			device.SwapBuffers();
			++frames;
		}
	}

	return 0;
}
