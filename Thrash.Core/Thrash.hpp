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

//#define THRASHAPI __declspec(dllexport)  __stdcall
#define THRASHAPI __stdcall

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "stdafx.h"

#include "ThrashTexColorFormats.hpp"
#include "ThrashTexColorIndexFormats.hpp"
#include "ThrashList.hpp"
#include "ThrashResolution.hpp"
#include "ThrashAbout.hpp"
#include "ThrashAboutFlags.hpp"
#include "ThrashVertCoord.hpp"
#include "ThrashColor.hpp"
#include "ThrashTexCoord.hpp"
#include "ThrashVertex.hpp"
#include "ThrashPrimitive.hpp"
#include "ThrashColorFormatIndex.hpp"
#include "ThrashIndexFormatIndex.hpp"
#include "ThrashTexture.hpp"
#include "ThrashWindow.hpp"
#include "ThrashFogMode.hpp"
#include "ThrashRectangle.hpp"
#include "ThrashState.hpp"
#include "ThrashFunctions.hpp"

#include "Color.hpp"

#include "Main.hpp"
#include "Memory.hpp"
#include "State.hpp"
#include "Texture.hpp"
#include "Rect.hpp"
#include "Window.hpp"

#include "Point.hpp"
#include "Line.hpp"
#include "Tri.hpp"
#include "Quad.hpp"
#include "Sprite.hpp"

const DWORD MSG_CHANGE_RESOLUTION = 1124;
const DWORD MSG_RESET_RESOLUTION = 1125;

extern DWORD API_VERSION;
extern CHAR* envPrefix;

extern HDC hDc;
extern HWND hWnd;
extern HANDLE hChangeHandle;
extern HANDLE hRestoreHandle;
extern HINSTANCE hDllModule;
extern TCHAR iniFile[];

extern ThrashTexColorFormats textureFormats;
extern ThrashTexColorIndexFormats textureIndexFormats;
extern ThrashAbout about;
extern ThrashFunctionsExt functions;
extern ThrashResolution resolutionsList[];

const DWORD STATES_SIZE = 140;
extern DWORD stateValueArray[][STATES_SIZE];

BOOL APIENTRY DllMain(HANDLE hModule, DWORD fdwReason, LPVOID lpReserved);