/*
	MIT License

	Copyright (c) 2017 Oleksiy Ryabchun

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "stdafx.h"
#include "Global.hpp"

#define WC_DRAW "40bd3b41-ff62-4227-b8d5-dae24eb338b7"

namespace Context
{
	VOID __fastcall ResetPixelFormatDescription(PIXELFORMATDESCRIPTOR* pfd)
	{
		MemoryZero(pfd, sizeof(PIXELFORMATDESCRIPTOR));
		pfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd->nVersion = 1;
	}

	VOID __fastcall PreparePixelFormatDescription(PIXELFORMATDESCRIPTOR* pfd)
	{
		ResetPixelFormatDescription(pfd);

		pfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SWAP_EXCHANGE;
		pfd->cColorBits = LOBYTE(forced.colorDepth);
		pfd->cDepthBits = LOBYTE(forced.zdepth);
		pfd->cStencilBits = 8;
	}

	BOOL __fastcall PreparePixelFormat(PIXELFORMATDESCRIPTOR* pfd, DWORD* pixelFormat)
	{
		BOOL res = FALSE;

		WNDCLASS wc;
		MemoryZero(&wc, sizeof(WNDCLASS));
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = DefWindowProc;
		wc.hInstance = hDllModule;
		wc.lpszClassName = WC_DRAW;

		if (RegisterClass(&wc))
		{
			HWND hWndDraw = CreateWindowEx(
				WS_EX_APPWINDOW,
				WC_DRAW,
				"DUMMY",
				WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
				0, 0,
				1, 1,
				NULL,
				NULL,
				NULL,
				NULL);

			if (hWndDraw)
			{
				HDC hDcDraw = GetDC(hWndDraw);
				if (hDcDraw)
				{
					PreparePixelFormatDescription(pfd);
					*pixelFormat = ChoosePixelFormat(hDcDraw, pfd);
					if (*pixelFormat)
					{
						if (SetPixelFormat(hDcDraw, *pixelFormat, pfd))
						{
							HGLRC hRcDraw = WGLCreateContext(hDcDraw);
							if (hRcDraw)
							{
								if (WGLMakeCurrent(hDcDraw, hRcDraw))
								{
									WGLChoosePixelFormat = (PFNWGLCHOOSEPIXELFORMATARBPROC)WGLGetProcAddress("wglChoosePixelFormatARB");
									if (WGLChoosePixelFormat)
									{
										INT piFormats[128];
										UINT nNumFormats = 0;

										INT glAttributes[] = {
											WGL_DRAW_TO_WINDOW_ARB, (pfd->dwFlags & PFD_DRAW_TO_WINDOW) ? GL_TRUE : GL_FALSE,
											WGL_SUPPORT_OPENGL_ARB, (pfd->dwFlags & PFD_SUPPORT_OPENGL) ? GL_TRUE : GL_FALSE,
											WGL_DOUBLE_BUFFER_ARB, (pfd->dwFlags & PFD_DOUBLEBUFFER) ? GL_TRUE : GL_FALSE,
											WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
											WGL_COLOR_BITS_ARB, pfd->cColorBits,
											WGL_DEPTH_BITS_ARB, pfd->cDepthBits,
											WGL_STENCIL_BITS_ARB, pfd->cStencilBits,
											WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
											WGL_SWAP_METHOD_ARB, (pfd->dwFlags & PFD_SWAP_EXCHANGE) ? WGL_SWAP_EXCHANGE_ARB : WGL_SWAP_COPY_ARB,
											0
										};

										if (WGLChoosePixelFormat(hDcDraw, glAttributes, NULL, sizeof(piFormats) / sizeof(INT), piFormats, &nNumFormats) && nNumFormats)
											*pixelFormat = piFormats[0];
									}

									res = TRUE;
									WGLMakeCurrent(hDcDraw, NULL);
								}

								WGLDeleteContext(hRcDraw);
							}
						}
					}

					ReleaseDC(hWndDraw, hDcDraw);
				}

				DestroyWindow(hWndDraw);
			}

			UnregisterClass(WC_DRAW, hDllModule);
		}

		return res;
	}

	VOID __fastcall Create()
	{
		if (hRc)
			return;

		if (!hDc)
			hDc = GetDC(hWnd);

		if (!::GetPixelFormat(hDc))
		{
			PIXELFORMATDESCRIPTOR pfd;
			DWORD pfIndex;
			if (!PreparePixelFormat(&pfd, &pfIndex))
			{
				pfIndex = ChoosePixelFormat(hDc, &pfd);
				if (pfIndex == NULL)
					Main::ShowError("ChoosePixelFormat failed", __FILE__, "Create", __LINE__);
				else if (pfd.dwFlags & PFD_NEED_PALETTE)
					Main::ShowError("Needs palette", __FILE__, "Create", __LINE__);
			}

			if (!SetPixelFormat(hDc, pfIndex, &pfd))
				Main::ShowError("SetPixelFormat failed", __FILE__, "Create", GetLastError());

			ResetPixelFormatDescription(&pfd);
			if (!DescribePixelFormat(hDc, pfIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
				Main::ShowError("DescribePixelFormat failed", __FILE__, "Create", __LINE__);

			if ((pfd.iPixelType != PFD_TYPE_RGBA) || (pfd.cRedBits < 5) || (pfd.cGreenBits < 5) || (pfd.cBlueBits < 5))
				Main::ShowError("Bad pixel type", __FILE__, "Create", __LINE__);
		}

		hRc = WGLCreateContext(hDc);
		WGLMakeCurrent(hDc, hRc);
		CreateContextAttribs(hDc, &hRc);

		if (forced.vSync)
		{
			if (WGLSwapInterval)
				WGLSwapInterval(1);
			else
				forced.vSync = FALSE;
		}

		Shaders::Prepare();
		Buffer::Create();
	}

	VOID __fastcall Release()
	{
		if (hRc)
		{
			Buffer::Draw();
			GLFinish();
			Texture::Reset();
			Shaders::Release();
			Buffer::Release();
		}

		if (hDc)
		{
			WGLMakeCurrent(hDc, NULL);
			ReleaseDC(hWnd, hDc);
			hDc = NULL;
		}

		if (hRc)
		{
			WGLDeleteContext(hRc);
			hRc = NULL;
		}
	}
}