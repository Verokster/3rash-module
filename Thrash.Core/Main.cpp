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

#include "stdafx.h"
#include "Thrash.hpp"

namespace Main
{
	VOID __fastcall ShowError(const CHAR* message, const CHAR* file, const CHAR* function, DWORD line)
	{
		if (functions.ErrorMessageBox)
		{
			CHAR dest[400];
			sprintf(dest, "%s:\n%s\n\n\nFILE %s\nLINE %d", function, message, file, line);
			functions.ErrorMessageBox(0, dest);
		}
	}

	DWORD __fastcall GetEnvironmentValue(DWORD def, const CHAR* prefix, const CHAR* name)
	{
		CHAR dest[80];
		CHAR destVal[12];
		CHAR* val = NULL;

		sprintf(dest, "%s_%s", prefix, name);
		if (GetEnvironmentVariable(dest, destVal, sizeof(destVal)))
			val = destVal;

		if (!val)
		{
			sprintf(dest, "THRASH_%s", name);
			if (GetEnvironmentVariable(dest, destVal, sizeof(destVal)))
				val = destVal;
		}

		return val ? atoi(val) : def;
	}

	VOID __fastcall LoadEnvironmentValues(ThrashAbout* about, const CHAR* prefix)
	{
		*(DWORD*)&about->signature = GetEnvironmentValue((DWORD)about->signature, prefix, "signature");
		about->version = GetEnvironmentValue(about->version, prefix, "version");

		about->flags = (about->flags & 0xFE) | GetEnvironmentValue(about->flags & 1, prefix, "linewidth");
		about->flags = (about->flags & 0xFD) | (GetEnvironmentValue(about->flags >> 1 & 1, prefix, "texturesquare") << 1);
		about->flags = (about->flags & 0xFB) | (GetEnvironmentValue(about->flags >> 2 & 1, prefix, "texturewidthpowerof2") << 2);
		about->flags = (about->flags & 0xF7) | (GetEnvironmentValue(about->flags >> 3 & 1, prefix, "textureheightpowerof2") << 3);
		about->flags = (about->flags & 0xEF) | (GetEnvironmentValue(about->flags >> 4 & 1, prefix, "software") << 4);
		about->flags = (about->flags & 0xDF) | (GetEnvironmentValue(about->flags >> 5 & 1, prefix, "windowed") << 5);
		about->flags = (about->flags & 0xBF) | (GetEnvironmentValue(about->flags >> 6 & 1, prefix, "globalclut") << 6);
		about->flags = (about->flags & 0x7F) | (GetEnvironmentValue(about->flags >> 7 & 1, prefix, "trilinear2pass") << 7);

		about->textureWidthMin = GetEnvironmentValue(about->textureWidthMin, prefix, "texturewidthmin");
		about->textureWidthMax = GetEnvironmentValue(about->textureWidthMax, prefix, "texturewidthmax");
		about->textureWidthMultiple = GetEnvironmentValue(about->textureWidthMultiple, prefix, "texturewidthmultiple");
		about->textureHeightMin = GetEnvironmentValue(about->textureHeightMin, prefix, "textureheightmin");
		about->textureHeightMax = GetEnvironmentValue(about->textureHeightMax, prefix, "textureheightmax");
		about->textureHeightMultiple = GetEnvironmentValue(about->textureHeightMultiple, prefix, "textureheightmultiple");
		about->clipAlign = GetEnvironmentValue(about->clipAlign, prefix, "clipalign");
		about->numStages = GetEnvironmentValue(about->numStages, prefix, "numstages");
		about->subType = (ThrashAboutSubType)GetEnvironmentValue((DWORD)about->subType, prefix, "subtype");
		about->textureRamSize = GetEnvironmentValue(about->textureRamSize, prefix, "textureramsize");
		about->textureRamType = GetEnvironmentValue(about->textureRamType, prefix, "textureramtype");
		about->dxVeriosn = GetEnvironmentValue(about->dxVeriosn, prefix, "dxversion");
	}
}