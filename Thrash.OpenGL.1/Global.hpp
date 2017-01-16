#ifndef _THRASH_GLOBAL_HPP_
#define _THRASH_GLOBAL_HPP_

#include "GL.hpp"
#include "Thrash.hpp"
#include "Resolution.hpp"
#include "GammaRamp.hpp"
#include "ThrashForced.hpp"
#include "ThrashViewport.hpp"
#include "Vertex.hpp"
#include "ThrashDesktopMode.hpp"
#include "Resource.h"
#include "Context.hpp"
#include "Main.hpp"
#include "Texture.hpp"

const FLOAT FLOAT_255 = 255;

extern HGLRC hGlRc;

extern DWORD appWindowed;
extern DWORD displayIndex;
extern DWORD resolutionsListCount;
extern ThrashResolution* selectedResolution;

extern ThrashDesktopMode desktopMode;

//extern ThrashResolution viewportResolution;
extern ThrashViewport viewport;

extern ThrashForced forced;

extern BOOL texturesEnabled;
extern DWORD textureMipMap;
extern BOOL textureFilterEnabled;
extern ThrashTexture* lastTexture;
extern ThrashTexture* bindedTexture;

extern DWORD textureWrap;
extern DWORD bufferModesArray[];

extern DWORD cullFaceMode;

extern BOOL fogEnabled;
extern DWORD fogMode;
extern DWORD fogColor;

extern BOOL isWindowLocked;

extern DWORD stencilFail;
extern DWORD stencilDepthFail;
extern DWORD stencilPass;
extern DWORD depthCmp;

#endif