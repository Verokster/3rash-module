#include "stdafx.h"
#include "Global.hpp"

WORD gammaArray[3][256];
BOOL gammaSupported;

namespace GammaRamp
{
	VOID __fastcall Check()
	{
		if (!appWindowed && !(about.flags & About_Windowed))
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

					for (DWORD i = 0; i < 3; ++i)
						for (DWORD j = 0; j < 255; ++j)
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

			WORD table[3][256];
			for (DWORD i = 0; i < 3; ++i)
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
				for (DWORD i = 0; i < 3; ++i)
				{
					for (DWORD j = 0; j < 128; ++j)
						if (table[i][j] > ((j + 128) << 8))
							table[i][j] = (j + 128) << 8;

					if (table[i][127] > 254 << 8)
						table[i][127] = 254 << 8;
				}
			}

			// enforce constantly increasing
			for (DWORD i = 0; i < 3; ++i)
				for (DWORD j = 1; j < 256; ++j)
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