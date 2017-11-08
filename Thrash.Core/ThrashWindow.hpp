#pragma once

typedef struct ThrashWindow
{
	VOID* data;
	DWORD bytesPerRow;
	ThrashColorFormatIndex colorFormatIndex;
	DWORD width;
	DWORD height;
	DWORD buffer;
	DWORD unknown;
} *LPTHRASHWINDOW;