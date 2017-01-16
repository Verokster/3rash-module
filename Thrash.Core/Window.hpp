#ifndef _THRASH_WINDOW_HPP_
#define _THRASH_WINDOW_HPP_

#include "stdafx.h"

namespace Window
{
	VOID __stdcall Clear();
	VOID __stdcall Flush();
	BOOL __stdcall Window(DWORD bufferIndex);
	ThrashWindow* __stdcall Lock();
	BOOL __stdcall Unlock(ThrashWindow* window);
}

#endif