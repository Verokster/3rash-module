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
//#include <GL\glew.h>
//#include <GL\wglew.h>
#include <GL\gl.h>

typedef BOOL(WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int* piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

#define GL_VER_1_1 0x0110
#define GL_VER_1_2 0x0120
#define GL_VER_1_3 0x0130
#define GL_VER_1_4 0x0140

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
#define WGL_CONTEXT_DEBUG_BIT_ARB               0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7

#define GL_FOG_COORDINATE_SOURCE 0x8450
#define GL_FOG_COORD_SRC GL_FOG_COORDINATE_SOURCE

#define GL_FOG_COORDINATE 0x8451
#define GL_FOG_COORD GL_FOG_COORDINATE

#define GL_TEXTURE_BASE_LEVEL 0x813C
#define GL_TEXTURE_MAX_LEVEL 0x813D

#define GL_BGR GL_BGR_EXT
#define GL_BGRA GL_BGRA_EXT
#define GL_RGB565 0x8D62
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#define GL_UNSIGNED_SHORT_4_4_4_4_REV 0x8365
#define GL_UNSIGNED_BYTE_4_4_MESA (GL_UNSIGNED_BYTE<<1)

#define GL_CLAMP_TO_EDGE 0x812F
#define GL_MIRRORED_REPEAT 0x8370

#define GL_TEXTURE_FILTER_CONTROL 0x8500
#define GL_TEXTURE_LOD_BIAS 0x8501

#define GL_COLOR_SUM 0x8458

#define GL_COMBINE 0x8570
#define GL_COMBINE_RGB 0x8571
#define GL_COMBINE_ALPHA 0x8572
#define GL_RGB_SCALE 0x8573
#define GL_SRC0_RGB 0x8580
#define GL_SRC1_RGB 0x8581
#define GL_SRC0_ALPHA 0x8588
#define GL_SRC1_ALPHA 0x8589
#define GL_PRIMARY_COLOR 0x8577
#define GL_OPERAND0_RGB 0x8590
#define GL_OPERAND1_RGB 0x8591
#define GL_OPERAND0_ALPHA 0x8598
#define GL_OPERAND1_ALPHA 0x8599
#define GL_PREVIOUS 0x8578
#define GL_INCR_WRAP 0x8507
#define GL_DECR_WRAP 0x8508
#define GL_SUBTRACT 0x84E7
#define GL_ADD_SIGNED 0x8574

typedef PROC(__stdcall *WGLGETPROCADDRESS)(LPCSTR name);
typedef BOOL(__stdcall *WGLMAKECURRENT)(HDC devContext, HGLRC glContext);
typedef HGLRC(__stdcall *WGLCREATECONTEXT)(HDC devContext);
typedef BOOL(__stdcall *WGLDELETECONTEXT)(HGLRC glContext);
typedef BOOL(__stdcall *WGLSWAPBUFFERS)(HDC devContext);
typedef HGLRC(__stdcall *WGLCREATECONTEXTATTRIBSARB)(HDC hDC, HGLRC hshareContext, const DWORD *attribList);
typedef BOOL (__stdcall *WGLSWAPINTERVALEXT)(DWORD);

typedef const GLubyte* (__stdcall *GLGETSTRING)(GLenum name);
typedef void(__stdcall *GLCOLOR4UBV)(const GLubyte *v);
typedef void(__stdcall *GLSECONDARYCOLOR3UBV)(const GLubyte *v);
typedef void(__stdcall *GLVERTEX2F)(GLfloat x, GLfloat y);
typedef void(__stdcall *GLVERTEX4F)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void(__stdcall *GLTEXCOORD2F)(GLfloat s, GLfloat t);
typedef void(__stdcall *GLTEXCOORD4F)(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void(__stdcall *GLBEGIN)(GLenum mode);
typedef void(__stdcall *GLEND)();
typedef void(__stdcall *GLSCISSOR)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void(__stdcall *GLVIEWPORT)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void(__stdcall *GLHINT)(GLenum target, GLenum mode);
typedef void(__stdcall *GLMATRIXMODE)(GLenum mode);
typedef void(__stdcall *GLLOADIDENTITY)();
typedef void(__stdcall *GLORTHO)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void(__stdcall *GLSCALEF)(GLfloat x, GLfloat y, GLfloat z);
typedef void(__stdcall *GLCLEARDEPTH)(GLclampd depth);
typedef void(__stdcall *GLDEPTHRANGE)(GLclampd, GLclampd);
typedef void(__stdcall *GLCLEAR)(GLbitfield mask);
typedef void(__stdcall *GLFLUSH)();
typedef void(__stdcall *GLFINISH)();
typedef void(__stdcall *GLREADBUFFER)(GLenum mode);
typedef void(__stdcall *GLDRAWBUFFER)(GLenum mode);
typedef void(__stdcall *GLREADPIXELS)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
typedef void(__stdcall *GLENABLE)(GLenum cap);
typedef void(__stdcall *GLDISABLE)(GLenum cap);
typedef void(__stdcall *GLBINDTEXTURE)(GLenum target, GLuint texture);
typedef void(__stdcall *GLDELETETEXTURES)(GLsizei n, const GLuint *textures);
typedef void(__stdcall *GLFRONTFACE)(GLenum mode);
typedef void(__stdcall *GLCLEARCOLOR)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void(__stdcall *GLSHADEMODEL)(GLenum mode);
typedef void(__stdcall *GLFOGF)(GLenum pname, GLfloat param);
typedef void(__stdcall *GLFOGFV)(GLenum pname, const GLfloat *params);
typedef void(__stdcall *GLFOGI)(GLenum pname, GLint param);
typedef void(__stdcall *GLFOGCOORDF)(GLfloat coord);
typedef void(__stdcall *GLDEPTHFUNC)(GLenum func);
typedef void(__stdcall *GLALPHAFUNC)(GLenum func, GLclampf ref);
typedef void(__stdcall *GLBLENDFUNC)(GLenum sfactor, GLenum dfactor);
typedef void(__stdcall *GLTEXPARAMETERI)(GLenum target, GLenum pname, GLint param);
typedef void(__stdcall *GLTEXENVI)(GLenum target, GLenum pname, GLint param);
typedef void(__stdcall *GLTEXENVF)(GLenum target, GLenum pname, GLfloat param);
typedef void(__stdcall *GLTEXIMAGE2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void(__stdcall *GLTEXSUBIMAGE2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
typedef void(__stdcall *GLDEPTHMASK)(GLboolean flag);
typedef void(__stdcall *GLSTENCILFUNC)(GLenum func, GLint ref, GLuint mask);
typedef void(__stdcall *GLSTENCILOP)(GLenum fail, GLenum zfail, GLenum zpass);
typedef void(__stdcall *GLLINEWIDTH)(GLfloat width);
typedef GLenum(__stdcall *GLGENTEXTURES)(GLsizei n, GLuint* textures);
typedef void(__stdcall *GLACTIVETEXTURE)(GLenum texture);
typedef void(__stdcall *GLMULTITEXCOORD2F)(GLenum target, GLfloat s, GLfloat t);
typedef void(__stdcall *GLMULTITEXCOORD4F)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void(__stdcall *GLCOLORTABLE)(GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const GLvoid *data);
typedef void(__stdcall *GLPOLYGONOFFSET)(GLfloat factor, GLfloat units);

extern WGLGETPROCADDRESS WGLGetProcAddress;
extern WGLMAKECURRENT WGLMakeCurrent;
extern WGLCREATECONTEXT WGLCreateContext;
extern WGLDELETECONTEXT WGLDeleteContext;
extern WGLSWAPBUFFERS WGLSwapBuffers;
extern WGLCREATECONTEXTATTRIBSARB WGLCreateContextAttribs;
extern WGLSWAPINTERVALEXT WGLSwapInterval;

extern GLGETSTRING GLGetString;
extern GLCOLOR4UBV GLColor4ubv;
extern GLSECONDARYCOLOR3UBV GLSecondaryColor3ubv;
extern GLVERTEX2F GLVertex2f;
extern GLVERTEX4F GLVertex4f;
extern GLTEXCOORD2F GLTexCoord2f;
extern GLTEXCOORD4F GLTexCoord4f;
extern GLBEGIN GLBegin;
extern GLEND GLEnd;
extern GLSCISSOR GLScissor;
extern GLVIEWPORT GLViewport;
extern GLHINT GLHint;
extern GLMATRIXMODE GLMatrixMode;
extern GLLOADIDENTITY GLLoadIdentity;
extern GLORTHO GLOrtho;
extern GLSCALEF GLScalef;
extern GLCLEARDEPTH GLClearDepth;
extern GLDEPTHRANGE GLDepthRange;
extern GLCLEAR GLClear;
extern GLFLUSH GLFlush;
extern GLFINISH GLFinish;
extern GLREADBUFFER GLReadBuffer;
extern GLDRAWBUFFER GLDrawBuffer;
extern GLREADPIXELS GLReadPixels;
extern GLENABLE GLEnable;
extern GLDISABLE GLDisable;
extern GLBINDTEXTURE GLBindTexture;
extern GLDELETETEXTURES GLDeleteTextures;
extern GLFRONTFACE GLFrontFace;
extern GLCLEARCOLOR GLClearColor;
extern GLSHADEMODEL GLShadeModel;
extern GLFOGF GLFogf;
extern GLFOGFV GLFogfv;
extern GLFOGI GLFogi;
extern GLFOGCOORDF GLFogCoordf;
extern GLDEPTHFUNC GLDepthFunc;
extern GLALPHAFUNC GLAlphaFunc;
extern GLBLENDFUNC GLBlendFunc;
extern GLTEXPARAMETERI GLTexParameteri;
extern GLTEXENVI GLTexEnvi;
extern GLTEXENVF GLTexEnvf;
extern GLTEXIMAGE2D GLTexImage2D;
extern GLTEXSUBIMAGE2D GLTexSubImage2D;
extern GLDEPTHMASK GLDepthMask;
extern GLSTENCILFUNC GLStencilFunc;
extern GLSTENCILOP GLStencilOp;
extern GLLINEWIDTH GLLineWidth;
extern GLGENTEXTURES GLGenTextures;
extern GLACTIVETEXTURE GLActiveTexture;
extern GLMULTITEXCOORD2F GLMultiTexCoord2f;
extern GLMULTITEXCOORD4F GLMultiTexCoord4f;
extern GLCOLORTABLE GLColorTable;
extern GLPOLYGONOFFSET GLPolygonOffset;

extern HMODULE hModule;

extern WORD glVersion;
extern BOOL glCapsBGR;
extern BOOL glCapsBGRA;
extern BOOL glCapsClampToEdge;
extern BOOL glCapsMirroredRepeat;
extern BOOL glCapsAnisotropic;

VOID CreateContextAttribs(HDC* devContext, HGLRC* glContext);