/*
	MIT License

	Copyright (c) 2017 Oleksiy Ryabchun

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#pragma once

enum ThrashAboutSubType
{
	DeviceGeneric = 0,
	DeviceVoodooRush = 1,
	DeviceVoodoo2 = 2,
	DeviceVoodooBanshee = 3,
	DeviceVoodoo3 = 4,
};

typedef struct ThrashAbout
{
	CHAR signature[4];							// 0
	DWORD size;									// 4
	DWORD version;								// 8
	DWORD flags;								// 12
	DWORD textureWidthMin;						// 16
	DWORD textureWidthMax;						// 20
	DWORD textureWidthMultiple;					// 24
	DWORD textureHeightMin;						// 28
	DWORD textureHeightMax;						// 32
	DWORD textureHeightMultiple;				// 36

	DWORD clipAlign;							// 40
	DWORD texFormatsCount;						// 44
	ThrashTexColorFormats* texFormats;			// 48
	DWORD texIndexFormatsCount;					// 52
	ThrashTexColorIndexFormats* texIndexFormats;// 56
	DWORD resolutionsCount;						// 60
	ThrashResolution* resolutionsList;			// 64
	DWORD numStages;							// 68	- Multitexturing
	DWORD unknown;								// 72
	CHAR driverName[32];						// 76

	// WORD										// 88

	ThrashAboutSubType subType;					// 108

	DWORD textureRamSize;						// 112
	DWORD textureRamType;						// 116

	CHAR* author;								// 120
	DWORD dxVeriosn;							// 124
	CHAR deviceName[32];						// 128
} *LPTHRASHABOUT;