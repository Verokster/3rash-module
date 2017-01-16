#include "stdafx.h"
#include "Global.hpp"
#include <math.h>

#define AUTHOR "AUTHOR: Oleksiy Ryabchun, " __TIMESTAMP__

DWORD API_VERSION = THRASH_API_VERSION;
CHAR* envPrefix = "OGL";

ThrashForced forced;
ThrashViewport viewport;

ThrashTexColorFormats textureFormats = { 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, -1 };
ThrashTexColorIndexFormats textureIndexFormats = { 0, 0, 0, 1, 1, -1, 0 };

DWORD appWindowed = FALSE;
DWORD displayIndex = 0;
ThrashResolution resolutionsList[100];
DWORD resolutionsListCount = ARRAYSIZE(resolutionsList);

BOOL texturesEnabled = FALSE;
DWORD textureMipMap = 0;
BOOL textureFilterEnabled = FALSE;
DWORD textureWrap = 1;
DWORD stencilFail = GL_KEEP;
DWORD stencilDepthFail = GL_KEEP;
DWORD stencilPass = GL_KEEP;
DWORD depthCmp = GL_LEQUAL;

BOOL shadeModel;
DWORD cullFace;

BOOL alphaEnabled;
DWORD alphaFunc;

BOOL fogEnabled;
DWORD fogMode;
DWORD fogColor;
DWORD fogStart;
DWORD fogEnd;
DWORD fogDensity;

DWORD gamma = 1.0;

BOOL isInit = FALSE;
BOOL isWindowLocked = FALSE;

HGLRC hGlRc;

GLint attrCoordsLoc;
GLint attrColorLoc;
GLint attrTexCoordsLoc;
GLint attrTexUnitLoc;

GLint uniMVPLoc;

GLuint uniShadeModelLoc;
GLuint uniAlphaEnabledLoc;
GLuint uniAlphaFuncLoc;
GLuint uniFogModeLoc;
GLuint uniFogColorLoc;
GLuint uniFogStartLoc;
GLuint uniFogEndLoc;
GLuint uniFogDensityLoc;
GLuint uniGammaLoc;

DWORD currTextureUnit;
DWORD drawTextureUnit;

namespace Main
{
	DWORD __fastcall Round(FLOAT number)
	{
		FLOAT floorVal = floor(number);
		return floorVal + 0.5 > number ? floorVal : ceil(number);
	}

	VOID __inline LoadForced()
	{
		forced.resolution = GetEnvironmentValue(0, envPrefix, "RESOLUTION");
		forced.refreshRate = GetEnvironmentValue(0, envPrefix, "REFRESH");
		forced.exclusiveMode = GetEnvironmentValue(1, envPrefix, "EXCLUSIVE");
		forced.vSync = GetEnvironmentValue(1, envPrefix, "VSYNC");
		forced.aspect = GetEnvironmentValue(0, envPrefix, "ASPECT");
		
		DWORD gammaVal = GetEnvironmentValue(5, envPrefix, "GAMMA");
		switch (gammaVal)
		{
			case 0:
				forced.gamma = 0.5;
				break;
			case 1:
				forced.gamma = 0.6;
				break;
			case 2:
				forced.gamma = 0.7;
				break;
			case 3:
				forced.gamma = 0.8;
				break;
			case 4:
				forced.gamma = 0.9;
				break;
			case 6:
				forced.gamma = 1.1;
				break;
			case 7:
				forced.gamma = 1.2;
				break;
			case 8:
				forced.gamma = 1.3;
				break;
			case 9:
				forced.gamma = 1.4;
				break;
			case 10:
				forced.gamma = 1.5;
				break;
			default:
				forced.gamma = 1.0;
				break;
		}

		forced.indexedVBO = GetEnvironmentValue(0, envPrefix, "INDEXED_VBO");
	}
	
	ThrashAbout* __stdcall About()
	{
		if (!about.signature)
		{
			about.signature = 0x4F50454E;
			about.size = sizeof(ThrashAbout);
			about.version = API_VERSION;
			about.flags = About_TextureSquare | About_TextureWidthPowerOf2 | About_TextureHeighPpowerOf2;
			about.textureWidthMin = 1;
			about.textureWidthMax = 256;
			about.textureWidthMultiple = 8;
			about.textureHeightMin = 1;
			about.textureHeightMax = 256;
			about.textureHeightMultiple = 8;
			about.clipAlign = 1;

			about.texFormatsCount = 9;
			about.texFormats = &textureFormats;

			about.texIndexFormatsCount = 4;
			about.texIndexFormats = &textureIndexFormats;

			about.resolutionsCount = resolutionsListCount;
			about.resolutionsList = resolutionsList;

			strcpy(about.driverName, "OpenGL 3.0");
			about.subType = 0;
			about.textureRamSize = 0x1000000;
			about.author = AUTHOR;

			LoadEnvironmentValues(&about, envPrefix);
		}
		return &about;
	}

	BOOL __stdcall Clip(RECT rect)
	{
		Buffer::Draw();

		if (viewport.width != (*selectedResolution).width)
		{
			rect.left = viewport.rectangle.x + Round((FLOAT)rect.left * viewport.clipFactor.x);
			rect.right = viewport.rectangle.x + Round((FLOAT)rect.right * viewport.clipFactor.x);
		}

		if (viewport.height != (*selectedResolution).height)
		{
			rect.top = viewport.rectangle.y + Round((FLOAT)rect.top * viewport.clipFactor.y);
			rect.bottom = viewport.rectangle.y + Round((FLOAT)rect.bottom * viewport.clipFactor.y);
		}

		GLScissor(rect.left, viewport.height - rect.bottom, rect.right - rect.left, rect.bottom - rect.top);

		return TRUE;
	}

	VOID __stdcall Idle() {}
	
	BOOL __stdcall Init()
	{
		if (!isInit)
		{
			isInit = TRUE;

			if (GetWindowLong(hWnd, GWL_STYLE) & WS_BORDER)
				appWindowed = TRUE;

			HMODULE hModule;

			char* library = getenv("THRASH_OGL_DRIVER");
			if (library)
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

			if (funcResolution != NULL)
			{
				funcResolution(MSG_CHANGE_RESOLUTION, (PROC)Resolution::Change);
				funcResolution(MSG_RESET_RESOLUTION, (PROC)Resolution::Restore);
			}

			LoadForced();
		}

		return TRUE;
	}

	DWORD __stdcall Is()
	{
		return hModule != NULL ? 80 : 0;
	}

	VOID __stdcall PageFlip()
	{
		Buffer::Draw();
		WGLSwapBuffers(hDc);
	}

	BOOL __stdcall Restore()
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
		if (funcResolution != NULL && GetCurrentThreadId() != processId)
		{
			hWnd = NULL;
			hWnd = funcGetHWND();
			hRestoreHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
			SetForegroundWindow(hWnd);
			PostMessage(hWnd, MSG_RESET_RESOLUTION, NULL, NULL);
			WaitForSingleObject(hRestoreHandle, 10000);
		}
		else
			Resolution::Restore(NULL, hWnd, MSG_RESET_RESOLUTION, NULL, NULL, NULL);

		return TRUE;
	}

	BOOL __stdcall SelectDisplay(DWORD index)
	{
		if (displayIndex != index)
		{
			displayIndex = index;
			Resolution::LoadList();
		}

		return TRUE;
	}

	BOOL __stdcall SetVideoMode(DWORD resolutionIndex, DWORD maxPanding, BOOL isDepthBuffer16)
	{
		DWORD processId = GetWindowThreadProcessId(hWnd, NULL);
		if (funcResolution != NULL && GetCurrentThreadId() != processId)
		{
			hWnd = funcGetHWND();
			hChangeHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
			SetForegroundWindow(hWnd);
			PostMessage(hWnd, MSG_CHANGE_RESOLUTION, resolutionIndex, maxPanding);
			WaitForSingleObject(hChangeHandle, 10000);
		}
		else
			Resolution::Change(NULL, hWnd, MSG_CHANGE_RESOLUTION, resolutionIndex, maxPanding, NULL);

		hDc = GetDC(hWnd);
		if (hGlRc == NULL)
			Context::Create(isDepthBuffer16);

		Window::Window(2);

		GLHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
		GLHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
		
		GLViewport(viewport.rectangle.x, viewport.rectangle.y, viewport.rectangle.width, viewport.rectangle.height);

		glm::mat4 pMatrix = glm::ortho(
			0.0f,
			static_cast<float>((*selectedResolution).width),
			static_cast<float>((*selectedResolution).height),
			0.0f,
			0.0f,
			1.0f
		);

		glm::mat4 mvMatrix = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, -1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		glm::mat4 mvpMatrix = pMatrix * mvMatrix;
		GLUniformMatrix4fv(uniMVPLoc, 1, GL_FALSE, &mvpMatrix[0][0]);

		GLClearDepth(1.0);
		GLDepthRange(0, 1.0);

		GLEnable(GL_SCISSOR_TEST);

		Window::Window(1);
		Window::Unlock(Window::Lock());
		Window::Window(0);

		State::Set(Hint, GetEnvironmentValue(0, envPrefix, "HINT"));
		State::Set(CullFace, GetEnvironmentValue(1, envPrefix, "CULL"));
		State::Set(TextureFilter, GetEnvironmentValue(1, envPrefix, "FILTER"));
		State::Set(ShadeModel, GetEnvironmentValue(1, envPrefix, "SHADE"));
		State::Set(EnableAlphaBlend, GetEnvironmentValue(2, envPrefix, "TRANSPARENCY"));
		State::Set(AlphaMode, GetEnvironmentValue(16, envPrefix, "ALPHATEST"));
		State::Set(TextureMipMap, GetEnvironmentValue(1, envPrefix, "MIPMAP"));
		State::Set(ClearColor, GetEnvironmentValue(0, envPrefix, "BACKGROUNDCOLOUR"));
		State::Set(ChromaColor, GetEnvironmentValue(0, envPrefix, "CHROMACOLOUR"));
		State::Set(EnableDither, GetEnvironmentValue(1, envPrefix, "DITHER"));
		State::Set(EnableFog, 0);
		State::Set(FogMode, GetEnvironmentValue(0, envPrefix, "FOGMODE"));
		State::Set(FogDensity, GetEnvironmentValue(0, envPrefix, "FOGDENSITY"));
		State::Set(FogStart, GetEnvironmentValue(0, envPrefix, "STATE_FOGZNEAR"));
		State::Set(FogEnd, GetEnvironmentValue(0x3F800000, envPrefix, "FOGZFAR"));
		State::Set(FogColor, GetEnvironmentValue(0xFFFFFFFF, envPrefix, "FOGCOLOUR"));
		State::Set(BlendMode, GetEnvironmentValue(0, envPrefix, "ALPHA"));
		State::Set(TextureWrap, GetEnvironmentValue(0, envPrefix, "TEXTURECLAMP"));
		State::Set(Gamma, 0x3F800000);
		State::Set(DepthMode, GetEnvironmentValue(0, envPrefix, "DEPTHBIAS"));
		State::Set(LodBias, 0);
		State::Set(MaxPending, GetEnvironmentValue(maxPanding - 2 < 0 ? 0 : maxPanding - 2, envPrefix, "MAXPENDING")); // TODO
		State::Set(TexturesCombine, 0);
		State::Set(VertexLayout, 0);
		State::Set(Unknown_65577, 2);
		State::Set(BackBufferType, GetEnvironmentValue(1, envPrefix, "BACKBUFFERTYPE"));
		State::Set(FlatFans, GetEnvironmentValue(1, envPrefix, "FLATFANS"));
		State::Set(LineWidth, GetEnvironmentValue(1, envPrefix, "LINEWIDTH"));
		State::Set(EnableStencilBuffer, GetEnvironmentValue(0, envPrefix, "STENCILBUFFER"));
		State::Set(DisplayMode, GetEnvironmentValue(resolutionIndex, envPrefix, "DISPLAYMODE"));
		State::Set(LineDouble, GetEnvironmentValue(0, envPrefix, "LINEDOUBLE"));
		State::Set(BufferMode, 0);
		State::Set(ClearDepth, 0x3F800000);
		State::Set(FlipRate, GetEnvironmentValue(1, envPrefix, "FLIPRATE"));
		State::Set(ShamelessPlug, GetEnvironmentValue(0, envPrefix, "SHAMELESSPLUG"));
		State::Set(EnableDepthBuffer, GetEnvironmentValue(0, envPrefix, "DEPTHBUFFER"));
		State::Set(DepthCompare, GetEnvironmentValue(3, envPrefix, "DEPTHCMP"));

		return TRUE;
	}

	BOOL __stdcall Sync(DWORD a1)
	{
		Buffer::Draw();
		return TRUE;
	}
}