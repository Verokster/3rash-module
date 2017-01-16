#ifndef _THRASH_MAIN_HPP_
#define _THRASH_MAIN_HPP_

#include "stdafx.h"

namespace Main
{
	VOID __fastcall ShowError(CHAR* message, CHAR* file, CHAR* function, DWORD line);
	DWORD __fastcall GetEnvironmentValue(DWORD def, CHAR* prefix, CHAR* name);
	VOID __fastcall LoadEnvironmentValues(ThrashAbout* about, CHAR* prefix);
	// --------------- EXPORT --------------
	ThrashAbout* __stdcall About();
	BOOL __stdcall Clip(RECT rect);
	VOID __stdcall Idle();
	BOOL __stdcall Init();
	DWORD __stdcall Is();
	VOID __stdcall PageFlip();
	BOOL __stdcall Restore();
	BOOL __stdcall SelectDisplay(DWORD index);
	BOOL __stdcall SetVideoMode(DWORD resolutionIndex, DWORD maxPanding, BOOL isDepthBuffer16);
	BOOL __stdcall Sync(DWORD a1);
}

#endif