#ifndef _THRASH_TEXTURE_HPP_
#define _THRASH_TEXTURE_HPP_

#include "stdafx.h"

namespace Texture
{
	ThrashTexture* __stdcall Allocate(DWORD width, DWORD height, ThrashColorFormatIndex colorFormatIndex, DWORD indexFormatIndex, DWORD level);
	BOOL __stdcall Set(ThrashTexture* texture);
	ThrashTexture* __stdcall Update(ThrashTexture* texture, VOID* memory, BYTE* pallete);
	BOOL __stdcall Free(ThrashTexture* texture);
	BOOL __stdcall Reset();
}

#endif