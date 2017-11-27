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
#include "math.h"

#define AUTHOR "AUTHOR: Oleksiy Ryabchun, " __TIMESTAMP__

DWORD API_VERSION = THRASH_API_VERSION;
CHAR* envPrefix = "OGL";

ThrashForced forced;
ThrashViewport viewport;

DWORD appWindowed = FALSE;
DWORD displayIndex = 0;
ThrashResolution resolutionsList[100];
DWORD resolutionsListCount = sizeof(resolutionsList) / sizeof(ThrashResolution);

BOOL extendedVertex;
BOOL specularEnabled;

BOOL texturesEnabled;
DWORD textureMipMap;
BOOL textureFilterEnabled;
DWORD textureClampS;
DWORD textureClampT;

DWORD depthCmp = GL_LEQUAL;
DWORD alphaCmp = GL_GEQUAL;
FLOAT alphaVal;

BOOL fogEnabled = FALSE;

BOOL stencilEnabled;
BOOL depthEnabled;

DWORD stencilFunc;
DWORD stencilFail;
DWORD stencilDepthFail;
DWORD stencilPass;

BOOL isInit = FALSE;
BOOL isWindowLocked = FALSE;

HGLRC hGlRc;

BOOL colorMask;

DWORD blendSrc;
DWORD blendDest;

DWORD bufferMode;
RECT clipRect;

BOOL vmChanged;
DWORD vmResolutionIndex;
DWORD vmMaxPanding;
ThrashDepthBufferType vmDepthBufferType;

BOOL textureColorFormats[COLOR_LAST + 1];
BOOL textureIndexFormats[INDEX_LAST + 1];

namespace Main
{
	DWORD __fastcall Round(FLOAT number)
	{
		FLOAT floorVal = floor(number);
		return DWORD(floorVal + 0.5f > number ? floorVal : ceil(number));
	}

	VOID __inline LoadForced()
	{
		TCHAR path[MAX_PATH];
		GetModuleFileName(hDllModule, path, MAX_PATH);
		TCHAR* dot = strrchr(path, '.');
		*dot = NULL;
		strcpy(iniFile, path);
		strcat(iniFile, ".ini");

		forced.windowed = GetEnvironmentValue(0, envPrefix, "WINDOWED");

		forced.resolution = GetEnvironmentValue(0, envPrefix, "RESOLUTION");
		if (forced.resolution && forced.resolution > 28)
			forced.resolution = 0;

		forced.colorDepth = GetEnvironmentValue(32, envPrefix, "COLORDEPTH");
		if (forced.colorDepth != 16 && forced.colorDepth != 24 && forced.colorDepth != 32)
			forced.colorDepth = 32;

		forced.zdepth = GetEnvironmentValue(24, envPrefix, "ZDEPTH");
		if (forced.zdepth != 16 && forced.zdepth != 24 && forced.zdepth != 32)
			forced.zdepth = 24;

		forced.refreshRate = GetEnvironmentValue(0, envPrefix, "REFRESH");
		if (forced.refreshRate && forced.refreshRate > 9)
			forced.refreshRate = 0;

		forced.exclusiveMode = GetEnvironmentValue(0, envPrefix, "EXCLUSIVE");
		forced.vSync = GetEnvironmentValue(1, envPrefix, "VSYNC");

		forced.aspect = GetEnvironmentValue(0, envPrefix, "ASPECT");
		forced.filtering = GetEnvironmentValue(0, envPrefix, "TEXFILTER");
		if (forced.filtering && forced.filtering > 2)
			forced.filtering = 0;

		forced.reconvert = GetEnvironmentValue(0, envPrefix, "TEXRGBA32");

		forced.add640x480x16 = GetEnvironmentValue(1, envPrefix, "ADD640X480X16");
		forced.movies16Bit = GetEnvironmentValue(0, envPrefix, "MOVIES16BIT");

		forced.gamma = GetEnvironmentValue(5, envPrefix, "GAMMA") * 0.1f + 0.5f;
		if (forced.gamma < 0.5f || forced.gamma > 1.5f)
			forced.gamma = 1.0f;
	}

	LPTHRASHABOUT THRASHAPI About()
	{
		if (!about.size)
		{
			//strcpy(about.signature, "OGL1"); strcpy(about.driverName, "OpenGL 1.4");
			strcpy(about.signature, "D3D7");  strcpy(about.driverName, "DX7 3rash");

			strcpy(about.deviceName, "D3D Device");

			about.size = sizeof(ThrashAbout);
			about.version = API_VERSION;
			about.flags = About_TextureSquare | About_TextureWidthPowerOf2 | About_TextureHeighPpowerOf2;
			about.textureWidthMin = 2;
			about.textureWidthMax = 512;
			about.textureWidthMultiple = 8;
			about.textureHeightMin = 2;
			about.textureHeightMax = 512;
			about.textureHeightMultiple = 8;
			about.clipAlign = 0;

			about.colorFormatsCount = COLOR_LAST;
			about.colorFormats = textureColorFormats;
			about.colorFormats[COLOR_ARGB_1555] = TRUE;
			about.colorFormats[COLOR_RGB_565] = TRUE;
			about.colorFormats[COLOR_RGB_888] = TRUE;
			about.colorFormats[COLOR_ARGB_8888] = TRUE;
			about.colorFormats[COLOR_ARGB_4444] = TRUE;

			about.indexFormatsCount = INDEX_LAST;
			about.indexFormats = textureIndexFormats;
			about.indexFormats[INDEX_RGB] = TRUE;
			about.indexFormats[INDEX_ARGB] = TRUE;

			about.resolutionsCount = resolutionsListCount;
			about.resolutionsList = resolutionsList;

			about.subType = DeviceVoodoo3;
			about.textureRamSize = 0x4000000; // 64 Mb
			about.textureRamType = 3;
			about.numStages = 4;
			about.author = AUTHOR;
			about.dxVeriosn = 7;

			LoadEnvironmentValues(&about, envPrefix);
		}
		return &about;
	}

	BOOL THRASHAPI Clip(RECT rect)
	{
		if (memcmp(&rect, &clipRect, sizeof(RECT)))
		{
			clipRect = rect;

			if (rect.left <= 0)
				rect.left = viewport.rectangle.x;
			else if (viewport.rectangle.x || viewport.rectangle.width != selectedResolution->width)
				rect.left = Round(viewport.point.x + viewport.clipFactor.x * (FLOAT)rect.left);

			if (rect.left >= viewport.rectangle.x + viewport.rectangle.width)
				rect.right = viewport.rectangle.x + viewport.rectangle.width;
			else if (viewport.rectangle.x || viewport.rectangle.width != selectedResolution->width)
				rect.right = Round(viewport.point.x + viewport.clipFactor.x * (FLOAT)rect.right);

			if (rect.top <= 0)
				rect.top = viewport.rectangle.y;
			else if (viewport.rectangle.y || viewport.rectangle.height != selectedResolution->height)
				rect.top = Round(viewport.point.y + viewport.clipFactor.y * (FLOAT)rect.top);

			if (rect.bottom >= viewport.rectangle.y + viewport.rectangle.height)
				rect.bottom = viewport.rectangle.y + viewport.rectangle.height;
			else if (viewport.rectangle.y || viewport.rectangle.height != selectedResolution->height)
				rect.bottom = Round(viewport.point.y + viewport.clipFactor.y * (FLOAT)rect.bottom);

			GLScissor(rect.left, viewport.height - rect.bottom, rect.right - rect.left, rect.bottom - rect.top);
		}

		return TRUE;
	}

	VOID THRASHAPI Idle() {}

	BOOL THRASHAPI Init()
	{
		if (!isInit)
		{
			isInit = TRUE;

			LoadForced();

			if (GetWindowLong(hWnd, GWL_STYLE) & WS_BORDER)
				forced.windowed = appWindowed = TRUE;

			CHAR library[256];
			if (GetEnvironmentVariable("THRASH_OGL_DRIVER", library, sizeof(library)))
				hModule = LoadLibrary(library);
			else
			{
				hModule = LoadLibrary("OPENGL32.DLL");
				if (!hModule)
					hModule = LoadLibrary("OPENGL.DLL");
			}

			if (hModule != NULL)
			{
				WGLGetProcAddress = (WGLGETPROCADDRESS)GetProcAddress(hModule, "wglGetProcAddress");
				WGLMakeCurrent = (WGLMAKECURRENT)GetProcAddress(hModule, "wglMakeCurrent");
				WGLCreateContext = (WGLCREATECONTEXT)GetProcAddress(hModule, "wglCreateContext");
				WGLDeleteContext = (WGLDELETECONTEXT)GetProcAddress(hModule, "wglDeleteContext");
				WGLSwapBuffers = (WGLSWAPBUFFERS)GetProcAddress(hModule, "wglSwapBuffers");
			}
			else
				return FALSE;

			Resolution::LoadList();

			if (functions.Event != NULL)
			{
				functions.Event(MSG_CHANGE_RESOLUTION, (PROC)Resolution::Change);
				functions.Event(MSG_RESET_RESOLUTION, (PROC)Resolution::Restore);
			}

			GammaRamp::Check();
		}

		return TRUE;
	}

	DWORD THRASHAPI Is()
	{
		return hModule != NULL ? 85 : 0;
	}

	VOID THRASHAPI PageFlip()
	{
		WGLSwapBuffers(hDc);

		if (forced.vSync)
			GLFinish();
	}

	BOOL THRASHAPI Restore()
	{
		Window::Release();
		Context::Release();

		if (hModule)
		{
			FreeLibrary(hModule);
			WGLGetProcAddress = NULL;
			WGLMakeCurrent = NULL;
			WGLCreateContext = NULL;
			WGLDeleteContext = NULL;
			hModule = NULL;
		}

		DWORD processId = GetWindowThreadProcessId(hWnd, NULL);
		if (functions.Event != NULL && functions.GetHWND != NULL && GetCurrentThreadId() != processId)
		{
			hWnd = NULL;
			hWnd = functions.GetHWND();
			hRestoreHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
			SetForegroundWindow(hWnd);
			PostMessage(hWnd, MSG_RESET_RESOLUTION, NULL, NULL);
			WaitForSingleObject(hRestoreHandle, 10000);
		}
		else
			Resolution::Restore(NULL, hWnd, MSG_RESET_RESOLUTION, NULL, NULL, NULL);

		return TRUE;
	}

	BOOL THRASHAPI SelectDisplay(DWORD index)
	{
		if (displayIndex != index)
		{
			displayIndex = index;
			Resolution::LoadList();
		}

		return TRUE;
	}

	BOOL THRASHAPI SetVideoMode(DWORD resolutionIndex, DWORD maxPanding, ThrashDepthBufferType depthBufferType)
	{
		if (vmChanged && resolutionIndex == vmResolutionIndex && maxPanding == vmMaxPanding && depthBufferType == vmDepthBufferType)
			return TRUE;

		vmChanged = TRUE;
		vmResolutionIndex = resolutionIndex;
		vmMaxPanding = maxPanding;
		vmDepthBufferType = depthBufferType;

		Window::Release();
		{
			Context::Release();
			{
				if (functions.GetHWND)
				{
					hWnd = functions.GetHWND();
					DWORD processId = GetWindowThreadProcessId(hWnd, NULL);
					if (functions.Event && GetCurrentThreadId() != processId)
					{
						hChangeHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
						SetForegroundWindow(hWnd);
						PostMessage(hWnd, MSG_CHANGE_RESOLUTION, resolutionIndex, maxPanding);
						WaitForSingleObject(hChangeHandle, 10000);
					}
					else
						Resolution::Change(NULL, hWnd, MSG_CHANGE_RESOLUTION, resolutionIndex, maxPanding, NULL);
				}
				else
					Resolution::Change(NULL, hWnd, MSG_CHANGE_RESOLUTION, resolutionIndex, maxPanding, NULL);
			}
			Context::Create();
		}
		Window::Window(2);

		GLHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		GLHint(GL_FOG_HINT, GL_DONT_CARE);
		GLHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
		GLHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
		GLHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);

		viewport.refresh = FALSE;
		GLViewport(viewport.rectangle.x, viewport.rectangle.y, viewport.rectangle.width, viewport.rectangle.height);

		GLMatrixMode(GL_PROJECTION);
		GLLoadIdentity();
		GLOrtho(0.0, (double)selectedResolution->width, (double)selectedResolution->height, 0.0, 0.0, 1.0);
		GLMatrixMode(GL_MODELVIEW);
		GLLoadIdentity();
		GLScalef(1.0, 1.0, -1.0);
		GLDepthRange(0.0, 1.0);

		if (forced.aspect)
		{
			GLClearColor(0.0, 0.0, 0.0, 1.0);
			GLClear(GL_COLOR_BUFFER_BIT);

			Window::Window(3);
			GLClear(GL_COLOR_BUFFER_BIT);

			Window::Window(1);
			GLClear(GL_COLOR_BUFFER_BIT);
		}

		Window::Window(0);
		if (forced.aspect)
			GLClear(GL_COLOR_BUFFER_BIT);

		GLEnable(GL_SCISSOR_TEST);
		memset(&clipRect, NULL, sizeof(RECT));
		RECT rect = { 0, 0, selectedResolution->width, selectedResolution->height };
		Clip(rect);

		GLEnable(GL_COLOR_SUM); // For specular

		State::Set(State::Hint, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "HINT"));
		State::Set(State::CullFace, 0xFFFF0000, GetEnvironmentValue(1, envPrefix, "CULL"));
		State::Set(State::TextureFilter, 0xFFFF0000, GetEnvironmentValue(1, envPrefix, "FILTER"));
		State::Set(State::ShadeModel, 0xFFFF0000, GetEnvironmentValue(1, envPrefix, "SHADE"));
		State::Set(State::EnableAlphaBlend, 0xFFFF0000, GetEnvironmentValue(2, envPrefix, "TRANSPARENCY"));
		State::Set(State::AlphaMode, 0xFFFF0000, GetEnvironmentValue(16, envPrefix, "ALPHATEST"));
		State::Set(State::TextureMipMap, 0xFFFF0000, GetEnvironmentValue(1, envPrefix, "MIPMAP"));
		State::Set(State::ClearColor, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "BACKGROUNDCOLOUR"));
		State::Set(State::ChromaColor, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "CHROMACOLOUR"));
		State::Set(State::EnableDither, 0xFFFF0000, GetEnvironmentValue(1, envPrefix, "DITHER"));
		State::Set(State::EnableFog, 0xFFFF0000, FALSE);
		State::Set(State::FogMode, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "FOGMODE"));
		State::Set(State::FogDensity, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "FOGDENSITY"));
		State::Set(State::FogStart, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "STATE_FOGZNEAR"));
		State::Set(State::FogEnd, 0xFFFF0000, GetEnvironmentValue(0x3F800000, envPrefix, "FOGZFAR"));
		State::Set(State::FogColor, 0xFFFF0000, GetEnvironmentValue(0xFFFFFFFF, envPrefix, "FOGCOLOUR"));
		State::Set(State::BlendMode, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "ALPHA"));
		State::Set(State::TextureClamp, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "TEXTURECLAMP"));
		State::Set(State::Gamma, 0xFFFF0000, 0x3F800000);
		State::Set(State::DepthBias, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "DEPTHBIAS"));
		State::Set(State::TextureLodBias, 0xFFFF0000, 0);
		State::Set(State::MaxPending, 0xFFFF0000, GetEnvironmentValue(maxPanding - 2 < 0 ? 0 : maxPanding - 2, envPrefix, "MAXPENDING")); // TODO
		State::Set(State::VertexLayout, 0xFFFF0000, 0);
		State::Set(State::TexturesCombine, 0xFFFF0000, 0);
		State::Set(State::TexturesCombine, 0xFFFF0001, 2);
		State::Set(State::BackBufferType, 0xFFFF0000, GetEnvironmentValue(1, envPrefix, "BACKBUFFERTYPE"));
		State::Set(State::FlatFans, 0xFFFF0000, GetEnvironmentValue(1, envPrefix, "FLATFANS"));
		State::Set(State::LineWidth, 0xFFFF0000, GetEnvironmentValue(1, envPrefix, "LINEWIDTH"));
		State::Set(State::EnableStencilBuffer, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "STENCILBUFFER"));
		State::Set(State::DisplayMode, 0xFFFF0000, GetEnvironmentValue(resolutionIndex, envPrefix, "DISPLAYMODE"));
		State::Set(State::LineDouble, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "LINEDOUBLE"));
		State::Set(State::BufferMode, 0xFFFF0000, 0);
		State::Set(State::ClearDepth, 0xFFFF0000, 0x3F800000);
		State::Set(State::FlipRate, 0xFFFF0000, GetEnvironmentValue(1, envPrefix, "FLIPRATE"));
		State::Set(State::ShamelessPlug, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "SHAMELESSPLUG"));
		State::Set(State::EnableDepthBuffer, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "DEPTHBUFFER"));
		State::Set(State::DepthCompare, 0xFFFF0000, GetEnvironmentValue(3, envPrefix, "DEPTHCMP"));

		return TRUE;
	}

	BOOL THRASHAPI Sync(DWORD value)
	{
		return TRUE;
	}
}