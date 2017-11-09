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

#pragma once

#include <windows.h>
//#include <GL/glew.h>
//#include <GL/wglew.h>
#include <GL\gl.h>

#if !defined(_PTRDIFF_T_DEFINED) && !defined(_PTRDIFF_T_) && !defined(__MINGW64__)
#  ifdef _WIN64
typedef __int64 ptrdiff_t;
#  else
typedef _W64 int ptrdiff_t;
#  endif
#  define _PTRDIFF_T_DEFINED
#  define _PTRDIFF_T_
#endif

typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
typedef char GLchar;

typedef BOOL(WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int* piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_TYPE_RGBA_ARB 0x202B
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_FULL_ACCELERATION_ARB 0x2027

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001

#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506

#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7

#define GL_BGR GL_BGR_EXT
#define GL_BGRA GL_BGRA_EXT
#define GL_RGB565 0x8D62
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#define GL_UNSIGNED_SHORT_4_4_4_4_REV 0x8365
#define GL_UNSIGNED_BYTE_4_4_MESA (GL_UNSIGNED_BYTE<<1)

#define GL_CLAMP_TO_EDGE 0x812F
#define GL_MIRRORED_REPEAT 0x8370

#define GL_TEXTURE_LOD_BIAS 0x8501

#define GL_TEXTURE_BASE_LEVEL 0x813C
#define GL_TEXTURE_MAX_LEVEL 0x813D

#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STREAM_DRAW 0x88E0

#define GL_INCR_WRAP 0x8507
#define GL_DECR_WRAP 0x8508

#define GL_MULTISAMPLE 0x809D

typedef PROC(__stdcall *WGLGETPROCADDRESS)(LPCSTR name);
typedef BOOL(__stdcall *WGLMAKECURRENT)(HDC devContext, HGLRC glContext);
typedef HGLRC(__stdcall *WGLCREATECONTEXT)(HDC devContext);
typedef BOOL(__stdcall *WGLDELETECONTEXT)(HGLRC glContext);
typedef BOOL(__stdcall *WGLSWAPBUFFERS)(HDC devContext);
typedef HGLRC(__stdcall *WGLCREATECONTEXTATTRIBSARB)(HDC hDC, HGLRC hshareContext, const DWORD *attribList);
typedef BOOL(__stdcall *WGLSWAPINTERVALEXT)(DWORD);

typedef void(__stdcall *GLFLUSH)();
typedef void(__stdcall *GLFINISH)();
typedef void(__stdcall *GLSCISSOR)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void(__stdcall *GLVIEWPORT)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void(__stdcall *GLHINT)(GLenum target, GLenum mode);
typedef void(__stdcall *GLCLEARDEPTH)(GLclampd depth);
typedef void(__stdcall *GLDEPTHRANGE)(GLclampd, GLclampd);
typedef void(__stdcall *GLCLEAR)(GLbitfield mask);
typedef void(__stdcall *GLREADBUFFER)(GLenum mode);
typedef void(__stdcall *GLDRAWBUFFER)(GLenum mode);
typedef void(__stdcall *GLREADPIXELS)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
typedef void(__stdcall *GLENABLE)(GLenum cap);
typedef void(__stdcall *GLDISABLE)(GLenum cap);
typedef void(__stdcall *GLBINDTEXTURE)(GLenum target, GLuint texture);
typedef void(__stdcall *GLDELETETEXTURES)(GLsizei n, const GLuint *textures);
typedef void(__stdcall *GLCLEARCOLOR)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void(__stdcall *GLDEPTHFUNC)(GLenum func);
typedef void(__stdcall *GLBLENDFUNC)(GLenum sfactor, GLenum dfactor);
typedef void(__stdcall *GLTEXPARAMETERI)(GLenum, GLenum, GLint);
typedef void(__stdcall *GLTEXIMAGE2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void(__stdcall *GLTEXSUBIMAGE2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void(__stdcall *GLDEPTHMASK)(GLboolean flag);
typedef void(__stdcall *GLSTENCILFUNC)(GLenum func, GLint ref, GLuint mask);
typedef void(__stdcall *GLSTENCILOP)(GLenum fail, GLenum zfail, GLenum zpass);
typedef void(__stdcall *GLACTIVETEXTURE)(GLenum texture);
typedef void(__stdcall *GLGETINTEGERV)(GLenum pname, GLint* data);
typedef GLenum(__stdcall *GLGENTEXTURES)(GLsizei n, GLuint* textures);

#ifdef _DEBUG
typedef GLenum(__stdcall *GLGETERROR)();
#endif

typedef void(__stdcall *GLGENBUFFERS)(GLsizei n, GLuint* buffers);
typedef void(__stdcall *GLDELETEBUFFERS)(GLsizei n, const GLuint* buffers);
typedef void(__stdcall *GLBINDBUFFER)(GLenum target, GLuint buffer);
typedef void(__stdcall *GLBUFFERSUBDATA)(GLenum	target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
typedef void(__stdcall *GLBUFFERDATA)(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
typedef void(__stdcall *GLDRAWARRAYS)(GLenum mode, GLint first, GLsizei count);
typedef void(__stdcall *GLDRAWELEMENTS)(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
typedef void(__stdcall *GLGENVERTEXARRAYS)(GLsizei n, GLuint* arrays);
typedef void(__stdcall *GLBINDVERTEXARRAY)(	GLuint array);
typedef void(__stdcall *GLDELETEVERTEXARRAYS)(GLsizei n, const GLuint* arrays);

typedef void(__stdcall *GLENABLEVERTEXATTRIBARRAY)(GLuint index);
typedef void(__stdcall *GLDISABLEVERTEXATTRIBARRAY)(GLuint index);
typedef void(__stdcall *GLVERTEXATTRIBPOINTER)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
typedef void(__stdcall *GLVERTEXATTRIBIPOINTER)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);

typedef GLuint(__stdcall *GLCREATESHADER)(GLenum shaderType);
typedef GLuint(__stdcall *GLCREATEPROGRAM)(void);
typedef void(__stdcall *GLSHADERSOURCE)(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
typedef void(__stdcall *GLCOMPILESHADER)(GLuint shader);
typedef void(__stdcall *GLATTACHSHADER)(GLuint program, GLuint shader);
typedef void(__stdcall *GLLINKPROGRAM)(GLuint program);
typedef void(__stdcall *GLUSEPROGRAM)(GLuint program);
typedef void(__stdcall *GLDELETEPROGRAM)(GLuint program);
typedef void(__stdcall *GLGETSHADERIV)(	GLuint shader, GLenum pname, GLint* params);
typedef void(__stdcall *GLGETSHADERINFOLOG)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);

typedef GLuint(__stdcall *GLGETATTRIBLOCATION)(	GLuint program, const GLchar* name);
typedef GLuint(__stdcall *GLGETUNIFORMLOCATION)(GLuint program, const GLchar* name);

typedef void(__stdcall *GLUNIFORM1I)(GLint location, GLint v0);
typedef void(__stdcall *GLUNIFORM1UI)(GLint location, GLuint v0);
typedef void(__stdcall *GLUNIFORM1F)(GLint location, GLfloat v0);
typedef void(__stdcall *GLUNIFORM4F)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

typedef GLuint(__stdcall *GLUNIFORMMATRIX4FV)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);


extern WGLGETPROCADDRESS WGLGetProcAddress;
extern WGLMAKECURRENT WGLMakeCurrent;
extern WGLCREATECONTEXT WGLCreateContext;
extern WGLDELETECONTEXT WGLDeleteContext;
extern WGLSWAPBUFFERS WGLSwapBuffers;
extern WGLCREATECONTEXTATTRIBSARB WGLCreateContextAttribs;
extern WGLSWAPINTERVALEXT WGLSwapInterval;

extern GLFLUSH GLFlush;
extern GLFINISH GLFinish;
extern GLSCISSOR GLScissor;
extern GLVIEWPORT GLViewport;
extern GLHINT GLHint;
extern GLCLEARDEPTH GLClearDepth;
extern GLDEPTHRANGE GLDepthRange;
extern GLCLEAR GLClear;
extern GLREADBUFFER GLReadBuffer;
extern GLDRAWBUFFER GLDrawBuffer;
extern GLREADPIXELS GLReadPixels;
extern GLENABLE GLEnable;
extern GLDISABLE GLDisable;
extern GLBINDTEXTURE GLBindTexture;
extern GLDELETETEXTURES GLDeleteTextures;
extern GLCLEARCOLOR GLClearColor;
extern GLDEPTHFUNC GLDepthFunc;
extern GLBLENDFUNC GLBlendFunc;
extern GLTEXPARAMETERI GLTexParameteri;
extern GLTEXIMAGE2D GLTexImage2D;
extern GLTEXSUBIMAGE2D GLTexSubImage2D;
extern GLDEPTHMASK GLDepthMask;
extern GLSTENCILFUNC GLStencilFunc;
extern GLSTENCILOP GLStencilOp;
extern GLACTIVETEXTURE GLActiveTexture;
extern GLGETINTEGERV GLGetIntegerv;
extern GLGENTEXTURES GLGenTextures;

#ifdef _DEBUG
extern GLGETERROR GLGetError;
#endif

extern GLGENBUFFERS GLGenBuffers;
extern GLDELETEBUFFERS GLDeleteBuffers;
extern GLBINDBUFFER GLBindBuffer;
extern GLBUFFERDATA GLBufferData;
extern GLBUFFERSUBDATA GLBufferSubData;
extern GLDRAWARRAYS GLDrawArrays;
extern GLDRAWELEMENTS GLDrawElements;
extern GLGENVERTEXARRAYS GLGenVertexArrays;
extern GLBINDVERTEXARRAY GLBindVertexArray;
extern GLDELETEVERTEXARRAYS GLDeleteVertexArrays;

extern GLENABLEVERTEXATTRIBARRAY GLEnableVertexAttribArray;
extern GLDISABLEVERTEXATTRIBARRAY GLDisableVertexAttribArray;
extern GLVERTEXATTRIBPOINTER GLVertexAttribPointer;
extern GLVERTEXATTRIBIPOINTER GLVertexAttribIPointer;

extern GLCREATESHADER GLCreateShader;
extern GLCREATEPROGRAM GLCreateProgram;
extern GLSHADERSOURCE GLShaderSource;
extern GLCOMPILESHADER GLCompileShader;
extern GLATTACHSHADER GLAttachShader;
extern GLLINKPROGRAM GLLinkProgram;
extern GLUSEPROGRAM GLUseProgram;
extern GLDELETEPROGRAM GLDeleteProgram;
extern GLGETSHADERIV GLGetShaderiv;
extern GLGETSHADERINFOLOG GLGetShaderInfoLog;

extern GLGETATTRIBLOCATION GLGetAttribLocation;
extern GLGETUNIFORMLOCATION GLGetUniformLocation;

extern GLUNIFORM1I GLUniform1i;
extern GLUNIFORM1UI GLUniform1ui;
extern GLUNIFORM1F GLUniform1f;
extern GLUNIFORM4F GLUniform4f;

extern GLUNIFORMMATRIX4FV GLUniformMatrix4fv;

extern HMODULE hModule;

VOID __fastcall CreateContextAttribs(HDC* devContext, HGLRC* glContext);

#ifdef _DEBUG
VOID __fastcall CheckError(CHAR* file, CHAR* function, DWORD line);
#endif