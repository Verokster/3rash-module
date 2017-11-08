#pragma once

#include "GL.hpp"
#include "Thrash.hpp"
#include "Resolution.hpp"
#include "ThrashBuffer.hpp"
#include "ThrashViewport.hpp"
#include "ThrashForced.hpp"
#include "Buffer.hpp"
#include "ThrashDesktopMode.hpp"
#include "Resource.h"
#include "Shaders.hpp"
#include "Context.hpp"
#include "Main.hpp"
#include "State.hpp"
#include "Point.hpp"
#include "Line.hpp"
#include "Tri.hpp"
#include "Texture.hpp"
#include "Window.hpp"

const FLOAT FLOAT_255 = 255;
const double DEPTH_CORRECTION = 0.000030517578;

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
extern FLOAT textureLodBias;
extern DWORD textureClampS;
extern DWORD textureClampT;

extern DWORD bufferModesArray[];

extern DWORD cullFace;

extern BOOL alphaEnabled;
extern DWORD alphaVal;
extern DWORD alphaCmp;

extern DWORD blendSrc;
extern DWORD blendDest;

extern BOOL fogEnabled;
extern DWORD fogMode;
extern DWORD fogColor;
extern DWORD fogStart;
extern DWORD fogEnd;
extern DWORD fogDensity;

extern DWORD gamma;
extern DWORD bufferMode;

extern BOOL isWindowLocked;

extern DWORD stencilFail;
extern DWORD stencilDepthFail;
extern DWORD stencilPass;
extern DWORD depthCmp;
extern FLOAT depthBias;

extern BOOL colorMask;
extern BOOL stencilEnabled;
extern BOOL depthEnabled;

extern GLint attrCoordsLoc;
extern GLint attrDiffuseLoc;
extern GLint attrSpecularLoc;
extern GLint attrTexCoordsLoc;

extern GLint uniMVPLoc;

extern GLuint uniTexEnabledLoc;
extern GLuint uniShadeModelLoc;
extern GLuint uniAlphaFuncLoc;
extern GLuint uniAlphaValLoc;
extern GLuint uniFogEnabledLoc;
extern GLuint uniFogModeLoc;
extern GLuint uniFogColorLoc;
extern GLuint uniFogStartLoc;
extern GLuint uniFogEndLoc;
extern GLuint uniFogDensityLoc;
extern GLuint uniSpecularEnabledLoc;
extern GLuint uniGammaLoc;