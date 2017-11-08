#ifndef _THRASH_TYPE_TEXTURE_HPP_
#define _THRASH_TYPE_TEXTURE_HPP_

typedef struct ThrashTexture
{
	DWORD id;
	DWORD width;
	DWORD height;
	DWORD tmu;
	DWORD level;
	ThrashColorFormatIndex colorFormatIndex;
	DWORD indexFormatIndex;

	DWORD format;
	DWORD internalFormat;
	DWORD type;
	DWORD size;
	DWORD stride;
	DWORD pixels;

	BOOL reconvert;
	ThrashTexture* previousTexture;
	/*DWORD smallLOD;
	DWORD largeLOD;
	DWORD pallete;*/
} *LPTHRASHTEXTURE;

#endif