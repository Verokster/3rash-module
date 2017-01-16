#ifndef _THRASH_TYPE_WINDOW_HPP_
#define _THRASH_TYPE_WINDOW_HPP_

struct ThrashWindow
{
	VOID* data;
	DWORD bytesPerRow;
	ThrashColorFormatIndex colorFormatIndex;
	DWORD width;
	DWORD height;
};

#endif