#pragma once

struct ThrashForced
{
	BOOL windowed;
	DWORD resolution;
	DWORD colorDepth;
	DWORD zdepth;
	DWORD refreshRate;
	BOOL exclusiveMode;
	BOOL vSync;
	BOOL aspect;
	FLOAT gamma;
	BOOL indexedVBO;
	DWORD filtering;
	BOOL add640x480x16;
	BOOL movies16Bit;
};