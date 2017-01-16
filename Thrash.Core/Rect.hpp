#ifndef _THRASH_RECT_HPP_
#define _THRASH_RECT_HPP_

#include "stdafx.h"

namespace Rect
{
	BOOL __stdcall Read(ThrashRectangle rectangle, VOID* data);
	BOOL __stdcall Write(ThrashRectangle rectangle, VOID* data);
	BOOL __stdcall Write(ThrashRectangle rectangle, VOID* data, DWORD bytesPerRow);
}

#endif