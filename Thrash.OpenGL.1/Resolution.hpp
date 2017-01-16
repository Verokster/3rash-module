#ifndef _THRASH_RESOLUTION_HPP_
#define _THRASH_RESOLUTION_HPP_

namespace Resolution
{
	DWORD __inline Add(DEVMODE* devMode);
	BOOL __fastcall LoadList();
	BOOL __stdcall Change(DWORD unknown, HWND hWnd, UINT msg, DWORD resolutionIndex, DWORD maxPanding, BOOL *result);
	BOOL __stdcall Restore(DWORD a1, HWND hWnd, UINT msg, DWORD resolutionIndex, DWORD maxPanding, BOOL *result);
}

#endif