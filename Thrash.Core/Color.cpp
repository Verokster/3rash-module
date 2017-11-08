#include "stdafx.h"
#include "Thrash.hpp"

namespace Color
{
	DWORD __fastcall Swap(DWORD color)
	{
		return color & 0xFF00FF00 | (color & 0x000000FF) << 16 | (color & 0x00FF0000) >> 16;
	}
}
