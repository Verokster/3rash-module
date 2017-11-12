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
#include "Global.hpp"

WORD gammaArray[3][256];
BOOL gammaSupported;

namespace GammaRamp
{
	VOID __fastcall Check()
	{
		if (!forced.windowed)
		{
			HWND dWnd = GetDesktopWindow();
			HDC dDc = GetDC(dWnd);
			gammaSupported = GetDeviceGammaRamp(dDc, gammaArray);
			ReleaseDC(dWnd, dDc);

			if (gammaSupported)
			{
				if ((HIBYTE(gammaArray[0][255]) <= HIBYTE(gammaArray[0][0])) ||
					(HIBYTE(gammaArray[1][255]) <= HIBYTE(gammaArray[1][0])) ||
					(HIBYTE(gammaArray[2][255]) <= HIBYTE(gammaArray[2][0])))
					gammaSupported = FALSE;
					// WARNING: device has broken gamma support, generated gamma.dat

				if ((HIBYTE(gammaArray[0][181]) == 255))
				{
					// WARNING: suspicious gamma tables, using linear ramp for restoration

					for (WORD i = 0; i < 3; ++i)
						for (WORD j = 0; j < 255; ++j)
							gammaArray[i][j] = j << 8;
				}
			}
		}
	}

	VOID __fastcall Set(FLOAT factor)
	{
		if (gammaSupported)
		{
			factor *= forced.gamma;

			DWORD i = 0;

			WORD table[3][256];
			for (i = 0; i < 3; ++i)
				for (DWORD j = 0; j < 256; ++j)
				{
					DWORD c = (DWORD)((FLOAT)gammaArray[i][j] * factor + 0.5);
					table[i][j] = c > 65535 ? 65535 : LOWORD(c);
				}

			// Win2K puts this odd restriction on gamma ramps...
			OSVERSIONINFO vinfo;
			vinfo.dwOSVersionInfoSize = sizeof(vinfo);
			GetVersionEx(&vinfo);

			if (vinfo.dwMajorVersion == 5 && vinfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
			{
				for (i = 0; i < 3; ++i)
				{
					for (WORD j = 0; j < 128; ++j)
						if (table[i][j] > ((j + 128) << 8))
							table[i][j] = (j + 128) << 8;

					if (table[i][127] > 254 << 8)
						table[i][127] = 254 << 8;
				}
			}

			// enforce constantly increasing
			for (i = 0; i < 3; ++i)
				for (WORD j = 1; j < 256; ++j)
					if (table[i][j] < table[i][j-1])
						table[i][j] = table[i][j-1];

			SetDeviceGammaRamp(hDc, table);
		}
	}

	VOID __fastcall Restore()
	{
		if (gammaSupported)
		{
			HWND dWnd = GetDesktopWindow();
			HDC dDc = GetDC(dWnd);
			SetDeviceGammaRamp(dDc, gammaArray);
			ReleaseDC(dWnd, dDc);
		}
	}

}