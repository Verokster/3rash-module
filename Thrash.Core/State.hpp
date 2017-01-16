#ifndef _THRASH_STATE_HPP_
#define _THRASH_STATE_HPP_

#include "stdafx.h"

namespace State
{
	DWORD __fastcall GetKeyIndex(DWORD key);
	DWORD __stdcall Get(ThrashState type);
	BOOL __stdcall Set(ThrashState type, DWORD value);
}

#endif