#pragma once

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
#include "Window.hpp"

#define FLOAT_255 255.0f
#define FLOAT_65536 65536.0f
const double DEPTH_CORRECTION = 2.0 / FLOAT_65536;

extern HGLRC hGlRc;

extern DWORD appWindowed;
extern DWORD displayIndex;
extern DWORD resolutionsListCount;
extern ThrashResolution* selectedResolution;

extern ThrashDesktopMode desktopMode;
extern ThrashViewport viewport;
extern ThrashForced forced;

extern BOOL extendedVertex;
extern BOOL specularEnabled;

extern BOOL texturesEnabled;
extern DWORD textureMipMap;
extern BOOL textureFilterEnabled;
extern ThrashTexture* lastTexture;
extern ThrashTexture* bindedTexture;
extern FLOAT textureLodBias;

extern DWORD textureClampS;
extern DWORD textureClampT;
extern DWORD bufferModesArray[];

extern BOOL fogEnabled;

extern DWORD stencilFail;
extern DWORD stencilDepthFail;
extern DWORD stencilPass;

extern BOOL isWindowLocked;

extern DWORD depthCmp;
extern DWORD alphaCmp;
extern FLOAT alphaVal;

extern BOOL colorMask;
extern DWORD blendSrc;
extern DWORD blendDest;
extern BOOL stencilEnabled;
extern BOOL depthEnabled;
extern DWORD bufferMode;