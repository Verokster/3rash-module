/*#ifdef THRASH_EXPORTS
#define THRASH_API __declspec(dllexport)
#else
#define THRASH_API __declspec(dllimport)
#endif*/

#ifndef _THRASH_HPP_
#define _THRASH_HPP_

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
#include "ThrashColorFormatIndex.hpp"
#include "ThrashTexture.hpp"
#include "ThrashWindow.hpp"
#include "ThrashFogMode.hpp"
#include "ThrashRectangle.hpp"
#include "ThrashState.hpp"

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

extern ThrashTexColorFormats textureFormats;
extern ThrashTexColorIndexFormats textureIndexFormats;
extern ThrashAbout about;
extern ThrashResolution resolutionsList[];

DWORD stateValueArray[];

extern VOID*(__stdcall *funcMalloc)(size_t);
extern VOID(__stdcall *funcFree)(VOID*);
extern DWORD(__stdcall *funcErrorMessageBox)(DWORD, LPCSTR);
extern HWND(__stdcall *funcGetHWND)();
extern VOID(__stdcall *funcResolution)(UINT, PROC);
extern DWORD(__stdcall *funcState)(DWORD, DWORD);

BOOL APIENTRY DllMain(HANDLE hModule, DWORD fdwReason, LPVOID lpReserved);

#endif