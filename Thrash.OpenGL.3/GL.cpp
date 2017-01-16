#include "stdafx.h"
#include "Global.hpp"

WGLGETPROCADDRESS WGLGetProcAddress;
WGLMAKECURRENT WGLMakeCurrent;
WGLCREATECONTEXT WGLCreateContext;
WGLDELETECONTEXT WGLDeleteContext;
WGLSWAPBUFFERS WGLSwapBuffers;
WGLCREATECONTEXTATTRIBSARB WGLCreateContextAttribsARB;
WGLSWAPINTERVALEXT WGLSwapIntervalEXT;

GLSCISSOR GLScissor;
GLVIEWPORT GLViewport;
GLHINT GLHint;
GLCLEARDEPTH GLClearDepth;
GLDEPTHRANGE GLDepthRange;
GLCLEAR GLClear;
GLDRAWBUFFER GLDrawBuffer;
GLREADPIXELS GLReadPixels;
GLENABLE GLEnable;
GLDISABLE GLDisable;
GLBINDTEXTURE GLBindTexture;
GLDELETETEXTURES GLDeleteTextures;
GLCLEARCOLOR GLClearColor;
GLDEPTHFUNC GLDepthFunc;
GLALPHAFUNC GLAlphaFunc;
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

DWORD wglAttributesARB[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 0,
		//WGL_CONTEXT_FLAGS_ARB, 0,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
};

VOID __fastcall LoadGLFunction(LPCSTR name, PROC* func)
{
	*func = WGLGetProcAddress(name);
	if ((INT)*func >= -1 && (INT)*func <= 3)
		*func = GetProcAddress(hModule, name);
}

VOID __fastcall CreateContextAttribs(HDC* devContext, HGLRC* glContext)
{
	WGLSwapIntervalEXT = (WGLSWAPINTERVALEXT)WGLGetProcAddress("wglSwapIntervalEXT");
	WGLCreateContextAttribsARB = (WGLCREATECONTEXTATTRIBSARB)WGLGetProcAddress("wglCreateContextAttribsARB");
	if (WGLCreateContextAttribsARB != NULL)
	{
		HGLRC glHandler = WGLCreateContextAttribsARB(*devContext, NULL, wglAttributesARB);
		if (glHandler != NULL)
		{
			WGLMakeCurrent(*devContext, glHandler);
			WGLDeleteContext(*glContext);
			*glContext = glHandler;
		}
		else
		{
			DWORD errorCode = GetLastError();
			if (errorCode == 0x2095)
				Main::ShowError("Invalid version", __FILE__, "[OGL]", __LINE__);
			else if (errorCode == 0x2096)
				Main::ShowError("Invalid profile", __FILE__, "[OGL]", __LINE__);
		}
	}
	else
	{
		Main::ShowError("wglCreateContextAttribsARB error", __FILE__, "[OGL]", __LINE__);
	}

	LoadGLFunction("glScissor", (PROC*)&GLScissor);
	LoadGLFunction("glViewport", (PROC*)&GLViewport);
	LoadGLFunction("glHint", (PROC*)&GLHint);
	LoadGLFunction("glClearDepth", (PROC*)&GLClearDepth);
	LoadGLFunction("glDepthRange", (PROC*)&GLDepthRange);
	LoadGLFunction("glClear", (PROC*)&GLClear);
	LoadGLFunction("glDrawBuffer", (PROC*)&GLDrawBuffer);
	LoadGLFunction("glReadPixels", (PROC*)&GLReadPixels);
	LoadGLFunction("glEnable", (PROC*)&GLEnable);
	LoadGLFunction("glDisable", (PROC*)&GLDisable);
	LoadGLFunction("glBindTexture", (PROC*)&GLBindTexture);
	LoadGLFunction("glDeleteTextures", (PROC*)&GLDeleteTextures);
	LoadGLFunction("glClearColor", (PROC*)&GLClearColor);
	LoadGLFunction("glDepthFunc", (PROC*)&GLDepthFunc);
	LoadGLFunction("glAlphaFunc", (PROC*)&GLAlphaFunc);
	LoadGLFunction("glBlendFunc", (PROC*)&GLBlendFunc);
	LoadGLFunction("glTexParameteri", (PROC*)&GLTexParameteri);
	LoadGLFunction("glTexImage2D", (PROC*)&GLTexImage2D);
	LoadGLFunction("glTexSubImage2D", (PROC*)&GLTexSubImage2D);
	LoadGLFunction("glDepthMask", (PROC*)&GLDepthMask);
	LoadGLFunction("glStencilFunc", (PROC*)&GLStencilFunc);
	LoadGLFunction("glStencilOp", (PROC*)&GLStencilOp);
	LoadGLFunction("glActiveTexture", (PROC*)&GLActiveTexture);
	LoadGLFunction("glGetIntegerv", (PROC*)&GLGetIntegerv);
	
	LoadGLFunction("glGenTextures", (PROC*)&GLGenTextures);

#ifdef _DEBUG
	LoadGLFunction("glGetError", (PROC*)&GLGetError);
#endif

	LoadGLFunction("glGenBuffers", (PROC*)&GLGenBuffers);
	LoadGLFunction("glDeleteBuffers", (PROC*)&GLDeleteBuffers);
	LoadGLFunction("glBindBuffer", (PROC*)&GLBindBuffer);
	LoadGLFunction("glBufferData", (PROC*)&GLBufferData);
	LoadGLFunction("glBufferSubData", (PROC*)&GLBufferSubData);
	LoadGLFunction("glDrawArrays", (PROC*)&GLDrawArrays);
	LoadGLFunction("glDrawElements", (PROC*)&GLDrawElements);
	LoadGLFunction("glGenVertexArrays", (PROC*)&GLGenVertexArrays);
	LoadGLFunction("glBindVertexArray", (PROC*)&GLBindVertexArray);
	LoadGLFunction("glDeleteVertexArrays", (PROC*)&GLDeleteVertexArrays);

	LoadGLFunction("glEnableVertexAttribArray", (PROC*)&GLEnableVertexAttribArray);
	LoadGLFunction("glDisableVertexAttribArray", (PROC*)&GLDisableVertexAttribArray);
	LoadGLFunction("glVertexAttribPointer", (PROC*)&GLVertexAttribPointer);
	LoadGLFunction("glVertexAttribIPointer", (PROC*)&GLVertexAttribIPointer);

	LoadGLFunction("glCreateShader", (PROC*)&GLCreateShader);
	LoadGLFunction("glCreateProgram", (PROC*)&GLCreateProgram);
	LoadGLFunction("glShaderSource", (PROC*)&GLShaderSource);
	LoadGLFunction("glCompileShader", (PROC*)&GLCompileShader);
	LoadGLFunction("glAttachShader", (PROC*)&GLAttachShader);
	LoadGLFunction("glLinkProgram", (PROC*)&GLLinkProgram);
	LoadGLFunction("glUseProgram", (PROC*)&GLUseProgram);
	LoadGLFunction("glDeleteProgram", (PROC*)&GLDeleteProgram);
	LoadGLFunction("glGetShaderiv", (PROC*)&GLGetShaderiv);
	LoadGLFunction("glGetShaderInfoLog", (PROC*)&GLGetShaderInfoLog);

	LoadGLFunction("glGetAttribLocation", (PROC*)&GLGetAttribLocation);
	LoadGLFunction("glGetUniformLocation", (PROC*)&GLGetUniformLocation);

	LoadGLFunction("glUniform1i", (PROC*)&GLUniform1i);
	LoadGLFunction("glUniform1ui", (PROC*)&GLUniform1ui);
	LoadGLFunction("glUniform1f", (PROC*)&GLUniform1f);
	LoadGLFunction("glUniform4f", (PROC*)&GLUniform4f);

	LoadGLFunction("glUniformMatrix4fv", (PROC*)&GLUniformMatrix4fv);
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