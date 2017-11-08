#include "stdafx.h"
#include "Global.hpp"

namespace Context
{
	LRESULT CALLBACK DummyWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	BOOL __inline PreparePixelFormat(PIXELFORMATDESCRIPTOR* pfd, DWORD* pixelFormat, BOOL isDepthBuffer16)
	{
		HINSTANCE hIntance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

		CHAR* dummyclassname = "40bd3b41-ff62-4227-b8d5-dae24eb338b7";
		WNDCLASS wc = { NULL };
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
		wc.lpfnWndProc = DummyWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hIntance;
		wc.hIcon = NULL;
		wc.hCursor = NULL;
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = dummyclassname;

		if (RegisterClass(&wc))
		{
			HWND dummyhWnd = CreateWindowEx(
				WS_EX_APPWINDOW,
				dummyclassname,
				"DUMMY",
				WS_POPUP |
				WS_CLIPSIBLINGS |
				WS_CLIPCHILDREN,
				0, 0, // Window Position
				1, 1, // Window size
				NULL, // No Parent Window
				NULL, // No Menu
				hIntance,
				NULL
			);

			if (dummyhWnd != NULL)
			{
				HDC dummyDc = GetDC(dummyhWnd);
				if (dummyDc != NULL)
				{
					*pixelFormat = ChoosePixelFormat(dummyDc, pfd);
					if (*pixelFormat != 0)
					{
						if (SetPixelFormat(dummyDc, *pixelFormat, pfd))
						{
							HGLRC hRc = WGLCreateContext(dummyDc);
							if (hRc != NULL)
							{
								if (WGLMakeCurrent(dummyDc, hRc))
								{
									UINT numGlFormats;
									PFNWGLCHOOSEPIXELFORMATARBPROC WGLChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)WGLGetProcAddress("wglChoosePixelFormatARB");

									if (WGLChoosePixelFormatARB != NULL)
									{
										DWORD attribList[] = {
											WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
											WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
											WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
											WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
											WGL_COLOR_BITS_ARB, forced.colorDepth,
											WGL_DEPTH_BITS_ARB, (!forced.zdepth ? (isDepthBuffer16 ? 16 : 24) : forced.zdepth),
											WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
											0
										};

										int glPixelFormats[128];
										if (WGLChoosePixelFormatARB(dummyDc, (const int*)attribList, NULL, sizeof(glPixelFormats) / sizeof(int), glPixelFormats, &numGlFormats))
											if (numGlFormats >= 1)
												*pixelFormat = glPixelFormats[0];
									}

									if (hRc != NULL)
									{
										WGLMakeCurrent(NULL, NULL);
										WGLDeleteContext(hRc);
										hRc = NULL;
									}

									if (dummyDc != NULL)
									{
										ReleaseDC(dummyhWnd, dummyDc);
										dummyDc = NULL;
									}

									DestroyWindow(dummyhWnd);
									UnregisterClass(dummyclassname, hIntance);

									return TRUE;
								}
							}
						}
					}
				}
			}
		}

		return FALSE;
	}

	VOID __fastcall Create(BOOL isDepthBuffer16)
	{
		if (hGlRc)
			return;

		if (!hDc)
			hDc = GetDC(hWnd);

		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
			1, // Version Number
			PFD_DRAW_TO_WINDOW | // Format Must Support Window
			PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
			PFD_DOUBLEBUFFER, // Must Support Double Buffering
			PFD_TYPE_RGBA, // Request An RGBA Format
			forced.colorDepth, // Select Our Color Depth
			0, 0, 0, 0, 0, 0, // Color Bits Ignored
			0, // No Alpha Buffer
			0, // Shift Bit Ignored
			0, // No Accumulation Buffer
			0, 0, 0, 0, // Accumulation Bits Ignored
			(!forced.zdepth ? (isDepthBuffer16 ? 16 : 24) : forced.zdepth), // Z-Buffer (Depth Buffer) 
			(!forced.zdepth ? (isDepthBuffer16 ? 0 : 8) : (forced.zdepth == 24 ? 8 : 0)), // Stencil Buffer
			0, // No Auxiliary Buffer
			PFD_MAIN_PLANE, // Main Drawing Layer
			0, // Reserved
			0, 0, 0 // Layer Masks Ignored
		};

		DWORD pfIndex;
		if (!PreparePixelFormat(&pfd, &pfIndex, isDepthBuffer16))
		{
			pfIndex = ChoosePixelFormat(hDc, &pfd);
			if (pfIndex == NULL)
				Main::ShowError("ChoosePixelFormat failed", __FILE__, "Create", __LINE__);
			else if (pfd.dwFlags & PFD_NEED_PALETTE)
				Main::ShowError("Needs palette", __FILE__, "Create", __LINE__);
		}

		if (!SetPixelFormat(hDc, pfIndex, &pfd))
			Main::ShowError("SetPixelFormat failed", __FILE__, "Create", __LINE__);

		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		if (DescribePixelFormat(hDc, pfIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == NULL)
			Main::ShowError("DescribePixelFormat failed", __FILE__, "Create", __LINE__);

		if ((pfd.iPixelType != PFD_TYPE_RGBA) ||
			(pfd.cRedBits < 5) ||
			(pfd.cGreenBits < 5) ||
			(pfd.cBlueBits < 5))
			Main::ShowError("Bad pixel type", __FILE__, "Create", __LINE__);

		hGlRc = WGLCreateContext(hDc);
		WGLMakeCurrent(hDc, hGlRc);
		CreateContextAttribs(&hDc, &hGlRc);

		if (WGLSwapInterval)
			WGLSwapInterval(!forced.windowed && forced.exclusiveMode && forced.vSync);

		Shaders::Prepare();
		Buffer::Create();
	}

	VOID __fastcall Release()
	{
		if (!hGlRc)
			return;

		Buffer::Draw();
		GLFinish();
		Texture::Reset();
		Shaders::Release();
		Buffer::Release();

		if (hDc)
			WGLMakeCurrent(hDc, NULL);

		WGLDeleteContext(hGlRc);
		hGlRc = NULL;

		if (hDc)
		{
			ReleaseDC(hWnd, hDc);
			hDc = NULL;
		}
	}
}