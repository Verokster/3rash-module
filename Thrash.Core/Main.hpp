#pragma once

namespace Main
{
	VOID __fastcall ShowError(CHAR* message, CHAR* file, CHAR* function, DWORD line);
	DWORD __fastcall GetEnvironmentValue(DWORD def, const CHAR* prefix, const CHAR* name);
	VOID __fastcall LoadEnvironmentValues(ThrashAbout* about, const CHAR* prefix);
	// --------------- EXPORT --------------

	LPTHRASHABOUT THRASHAPI About();
	BOOL THRASHAPI Clip(RECT rect);
	VOID THRASHAPI Idle();
	BOOL THRASHAPI Init();
	DWORD THRASHAPI Is();
	VOID THRASHAPI PageFlip();
	BOOL THRASHAPI Restore();
	BOOL THRASHAPI SelectDisplay(DWORD index);
	BOOL THRASHAPI SetVideoMode(DWORD resolutionIndex, DWORD maxPanding, BOOL isDepthBuffer16);
	BOOL THRASHAPI Sync(DWORD value);
}