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