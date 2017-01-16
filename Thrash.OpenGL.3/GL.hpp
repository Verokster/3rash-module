#ifndef _GL_HPP_
#define _GL_HPP_

#include <windows.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/wglew.h>

typedef PROC(__stdcall *WGLGETPROCADDRESS)(LPCSTR name);
typedef BOOL(__stdcall *WGLMAKECURRENT)(HDC devContext, HGLRC glContext);
typedef HGLRC(__stdcall *WGLCREATECONTEXT)(HDC devContext);
typedef BOOL(__stdcall *WGLDELETECONTEXT)(HGLRC glContext);
typedef BOOL(__stdcall *WGLSWAPBUFFERS)(HDC devContext);
typedef HGLRC(__stdcall *WGLCREATECONTEXTATTRIBSARB)(HDC hDC, HGLRC hshareContext, const DWORD *attribList);
typedef BOOL (__stdcall *WGLSWAPINTERVALEXT)(DWORD);

typedef void(__stdcall *GLSCISSOR)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void(__stdcall *GLVIEWPORT)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void(__stdcall *GLHINT)(GLenum target, GLenum mode);
typedef void(__stdcall *GLCLEARDEPTH)(GLclampd depth);
typedef void(__stdcall *GLDEPTHRANGE)(GLclampd, GLclampd);
typedef void(__stdcall *GLCLEAR)(GLbitfield mask);
typedef void(__stdcall *GLDRAWBUFFER)(GLenum mode);
typedef void(__stdcall *GLREADPIXELS)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
typedef void(__stdcall *GLENABLE)(GLenum cap);
typedef void(__stdcall *GLDISABLE)(GLenum cap);
typedef void(__stdcall *GLBINDTEXTURE)(GLenum target, GLuint texture);
typedef void(__stdcall *GLDELETETEXTURES)(GLsizei n, const GLuint *textures);
typedef void(__stdcall *GLCLEARCOLOR)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void(__stdcall *GLDEPTHFUNC)(GLenum func);
typedef void(__stdcall *GLALPHAFUNC)(GLenum func, GLclampf ref);
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
extern WGLCREATECONTEXTATTRIBSARB WGLCreateContextAttribsARB;
extern WGLSWAPINTERVALEXT WGLSwapIntervalEXT;

extern GLSCISSOR GLScissor;
extern GLVIEWPORT GLViewport;
extern GLHINT GLHint;
extern GLCLEARDEPTH GLClearDepth;
extern GLDEPTHRANGE GLDepthRange;
extern GLCLEAR GLClear;
extern GLDRAWBUFFER GLDrawBuffer;
extern GLREADPIXELS GLReadPixels;
extern GLENABLE GLEnable;
extern GLDISABLE GLDisable;
extern GLBINDTEXTURE GLBindTexture;
extern GLDELETETEXTURES GLDeleteTextures;
extern GLCLEARCOLOR GLClearColor;
extern GLDEPTHFUNC GLDepthFunc;
extern GLALPHAFUNC GLAlphaFunc;
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

#endif