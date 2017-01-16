#ifndef _GL_HPP_
#define _GL_HPP_

#include <windows.h>
#include <GL\glew.h>
#include <GL\wglew.h>

typedef PROC(__stdcall *WGLGETPROCADDRESS)(LPCSTR name);
typedef BOOL(__stdcall *WGLMAKECURRENT)(HDC devContext, HGLRC glContext);
typedef HGLRC(__stdcall *WGLCREATECONTEXT)(HDC devContext);
typedef BOOL(__stdcall *WGLDELETECONTEXT)(HGLRC glContext);
typedef BOOL(__stdcall *WGLSWAPBUFFERS)(HDC devContext);
typedef HGLRC(__stdcall *WGLCREATECONTEXTATTRIBSARB)(HDC hDC, HGLRC hshareContext, const DWORD *attribList);
typedef BOOL (__stdcall *WGLSWAPINTERVALEXT)(DWORD);

typedef void(__stdcall *GLCOLOR4UBV)(const GLubyte *v);
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
typedef void(__stdcall *GLTEXPARAMETERI)(GLenum, GLenum, GLint);
typedef void(__stdcall *GLTEXENVI)(GLenum target, GLenum pname, GLint param);
typedef void(__stdcall *GLTEXIMAGE2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void(__stdcall *GLTEXSUBIMAGE2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void(__stdcall *GLDEPTHMASK)(GLboolean flag);
typedef void(__stdcall *GLSTENCILFUNC)(GLenum func, GLint ref, GLuint mask);
typedef void(__stdcall *GLSTENCILOP)(GLenum fail, GLenum zfail, GLenum zpass);
typedef void(__stdcall *GLLINEWIDTH)(GLfloat width); // glLineWidth

extern WGLGETPROCADDRESS WGLGetProcAddress;
extern WGLMAKECURRENT WGLMakeCurrent;
extern WGLCREATECONTEXT WGLCreateContext;
extern WGLDELETECONTEXT WGLDeleteContext;
extern WGLSWAPBUFFERS WGLSwapBuffers;
extern WGLCREATECONTEXTATTRIBSARB WGLCreateContextAttribsARB;
extern WGLSWAPINTERVALEXT WGLSwapIntervalEXT;

extern GLCOLOR4UBV GLColor4ubv;
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
extern GLTEXIMAGE2D GLTexImage2D;
extern GLTEXSUBIMAGE2D GLTexSubImage2D;
extern GLDEPTHMASK GLDepthMask;
extern GLSTENCILFUNC GLStencilFunc;
extern GLSTENCILOP GLStencilOp;
extern GLLINEWIDTH GLLineWidth;

extern HMODULE hModule;

VOID CreateContextAttribs(HDC* devContext, HGLRC* glContext);

#endif