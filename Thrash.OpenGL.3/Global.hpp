#ifndef _THRASH_GLOBAL_HPP_
#define _THRASH_GLOBAL_HPP_

#include "GL.hpp"
#include "Thrash.hpp"
#include "Resolution.hpp"
#include "ThrashBufferVertex.hpp"
#include "ThrashBuffer.hpp"
#include "ThrashViewport.hpp"
#include "ThrashForced.hpp"
#include "Buffer.hpp"
#include "ThrashDesktopMode.hpp"
#include "ThrashBindedTextureId.hpp"
#include "Resource.h"
#include "Shaders.hpp"
#include "Context.hpp"
#include "Main.hpp"
#include "Tri.hpp"
#include "Texture.hpp"

const FLOAT FLOAT_255 = 255;

extern HGLRC hGlRc;

extern DWORD appWindowed;
extern DWORD displayIndex;
extern DWORD resolutionsListCount;
extern ThrashResolution* selectedResolution;

extern ThrashDesktopMode desktopMode;

extern ThrashViewport viewport;

extern ThrashForced forced;

extern BOOL texturesEnabled;
extern DWORD textureMipMap;
extern BOOL textureFilterEnabled;

extern DWORD textureWrap;
extern DWORD bufferModesArray[];

extern BOOL shadeModel;
extern DWORD cullFace;

extern BOOL alphaEnabled;
extern DWORD alphaFunc;

extern BOOL fogEnabled;
extern DWORD fogMode;
extern DWORD fogColor;
extern DWORD fogStart;
extern DWORD fogEnd;
extern DWORD fogDensity;

extern DWORD gamma;

extern BOOL isWindowLocked;

extern DWORD stencilFail;
extern DWORD stencilDepthFail;
extern DWORD stencilPass;
extern DWORD depthCmp;

extern GLint attrCoordsLoc;
extern GLint attrColorLoc;
extern GLint attrTexCoordsLoc;
extern GLint attrTexUnitLoc;

extern GLint uniMVPLoc;

extern GLint uniTexEnabledLoc;
extern GLuint uniShadeModelLoc;
extern GLuint uniAlphaEnabledLoc;
extern GLuint uniAlphaFuncLoc;
extern GLuint uniFogModeLoc;
extern GLuint uniFogColorLoc;
extern GLuint uniFogStartLoc;
extern GLuint uniFogEndLoc;
extern GLuint uniFogDensityLoc;
extern GLuint uniGammaLoc;

extern DWORD currTextureUnit;
extern DWORD drawTextureUnit;

#endif