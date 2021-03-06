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

DWORD appWindowed;
DWORD displayIndex = 0;
ThrashResolution resolutionsList[100];
DWORD resolutionsListCount;

BOOL texturesEnabled;
DWORD textureMipMap;
BOOL textureFilterEnabled;
DWORD textureClampS;
DWORD textureClampT;
FLOAT textureLodBias;

DWORD stencilFunc;
DWORD stencilFail = GL_KEEP;
DWORD stencilDepthFail = GL_KEEP;
DWORD stencilPass = GL_KEEP;
DWORD depthCmp = GL_LEQUAL;
FLOAT depthBias;
BOOL colorMask;
BOOL stencilEnabled;
BOOL depthEnabled;

DWORD cullFace;

BOOL alphaEnabled;
DWORD alphaVal;
DWORD alphaCmp;

DWORD blendSrc;
DWORD blendDest;

DWORD bufferMode;
RECT clipRect;

BOOL isInit, isForced;
BOOL isWindowLocked;

HGLRC hRc;

GLint attrCoordsLoc;
GLint attrDiffuseLoc;
GLint attrSpecularLoc;
GLint attrTexCoordsLoc;

GLint uniMVPLoc;

GLuint uniTexEnabledLoc;
GLuint uniShadeModelLoc;
GLuint uniAlphaFuncLoc;
GLuint uniAlphaValLoc;
GLuint uniFogModeLoc;
GLuint uniFogColorLoc;
GLuint uniFogStartLoc;
GLuint uniFogEndLoc;
GLuint uniFogDensityLoc;
GLuint uniSpecularEnabledLoc;
GLuint uniGammaLoc;

BOOL vmChanged;
DWORD vmResolutionIndex;
DWORD vmMaxPanding;
ThrashDepthBufferType vmDepthBufferType;

BOOL textureColorFormats[COLOR_LAST + 1];
BOOL textureIndexFormats[INDEX_LAST + 1];

namespace Main
{
	VOID __inline LoadForced()
	{
		if (isForced)
			return;
		isForced = TRUE;

		CHAR path[MAX_PATH];
		GetModuleFileName(hDllModule, path, MAX_PATH);
		CHAR* dot = StrLastChar(path, '.');
		*dot = NULL;
		StrCopy(iniFile, path);
		StrCat(iniFile, ".ini");

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

		forced.add640x480x16 = GetEnvironmentValue(1, envPrefix, "ADD640X480X16");
		forced.movies16Bit = GetEnvironmentValue(0, envPrefix, "MOVIES16BIT");

		forced.gamma = GetEnvironmentValue(5, envPrefix, "GAMMA") * 0.1f + 0.5f;
		if (forced.gamma < 0.5f || forced.gamma > 1.5f)
			forced.gamma = 1.0f;

		gamma = 1.0f / forced.gamma;

		forced.reconvert = GetEnvironmentValue(0, envPrefix, "TEX_CONVERT_ARGB32");

		forced.texColor_INDEX_4 = GetEnvironmentValue(1, envPrefix, "TEX_COLOR_INDEX_4");
		forced.texColor_INDEX_8 = GetEnvironmentValue(1, envPrefix, "TEX_COLOR_INDEX_8");
		forced.texColor_ARGB_1555 = GetEnvironmentValue(1, envPrefix, "TEX_COLOR_ARGB_1555");
		forced.texColor_RGB_565 = GetEnvironmentValue(1, envPrefix, "TEX_COLOR_RGB_565");
		forced.texColor_RGB_888 = GetEnvironmentValue(1, envPrefix, "TEX_COLOR_RGB_888");
		forced.texColor_ARGB_8888 = GetEnvironmentValue(1, envPrefix, "TEX_COLOR_ARGB_8888");
		forced.texColor_ARGB_4444 = GetEnvironmentValue(1, envPrefix, "TEX_COLOR_ARGB_4444");

		forced.texIndex_RGB = GetEnvironmentValue(1, envPrefix, "TEX_INDEX_RGB");
		forced.texIndex_ARGB = GetEnvironmentValue(1, envPrefix, "TEX_INDEX_ARGB");

		Resolution::LoadList();
	}

	LPTHRASHABOUT THRASHAPI About()
	{
		if (!about.size)
		{
			LoadForced();

			StrCopy(about.signature, "OGL3"); StrCopy(about.driverName, "OpenGL 3.0");
			StrCopy(about.deviceName, "D3D Device");

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
			about.colorFormats[COLOR_INDEX_4] = forced.texColor_INDEX_4;
			about.colorFormats[COLOR_INDEX_8] = forced.texColor_INDEX_8;
			about.colorFormats[COLOR_ARGB_1555] = forced.texColor_ARGB_1555;
			about.colorFormats[COLOR_RGB_565] = forced.texColor_RGB_565;
			about.colorFormats[COLOR_RGB_888] = forced.texColor_RGB_888;
			about.colorFormats[COLOR_ARGB_8888] = forced.texColor_ARGB_8888;
			about.colorFormats[COLOR_ARGB_4444] = forced.texColor_ARGB_4444;

			about.indexFormatsCount = INDEX_LAST;
			about.indexFormats = textureIndexFormats;
			about.indexFormats[INDEX_RGB] = forced.texIndex_RGB;
			about.indexFormats[INDEX_ARGB] = forced.texIndex_ARGB;

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
			Buffer::Draw();

			if (rect.left <= 0)
				rect.left = viewport.rectangle.x;
			else if (viewport.rectangle.x || viewport.rectangle.width != selectedResolution->width)
				rect.left = (LONG)MathRound(viewport.point.x + viewport.clipFactor.x * (FLOAT)rect.left);

			if (rect.left >= viewport.rectangle.x + viewport.rectangle.width)
				rect.right = viewport.rectangle.x + viewport.rectangle.width;
			else if (viewport.rectangle.x || viewport.rectangle.width != selectedResolution->width)
				rect.right = (LONG)MathRound(viewport.point.x + viewport.clipFactor.x * (FLOAT)rect.right);

			if (rect.top <= 0)
				rect.top = viewport.rectangle.y;
			else if (viewport.rectangle.y || viewport.rectangle.height != selectedResolution->height)
				rect.top = (LONG)MathRound(viewport.point.y + viewport.clipFactor.y * (FLOAT)rect.top);

			if (rect.bottom >= viewport.rectangle.y + viewport.rectangle.height)
				rect.bottom = viewport.rectangle.y + viewport.rectangle.height;
			else if (viewport.rectangle.y || viewport.rectangle.height != selectedResolution->height)
				rect.bottom = (LONG)MathRound(viewport.point.y + viewport.clipFactor.y * (FLOAT)rect.bottom);

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

			CHAR library[MAX_PATH];
			GetModuleFileName(hDllModule, library, sizeof(library));
			CHAR* p = StrLastChar(library, '\\') + 1;
			*p = NULL;
			StrCat(library, "OPENGL32.DLL");
			hModule = LoadLibrary(library);
			if (!hModule)
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

			if (functions.Event != NULL)
			{
				functions.Event(MSG_CHANGE_RESOLUTION, (PROC)Resolution::Change);
				functions.Event(MSG_RESET_RESOLUTION, (PROC)Resolution::Restore);
			}
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
		Window::Window(2);

		GLHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
		GLHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);

		viewport.refresh = FALSE;
		GLViewport(viewport.rectangle.x, viewport.rectangle.y, viewport.rectangle.width, viewport.rectangle.height);

		FLOAT right = (FLOAT)selectedResolution->width;
		FLOAT bottom = (FLOAT)selectedResolution->height;
		FLOAT zFar = 1.0f;

		FLOAT mvpMatrix[4][4] = {
			2.0f / right,           0.0f,        0.0f, 0.0f,
					0.0f, -2.0f / bottom,        0.0f, 0.0f,
					0.0f,           0.0f, 2.0f / zFar, 0.0f,
					-1.0,            1.0,        -1.0, 1.0f
		};

		GLUniformMatrix4fv(uniMVPLoc, 1, GL_FALSE, (GLfloat*)mvpMatrix);
		GLDepthRange(0, 1.0);

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
		MemoryZero(&clipRect, sizeof(RECT));
		RECT rect = { 0, 0, selectedResolution->width, selectedResolution->height };
		Clip(rect);

		GLActiveTexture(GL_TEXTURE0);

		State::Set(State::Hint, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "HINT"));
		State::Set(State::CullFace, 0xFFFF0000, GetEnvironmentValue(1, envPrefix, "CULL"));
		State::Set(State::TextureFilter, 0xFFFF0000, GetEnvironmentValue(1, envPrefix, "FILTER"));
		State::Set(State::ShadeModel, 0xFFFF0000, GetEnvironmentValue(1, envPrefix, "SHADE"));
		State::Set(State::EnableAlphaBlend, 0xFFFF0000, GetEnvironmentValue(2, envPrefix, "TRANSPARENCY"));
		State::Set(State::AlphaValue, 0xFFFF0000, GetEnvironmentValue(16, envPrefix, "ALPHATEST"));
		State::Set(State::TextureMipMap, 0xFFFF0000, GetEnvironmentValue(1, envPrefix, "MIPMAP"));
		State::Set(State::ClearColor, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "BACKGROUNDCOLOUR"));
		State::Set(State::ChromaKeyColor, 0xFFFF0000, GetEnvironmentValue(0, envPrefix, "CHROMACOLOUR"));
		State::Set(State::DitherMode, 0xFFFF0000, GetEnvironmentValue(1, envPrefix, "DITHER"));
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