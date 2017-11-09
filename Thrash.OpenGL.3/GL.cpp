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

GLFLUSH GLFlush;
GLFINISH GLFinish;
GLSCISSOR GLScissor;
GLVIEWPORT GLViewport;
GLHINT GLHint;
GLCLEARDEPTH GLClearDepth;
GLDEPTHRANGE GLDepthRange;
GLCLEAR GLClear;
GLREADBUFFER GLReadBuffer;
GLDRAWBUFFER GLDrawBuffer;
GLREADPIXELS GLReadPixels;
GLENABLE GLEnable;
GLDISABLE GLDisable;
GLBINDTEXTURE GLBindTexture;
GLDELETETEXTURES GLDeleteTextures;
GLCLEARCOLOR GLClearColor;
GLDEPTHFUNC GLDepthFunc;
GLBLENDFUNC GLBlendFunc;
GLTEXPARAMETERI GLTexParameteri;
GLTEXIMAGE2D GLTexImage2D;
GLTEXSUBIMAGE2D GLTexSubImage2D;
GLDEPTHMASK GLDepthMask;
GLSTENCILFUNC GLStencilFunc;
GLSTENCILOP GLStencilOp;
GLACTIVETEXTURE GLActiveTexture;
GLGETINTEGERV GLGetIntegerv;

#ifdef _DEBUG
GLGETERROR GLGetError;
#endif

GLGENTEXTURES GLGenTextures;

GLGENBUFFERS GLGenBuffers;
GLDELETEBUFFERS GLDeleteBuffers;
GLBINDBUFFER GLBindBuffer;
GLBUFFERDATA GLBufferData;
GLBUFFERSUBDATA GLBufferSubData;
GLDRAWARRAYS GLDrawArrays;
GLDRAWELEMENTS GLDrawElements;
GLGENVERTEXARRAYS GLGenVertexArrays;
GLBINDVERTEXARRAY GLBindVertexArray;
GLDELETEVERTEXARRAYS GLDeleteVertexArrays;

GLENABLEVERTEXATTRIBARRAY GLEnableVertexAttribArray;
GLDISABLEVERTEXATTRIBARRAY GLDisableVertexAttribArray;
GLVERTEXATTRIBPOINTER GLVertexAttribPointer;
GLVERTEXATTRIBIPOINTER GLVertexAttribIPointer;

GLCREATESHADER GLCreateShader;
GLCREATEPROGRAM GLCreateProgram;
GLSHADERSOURCE GLShaderSource;
GLCOMPILESHADER GLCompileShader;
GLATTACHSHADER GLAttachShader;
GLLINKPROGRAM GLLinkProgram;
GLUSEPROGRAM GLUseProgram;
GLDELETEPROGRAM GLDeleteProgram;
GLGETSHADERIV GLGetShaderiv;
GLGETSHADERINFOLOG GLGetShaderInfoLog;

GLGETATTRIBLOCATION GLGetAttribLocation;
GLGETUNIFORMLOCATION GLGetUniformLocation;

GLUNIFORM1I GLUniform1i;
GLUNIFORM1UI GLUniform1ui;
GLUNIFORM1F GLUniform1f;
GLUNIFORM4F GLUniform4f;

GLUNIFORMMATRIX4FV GLUniformMatrix4fv;

HMODULE hModule;

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

VOID __fastcall CreateContextAttribs(HDC* devContext, HGLRC* glContext)
{
	CHAR buffer[256];

	LoadGLFunction(buffer, "wglCreateContextAttribs", (PROC*)&WGLCreateContextAttribs, "ARB");
	if (WGLCreateContextAttribs)
	{
		DWORD wglAttributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 0,
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
				Main::ShowError("Invalid version", __FILE__, "CreateContextAttribs", __LINE__);
			else if (errorCode == 0x2096)
				Main::ShowError("Invalid profile", __FILE__, "CreateContextAttribs", __LINE__);
		}
	}
	else
	{
		Main::ShowError("wglCreateContextAttribsARB error", __FILE__, "CreateContextAttribs", __LINE__);
	}

	LoadGLFunction(buffer, "wglSwapInterval", (PROC*)&WGLSwapInterval, "EXT");
	LoadGLFunction(buffer, "glFlush", (PROC*)&GLFlush);
	LoadGLFunction(buffer, "glFinish", (PROC*)&GLFinish);
	LoadGLFunction(buffer, "glScissor", (PROC*)&GLScissor);
	LoadGLFunction(buffer, "glViewport", (PROC*)&GLViewport);
	LoadGLFunction(buffer, "glHint", (PROC*)&GLHint);
	LoadGLFunction(buffer, "glClearDepth", (PROC*)&GLClearDepth);
	LoadGLFunction(buffer, "glDepthRange", (PROC*)&GLDepthRange);
	LoadGLFunction(buffer, "glClear", (PROC*)&GLClear);
	LoadGLFunction(buffer, "glReadBuffer", (PROC*)&GLReadBuffer);
	LoadGLFunction(buffer, "glDrawBuffer", (PROC*)&GLDrawBuffer);
	LoadGLFunction(buffer, "glReadPixels", (PROC*)&GLReadPixels);
	LoadGLFunction(buffer, "glEnable", (PROC*)&GLEnable);
	LoadGLFunction(buffer, "glDisable", (PROC*)&GLDisable);
	LoadGLFunction(buffer, "glBindTexture", (PROC*)&GLBindTexture);
	LoadGLFunction(buffer, "glDeleteTextures", (PROC*)&GLDeleteTextures);
	LoadGLFunction(buffer, "glClearColor", (PROC*)&GLClearColor);
	LoadGLFunction(buffer, "glDepthFunc", (PROC*)&GLDepthFunc);
	LoadGLFunction(buffer, "glBlendFunc", (PROC*)&GLBlendFunc);
	LoadGLFunction(buffer, "glTexParameteri", (PROC*)&GLTexParameteri);
	LoadGLFunction(buffer, "glTexImage2D", (PROC*)&GLTexImage2D);
	LoadGLFunction(buffer, "glTexSubImage2D", (PROC*)&GLTexSubImage2D);
	LoadGLFunction(buffer, "glDepthMask", (PROC*)&GLDepthMask);
	LoadGLFunction(buffer, "glStencilFunc", (PROC*)&GLStencilFunc);
	LoadGLFunction(buffer, "glStencilOp", (PROC*)&GLStencilOp);
	LoadGLFunction(buffer, "glActiveTexture", (PROC*)&GLActiveTexture);
	LoadGLFunction(buffer, "glGetIntegerv", (PROC*)&GLGetIntegerv);
	
	LoadGLFunction(buffer, "glGenTextures", (PROC*)&GLGenTextures);

#ifdef _DEBUG
	LoadGLFunction(buffer, "glGetError", (PROC*)&GLGetError);
#endif

	LoadGLFunction(buffer, "glGenBuffers", (PROC*)&GLGenBuffers);
	LoadGLFunction(buffer, "glDeleteBuffers", (PROC*)&GLDeleteBuffers);
	LoadGLFunction(buffer, "glBindBuffer", (PROC*)&GLBindBuffer);
	LoadGLFunction(buffer, "glBufferData", (PROC*)&GLBufferData);
	LoadGLFunction(buffer, "glBufferSubData", (PROC*)&GLBufferSubData);
	LoadGLFunction(buffer, "glDrawArrays", (PROC*)&GLDrawArrays);
	LoadGLFunction(buffer, "glDrawElements", (PROC*)&GLDrawElements);
	LoadGLFunction(buffer, "glGenVertexArrays", (PROC*)&GLGenVertexArrays);
	LoadGLFunction(buffer, "glBindVertexArray", (PROC*)&GLBindVertexArray);
	LoadGLFunction(buffer, "glDeleteVertexArrays", (PROC*)&GLDeleteVertexArrays);

	LoadGLFunction(buffer, "glEnableVertexAttribArray", (PROC*)&GLEnableVertexAttribArray);
	LoadGLFunction(buffer, "glDisableVertexAttribArray", (PROC*)&GLDisableVertexAttribArray);
	LoadGLFunction(buffer, "glVertexAttribPointer", (PROC*)&GLVertexAttribPointer);
	LoadGLFunction(buffer, "glVertexAttribIPointer", (PROC*)&GLVertexAttribIPointer);

	LoadGLFunction(buffer, "glCreateShader", (PROC*)&GLCreateShader);
	LoadGLFunction(buffer, "glCreateProgram", (PROC*)&GLCreateProgram);
	LoadGLFunction(buffer, "glShaderSource", (PROC*)&GLShaderSource);
	LoadGLFunction(buffer, "glCompileShader", (PROC*)&GLCompileShader);
	LoadGLFunction(buffer, "glAttachShader", (PROC*)&GLAttachShader);
	LoadGLFunction(buffer, "glLinkProgram", (PROC*)&GLLinkProgram);
	LoadGLFunction(buffer, "glUseProgram", (PROC*)&GLUseProgram);
	LoadGLFunction(buffer, "glDeleteProgram", (PROC*)&GLDeleteProgram);
	LoadGLFunction(buffer, "glGetShaderiv", (PROC*)&GLGetShaderiv);
	LoadGLFunction(buffer, "glGetShaderInfoLog", (PROC*)&GLGetShaderInfoLog);

	LoadGLFunction(buffer, "glGetAttribLocation", (PROC*)&GLGetAttribLocation);
	LoadGLFunction(buffer, "glGetUniformLocation", (PROC*)&GLGetUniformLocation);

	LoadGLFunction(buffer, "glUniform1i", (PROC*)&GLUniform1i);
	LoadGLFunction(buffer, "glUniform1ui", (PROC*)&GLUniform1ui);
	LoadGLFunction(buffer, "glUniform1f", (PROC*)&GLUniform1f);
	LoadGLFunction(buffer, "glUniform4f", (PROC*)&GLUniform4f);

	LoadGLFunction(buffer, "glUniformMatrix4fv", (PROC*)&GLUniformMatrix4fv);
}

#ifdef _DEBUG
VOID __fastcall CheckError(CHAR* file, CHAR* function, DWORD line)
{
	return;
	DWORD statusCode = GLGetError();

	CHAR* message;

	if (statusCode != GL_NO_ERROR)
	{
		switch (statusCode)
		{
			case GL_INVALID_ENUM:
				message = "GL_INVALID_ENUM";
				break;

			case GL_INVALID_VALUE:
				message = "GL_INVALID_VALUE";
				break;

			case GL_INVALID_OPERATION:
				message = "GL_INVALID_OPERATION";
				break;

			case GL_INVALID_FRAMEBUFFER_OPERATION:
				message = "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;

			case GL_OUT_OF_MEMORY:
				message = "GL_OUT_OF_MEMORY";
				break;

			case GL_STACK_UNDERFLOW:
				message = "GL_STACK_UNDERFLOW";
				break;

			case GL_STACK_OVERFLOW:
				message = "GL_STACK_OVERFLOW";
				break;

			default:
				message = "GL_UNDEFINED";
				break;
		}

		Main::ShowError(message, file, function, line);
	}
}
#endif