#include "stdafx.h"
#include "Global.hpp"

WGLGETPROCADDRESS WGLGetProcAddress;
WGLMAKECURRENT WGLMakeCurrent;
WGLCREATECONTEXT WGLCreateContext;
WGLDELETECONTEXT WGLDeleteContext;
WGLSWAPBUFFERS WGLSwapBuffers;
WGLCREATECONTEXTATTRIBSARB WGLCreateContextAttribsARB;
WGLSWAPINTERVALEXT WGLSwapIntervalEXT;

GLCOLOR4UBV GLColor4ubv;
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

HMODULE hModule;

DWORD wglAttributesARB[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 1,
		WGL_CONTEXT_MINOR_VERSION_ARB, 4,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
};

VOID __fastcall LoadGLFunction(LPCSTR name, PROC* func)
{
	*func = WGLGetProcAddress(name);
	if ((INT)*func >= -1 && (INT)*func <= 3)
		*func = GetProcAddress(hModule, name);
}

VOID CreateContextAttribs(HDC* devContext, HGLRC* glContext)
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
				Main::ShowError("Invalid version ARB", __FILE__, "[OGL]", __LINE__);
			else if (errorCode == 0x2096)
				Main::ShowError("Invalid profile ARB", __FILE__, "[OGL]", __LINE__);
		}
	}

	LoadGLFunction("glColor4ubv", (PROC*)&GLColor4ubv);
	LoadGLFunction("glTexCoord2f", (PROC*)&GLTexCoord2f);
	LoadGLFunction("glTexCoord4f", (PROC*)&GLTexCoord4f);
	LoadGLFunction("glVertex2f", (PROC*)&GLVertex2f);
	LoadGLFunction("glVertex4f", (PROC*)&GLVertex4f);
	LoadGLFunction("glBegin", (PROC*)&GLBegin);
	LoadGLFunction("glEnd", (PROC*)&GLEnd);
	LoadGLFunction("glScissor", (PROC*)&GLScissor);
	LoadGLFunction("glViewport", (PROC*)&GLViewport);
	LoadGLFunction("glHint", (PROC*)&GLHint);
	LoadGLFunction("glMatrixMode", (PROC*)&GLMatrixMode);
	LoadGLFunction("glLoadIdentity", (PROC*)&GLLoadIdentity);
	LoadGLFunction("glOrtho", (PROC*)&GLOrtho);
	LoadGLFunction("glScalef", (PROC*)&GLScalef);
	LoadGLFunction("glClearDepth", (PROC*)&GLClearDepth);
	LoadGLFunction("glDepthRange", (PROC*)&GLDepthRange);
	LoadGLFunction("glClear", (PROC*)&GLClear);
	LoadGLFunction("glFlush", (PROC*)&GLFlush);
	LoadGLFunction("glFinish", (PROC*)&GLFinish);
	LoadGLFunction("glDrawBuffer", (PROC*)&GLDrawBuffer);
	LoadGLFunction("glReadPixels", (PROC*)&GLReadPixels);
	LoadGLFunction("glEnable", (PROC*)&GLEnable);
	LoadGLFunction("glDisable", (PROC*)&GLDisable);
	LoadGLFunction("glBindTexture", (PROC*)&GLBindTexture);
	LoadGLFunction("glDeleteTextures", (PROC*)&GLDeleteTextures);
	LoadGLFunction("glFrontFace", (PROC*)&GLFrontFace);
	LoadGLFunction("glClearColor", (PROC*)&GLClearColor);
	LoadGLFunction("glShadeModel", (PROC*)&GLShadeModel);
	LoadGLFunction("glFogf", (PROC*)&GLFogf);
	LoadGLFunction("glFogfv", (PROC*)&GLFogfv);
	LoadGLFunction("glFogi", (PROC*)&GLFogi);
	LoadGLFunction("glFogCoordf", (PROC*)&GLFogCoordf);
	LoadGLFunction("glDepthFunc", (PROC*)&GLDepthFunc);
	LoadGLFunction("glAlphaFunc", (PROC*)&GLAlphaFunc);
	LoadGLFunction("glBlendFunc", (PROC*)&GLBlendFunc);
	LoadGLFunction("glTexParameteri", (PROC*)&GLTexParameteri);
	LoadGLFunction("glTexEnvi", (PROC*)&GLTexEnvi);
	LoadGLFunction("glTexImage2D", (PROC*)&GLTexImage2D);
	LoadGLFunction("glTexSubImage2D", (PROC*)&GLTexSubImage2D);
	LoadGLFunction("glDepthMask", (PROC*)&GLDepthMask);
	LoadGLFunction("glStencilFunc", (PROC*)&GLStencilFunc);
	LoadGLFunction("glStencilOp", (PROC*)&GLStencilOp);
	LoadGLFunction("glLineWidth", (PROC*)&GLLineWidth);
}