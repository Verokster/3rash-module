#include "stdafx.h"
#include "Global.hpp"

ThrashResolution* selectedResolution;
ThrashDesktopMode desktopMode;

WORD forcedResolutionList[] =
{
	640, 480,
	800, 600,
	1024, 600,
	1024, 768,
	1152, 864,
	1280, 720,
	1280, 768,
	1280, 800,
	1280, 960,
	1280, 1024,
	1360, 768,
	1366, 768,
	1400, 1050,
	1440, 900,
	1440, 1080,
	1536, 864,
	1600, 900,
	1600, 1200,
	1680, 1050,
	1920, 1080,
	1920, 1200,
	1920, 1440,
	2048, 1536,
	2560, 1440,
	2560, 1600,
	3840, 2160,
	7680, 4320
};

BYTE forcedRefreshRateList[] = { 60, 70, 72, 75, 85, 90, 100, 120, 144 };

namespace Resolution
{
	DWORD __inline Add(DEVMODE* devMode)
	{
		ThrashResolution* resList = resolutionsList + 1;
		DWORD attempt = 0;

		for (DWORD i = 1; i < resolutionsListCount; ++i, ++resList, attempt = 0)
		{
			if (resList->width == 0)
			{
				resList->width = devMode->dmPelsWidth;
				resList->height = devMode->dmPelsHeight;
				resList->colorDepth = devMode->dmBitsPerPel;
				resList->r3 = 4;
				resList->r4 = 1;
				resList->r5 = 3;
				resList->r6 = 3;

				ThrashResolution* oldRes = resolutionsList + 1;
				for (DWORD j = 1; j < i; ++j, ++oldRes)
				{
					if (resList->colorDepth < oldRes->colorDepth ||
						resList->width < oldRes->width ||
						resList->height < oldRes->height)
					{
						DWORD temp = oldRes->width;
						oldRes->width = resList->width;
						resList->width = temp;

						temp = oldRes->height;
						oldRes->height = resList->height;
						resList->height = temp;

						temp = oldRes->colorDepth;
						oldRes->colorDepth = resList->colorDepth;
						resList->colorDepth = temp;

						break;
					}
				}

				return i;
			}

			if (resList->width == devMode->dmPelsWidth)
				++attempt;
			if (resList->height == devMode->dmPelsHeight)
				++attempt;
			if (resList->colorDepth == devMode->dmBitsPerPel)
				++attempt;
			if (attempt == 3)
				return i;
		}

		return 0;
	}

	BOOL __fastcall LoadList()
	{
		memset(resolutionsList, NULL, resolutionsListCount);

		if (forced.add640x480x16)
		{
			ThrashResolution* res = resolutionsList + 1;
			res->width = 640;
			res->height = 480;
			res->colorDepth = 16;
			res->r3 = 4;
			res->r4 = 1;
			res->r5 = 3;
			res->r6 = 3;
		}

		LPCSTR displayName = NULL;
		DISPLAY_DEVICE display;
		display.cb = sizeof(display);
		if (EnumDisplayDevices(NULL, displayIndex, &display, NULL))
			displayName = display.DeviceName;

		DEVMODE devMode;
		devMode.dmSize = sizeof(DEVMODE);

		EnumDisplaySettings(displayName, ENUM_REGISTRY_SETTINGS, &devMode);
		desktopMode.width = devMode.dmPelsWidth;
		desktopMode.height = devMode.dmPelsHeight;

		DWORD dbp = devMode.dmBitsPerPel;
		if (forced.windowed)
			forced.colorDepth = devMode.dmBitsPerPel;
		else
		{
			BOOL found = FALSE;
			DWORD maxAvailable = 0;
			memset(&devMode, NULL, sizeof(DEVMODE));
			for (DWORD i = 0; EnumDisplaySettings(displayName, i, &devMode); ++i)
			{
				if (devMode.dmPelsWidth >= 640 && devMode.dmPelsHeight >= 480)
				{
					if (devMode.dmBitsPerPel == forced.colorDepth)
					{
						found = TRUE;
						break;
					}
					else if (devMode.dmBitsPerPel > maxAvailable)
						maxAvailable = devMode.dmBitsPerPel;
				}

				memset(&devMode, NULL, sizeof(DEVMODE));
			}

			if (!found)
				forced.colorDepth = maxAvailable;
		}

		DWORD res = FALSE;

		memset(&devMode, NULL, sizeof(DEVMODE));
		for (DWORD i = 0; EnumDisplaySettings(displayName, i, &devMode); ++i)
		{
			if (devMode.dmPelsWidth >= 640 && devMode.dmPelsHeight >= 480 && devMode.dmBitsPerPel == forced.colorDepth)
			{
				if (Resolution::Add(&devMode))
					res = TRUE;
				else
					break;
			}

			memset(&devMode, NULL, sizeof(DEVMODE));
		}

		return res;
	}

	BOOL __stdcall Change(DWORD unknown, HWND newHWnd, UINT msg, DWORD resolutionIndex, DWORD maxPanding, BOOL *result)
	{
		selectedResolution = &resolutionsList[resolutionIndex];
		viewport.rectangle.width = viewport.width = selectedResolution->width;
		viewport.rectangle.height = viewport.height = selectedResolution->height;
		viewport.rectangle.x = viewport.rectangle.y = 0;
		viewport.clipFactor.x = viewport.clipFactor.y = viewport.viewFactor.x = viewport.viewFactor.y = 1.0;

		if (!appWindowed)
		{
			if (forced.resolution > 0)
			{
				if (forced.resolution == 1)
				{
					viewport.rectangle.width = viewport.width = desktopMode.width;
					viewport.rectangle.height = viewport.height = desktopMode.height;
				}
				else if (forced.resolution >= 2)
				{
					WORD* res = &forcedResolutionList[(forced.resolution - 2) << 1];
					viewport.rectangle.width = viewport.width = *res++;
					viewport.rectangle.height = viewport.height = *res;
				}

				viewport.clipFactor.x = viewport.viewFactor.x = (FLOAT)viewport.width / (FLOAT)selectedResolution->width;
				viewport.clipFactor.y = viewport.viewFactor.y = (FLOAT)viewport.height / (FLOAT)selectedResolution->height;

				if (forced.aspect)
				{
					if (viewport.viewFactor.x > viewport.viewFactor.y)
					{
						viewport.rectangle.width = Main::Round((FLOAT)selectedResolution->width * viewport.viewFactor.y);
						viewport.rectangle.x = Main::Round((viewport.width - viewport.rectangle.width) >> 1);
						viewport.clipFactor.x = (FLOAT)viewport.rectangle.width / (FLOAT)selectedResolution->width;
					}
					else
					{
						viewport.rectangle.height = Main::Round((FLOAT)selectedResolution->height * viewport.viewFactor.x);
						viewport.rectangle.y = Main::Round((viewport.height - viewport.rectangle.height) >> 1);
						viewport.clipFactor.y = (FLOAT)viewport.rectangle.height / (FLOAT)selectedResolution->height;
					}
				}
			}

			DISPLAY_DEVICE display;
			display.cb = sizeof(display);

			DEVMODE devMode;
			devMode.dmSize = sizeof(devMode);

			EnumDisplayDevices(NULL, displayIndex, &display, NULL);
			EnumDisplaySettings(display.DeviceName, ENUM_REGISTRY_SETTINGS, &devMode);

			RECT rect;
			LONG dwStyle;
			if (forced.windowed)
			{
				dwStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

				SetWindowLong(newHWnd, GWL_STYLE, dwStyle);
				SetWindowLong(newHWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME);

				rect.left = Main::Round((devMode.dmPelsWidth - viewport.width) >> 1);
				rect.top = Main::Round((devMode.dmPelsHeight - viewport.height) >> 1);
				rect.right = viewport.width + rect.left;
				rect.bottom = viewport.height + rect.top;
			}
			else
			{
				dwStyle = GetWindowLong(newHWnd, GWL_STYLE) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

				rect.left = 0;
				rect.top = forced.exclusiveMode ? 0 : -1;
				rect.right = viewport.width;
				rect.bottom = viewport.height;

				devMode.dmPelsWidth = viewport.width;
				devMode.dmPelsHeight = viewport.height;
				devMode.dmBitsPerPel = selectedResolution->colorDepth;
				devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

				if (forced.refreshRate > 0)
				{
					devMode.dmDisplayFrequency = forcedRefreshRateList[forced.refreshRate - 1];
					devMode.dmFields |= DM_DISPLAYFREQUENCY;
				}

				DWORD res = ChangeDisplaySettingsEx(display.DeviceName, &devMode, NULL, CDS_FULLSCREEN | CDS_TEST | CDS_RESET, NULL);
				if (res != DISP_CHANGE_SUCCESSFUL)
					Main::ShowError("Bad display mode", __FILE__, "Change", __LINE__);

				ChangeDisplaySettingsEx(display.DeviceName, &devMode, NULL, CDS_FULLSCREEN | CDS_RESET, NULL);
				EnumDisplaySettings(display.DeviceName, ENUM_CURRENT_SETTINGS, &devMode);
			}

			AdjustWindowRect(&rect, dwStyle, FALSE);
			SetWindowPos(newHWnd, NULL, rect.left + devMode.dmPosition.x, rect.top + devMode.dmPosition.y, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOACTIVATE);
		}

		if (result)
		{
			SetEvent(hChangeHandle);
			*result = TRUE;
		}
		return TRUE;
	}

	BOOL __stdcall Restore(DWORD a1, HWND hWnd, UINT msg, DWORD resolutionIndex, DWORD maxPanding, BOOL *result)
	{
		if (!forced.windowed)
			ChangeDisplaySettingsA(NULL, NULL);

		if (result)
		{
			SetEvent(hRestoreHandle);
			*result = TRUE;
		}
		return TRUE;
	}
}