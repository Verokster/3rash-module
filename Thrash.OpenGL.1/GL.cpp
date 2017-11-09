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

WGLGETPROCADDRESS WGLGetProcAddress;
WGLMAKECURRENT WGLMakeCurrent;
WGLCREATECONTEXT WGLCreateContext;
WGLDELETECONTEXT WGLDeleteContext;
WGLSWAPBUFFERS WGLSwapBuffers;
WGLCREATECONTEXTATTRIBSARB WGLCreateContextAttribs;
WGLSWAPINTERVALEXT WGLSwapInterval;

GLGETSTRING GLGetString;
GLCOLOR4UBV GLColor4ubv;
GLSECONDARYCOLOR3UBV GLSecondaryColor3ubv;
GLVERTEX2F GLVertex2f;
GLVERTEX4F GLVertex4f;
GLTEXCOORD2F GLTexCoord2f;
GLTEXCOORD4F GLTexCoord4f;
GLBEGIN GLBegin;
GLEND GLEnd;
GLSCISSOR GLScissor;
GLVIEWPORT GLViewport;
GLHINT GLHint;
GLMATRIXMODE GLMatrixMode;
GLLOADIDENTITY GLLoadIdentity;
GLORTHO GLOrtho;
GLSCALEF GLScalef;
GLCLEARDEPTH GLClearDepth;
GLDEPTHRANGE GLDepthRange;
GLCLEAR GLClear;
GLFLUSH GLFlush;
GLFINISH GLFinish;
GLREADBUFFER GLReadBuffer;
GLDRAWBUFFER GLDrawBuffer;
GLREADPIXELS GLReadPixels;
GLENABLE GLEnable;
GLDISABLE GLDisable;
GLBINDTEXTURE GLBindTexture;
GLDELETETEXTURES GLDeleteTextures;
GLFRONTFACE GLFrontFace;
GLCLEARCOLOR GLClearColor;
GLSHADEMODEL GLShadeModel;
GLFOGF GLFogf;
GLFOGFV GLFogfv;
GLFOGI GLFogi;
GLFOGCOORDF GLFogCoordf;
GLDEPTHFUNC GLDepthFunc;
GLALPHAFUNC GLAlphaFunc;
GLBLENDFUNC GLBlendFunc;
GLTEXPARAMETERI GLTexParameteri;
GLTEXENVI GLTexEnvi;
GLTEXIMAGE2D GLTexImage2D;
GLTEXSUBIMAGE2D GLTexSubImage2D;
GLDEPTHMASK GLDepthMask;
GLSTENCILFUNC GLStencilFunc;
GLSTENCILOP GLStencilOp;
GLLINEWIDTH GLLineWidth;
GLGENTEXTURES GLGenTextures;
GLACTIVETEXTURE GLActiveTexture;
GLMULTITEXCOORD2F GLMultiTexCoord2f;
GLMULTITEXCOORD4F GLMultiTexCoord4f;
GLCOLORTABLE GLColorTable;
GLPOLYGONOFFSET GLPolygonOffset;

HMODULE hModule;

WORD glVersion;
BOOL glCapsBGR;
BOOL glCapsBGRA;
BOOL glCapsClampToEdge;
BOOL glCapsMirroredRepeat;

VOID __fastcall LoadGLFunction(CHAR* buffer, const CHAR* name, PROC* func, const CHAR* sufix = NULL)
{
	const CHAR* loadName;
	if (!sufix)
		loadName = name;
	else
	{
		strcpy(buffer, name);
		strcat(buffer, sufix);
		loadName = buffer;
	}

	if (WGLGetProcAddress)
		*func = WGLGetProcAddress(loadName);

	if ((INT)*func >= -1 && (INT)*func <= 3)
		*func = GetProcAddress(hModule, loadName);

	if (!sufix && !*func)
	{
		LoadGLFunction(buffer, name, func, "EXT");
		if (!*func)
			LoadGLFunction(buffer, name, func, "ARB");
	}
}

VOID CreateContextAttribs(HDC* devContext, HGLRC* glContext)
{
	CHAR buffer[256];
	
	LoadGLFunction(buffer, "wglCreateContextAttribs", (PROC*)&WGLCreateContextAttribs, "ARB");
	if (WGLCreateContextAttribs)
	{
		DWORD wglAttributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 1,
			WGL_CONTEXT_MINOR_VERSION_ARB, 4,
			//WGL_CONTEXT_FLAGS_ARB, 0,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		HGLRC glHandler = WGLCreateContextAttribs(*devContext, NULL, wglAttributes);
		if (glHandler)
		{
			WGLMakeCurrent(*devContext, glHandler);
			WGLDeleteContext(*glContext);
			*glContext = glHandler;
		}
		else
		{
			DWORD errorCode = GetLastError();
			if (errorCode == 0x2095)
				Main::ShowError("Invalid ARB version", __FILE__, "CreateContextAttribs", __LINE__);
			else if (errorCode == 0x2096)
				Main::ShowError("Invalid ARB profile", __FILE__, "CreateContextAttribs", __LINE__);
		}
	}

	LoadGLFunction(buffer, "wglSwapInterval", (PROC*)&WGLSwapInterval, "EXT");
	LoadGLFunction(buffer, "glGetString", (PROC*)&GLGetString);
	LoadGLFunction(buffer, "glColor4ubv", (PROC*)&GLColor4ubv);
	LoadGLFunction(buffer, "glSecondaryColor3ubv", (PROC*)&GLSecondaryColor3ubv);
	LoadGLFunction(buffer, "glTexCoord2f", (PROC*)&GLTexCoord2f);
	LoadGLFunction(buffer, "glTexCoord4f", (PROC*)&GLTexCoord4f);
	LoadGLFunction(buffer, "glVertex2f", (PROC*)&GLVertex2f);
	LoadGLFunction(buffer, "glVertex4f", (PROC*)&GLVertex4f);
	LoadGLFunction(buffer, "glBegin", (PROC*)&GLBegin);
	LoadGLFunction(buffer, "glEnd", (PROC*)&GLEnd);
	LoadGLFunction(buffer, "glScissor", (PROC*)&GLScissor);
	LoadGLFunction(buffer, "glViewport", (PROC*)&GLViewport);
	LoadGLFunction(buffer, "glHint", (PROC*)&GLHint);
	LoadGLFunction(buffer, "glMatrixMode", (PROC*)&GLMatrixMode);
	LoadGLFunction(buffer, "glLoadIdentity", (PROC*)&GLLoadIdentity);
	LoadGLFunction(buffer, "glOrtho", (PROC*)&GLOrtho);
	LoadGLFunction(buffer, "glScalef", (PROC*)&GLScalef);
	LoadGLFunction(buffer, "glClearDepth", (PROC*)&GLClearDepth);
	LoadGLFunction(buffer, "glDepthRange", (PROC*)&GLDepthRange);
	LoadGLFunction(buffer, "glClear", (PROC*)&GLClear);
	LoadGLFunction(buffer, "glFlush", (PROC*)&GLFlush);
	LoadGLFunction(buffer, "glFinish", (PROC*)&GLFinish);
	LoadGLFunction(buffer, "glReadBuffer", (PROC*)&GLReadBuffer);
	LoadGLFunction(buffer, "glDrawBuffer", (PROC*)&GLDrawBuffer);
	LoadGLFunction(buffer, "glReadPixels", (PROC*)&GLReadPixels);
	LoadGLFunction(buffer, "glEnable", (PROC*)&GLEnable);
	LoadGLFunction(buffer, "glDisable", (PROC*)&GLDisable);
	LoadGLFunction(buffer, "glBindTexture", (PROC*)&GLBindTexture);
	LoadGLFunction(buffer, "glDeleteTextures", (PROC*)&GLDeleteTextures);
	LoadGLFunction(buffer, "glFrontFace", (PROC*)&GLFrontFace);
	LoadGLFunction(buffer, "glClearColor", (PROC*)&GLClearColor);
	LoadGLFunction(buffer, "glShadeModel", (PROC*)&GLShadeModel);
	LoadGLFunction(buffer, "glFogf", (PROC*)&GLFogf);
	LoadGLFunction(buffer, "glFogfv", (PROC*)&GLFogfv);
	LoadGLFunction(buffer, "glFogi", (PROC*)&GLFogi);
	LoadGLFunction(buffer, "glFogCoordf", (PROC*)&GLFogCoordf);
	LoadGLFunction(buffer, "glDepthFunc", (PROC*)&GLDepthFunc);
	LoadGLFunction(buffer, "glAlphaFunc", (PROC*)&GLAlphaFunc);
	LoadGLFunction(buffer, "glBlendFunc", (PROC*)&GLBlendFunc);
	LoadGLFunction(buffer, "glTexParameteri", (PROC*)&GLTexParameteri);
	LoadGLFunction(buffer, "glTexEnvi", (PROC*)&GLTexEnvi);
	LoadGLFunction(buffer, "glTexImage2D", (PROC*)&GLTexImage2D);
	LoadGLFunction(buffer, "glTexSubImage2D", (PROC*)&GLTexSubImage2D);
	LoadGLFunction(buffer, "glDepthMask", (PROC*)&GLDepthMask);
	LoadGLFunction(buffer, "glStencilFunc", (PROC*)&GLStencilFunc);
	LoadGLFunction(buffer, "glStencilOp", (PROC*)&GLStencilOp);
	LoadGLFunction(buffer, "glLineWidth", (PROC*)&GLLineWidth);
	LoadGLFunction(buffer, "glGenTextures", (PROC*)&GLGenTextures);
	LoadGLFunction(buffer, "glActiveTexture", (PROC*)&GLActiveTexture);
	LoadGLFunction(buffer, "glMultiTexCoord2f", (PROC*)&GLMultiTexCoord2f);
	LoadGLFunction(buffer, "glMultiTexCoord4f", (PROC*)&GLMultiTexCoord4f);
	LoadGLFunction(buffer, "glColorTable", (PROC*)&GLColorTable, "EXT");
	LoadGLFunction(buffer, "glPolygonOffset", (PROC*)&GLPolygonOffset);

	glVersion = 0;
	WORD shiftVal = 8;
	const CHAR* strVer = (const CHAR*)GLGetString(GL_VERSION);
	WORD j = 0;
	while (TRUE)
	{
		if (strVer[j] <= '9' && strVer[j] >= '0')
		{
			glVersion += (strVer[j] - '0') << shiftVal;
			shiftVal -= 4;
		}
		else if (strVer[j] != '.')
			break;

		++j;
	}

	const GLubyte* extensions = GLGetString(GL_EXTENSIONS);

	if (glVersion < GL_VER_1_2)
		glCapsBGR = strstr((const CHAR*)extensions, "GL_EXT_bgra") != NULL;
	else
		glCapsBGR = TRUE;

	if (glVersion < GL_VER_1_2)
		glCapsBGRA = strstr((const CHAR*)extensions, "GL_EXT_bgra") != NULL || strstr((const CHAR*)extensions, "GL_EXT_texture_format_BGRA8888") != NULL || strstr((const CHAR*)extensions, "GL_APPLE_texture_format_BGRA8888") != NULL;
	else
		glCapsBGRA = TRUE;

	if (glVersion < GL_VER_1_2)
		glCapsClampToEdge = strstr((const CHAR*)extensions, "GL_EXT_texture_edge_clamp") != NULL || strstr((const CHAR*)extensions, "GL_SGIS_texture_edge_clamp") != NULL;
	else
		glCapsClampToEdge = TRUE;

	if (glVersion < GL_VER_1_4)
		glCapsMirroredRepeat = strstr((const CHAR*)extensions, "GL_ARB_texture_mirrored_repeat") != NULL || strstr((const CHAR*)extensions, "GL_IBM_texture_mirrored_repeat") != NULL;
	else
		glCapsMirroredRepeat = TRUE;
}