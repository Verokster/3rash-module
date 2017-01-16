#ifndef _THRASH_TYPE_TEXTURE_HPP_
#define _THRASH_TYPE_TEXTURE_HPP_

struct ThrashTexture
{
	DWORD id;
	DWORD width;
	DWORD height;
	DWORD level;
	ThrashColorFormatIndex colorFormatIndex;
	DWORD indexFormatIndex;
	DWORD wrap;
	DWORD unknown_2;
	ThrashTexture* previousTexture;
};

#endif