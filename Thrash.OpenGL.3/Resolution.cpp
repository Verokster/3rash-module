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
#include "math.h"
#include "Windowsx.h"

ThrashResolution* selectedResolution;
ThrashDesktopMode desktopMode;

WORD forcedResolutionList[] = {
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

#define WM_XBUTTONDOWN 0x020B
#define WM_XBUTTONUP 0x020C
#define WM_XBUTTONDBLCLK 0x020D

namespace Resolution
{
	VOID __fastcall ChangeView(DWORD width, DWORD height)
	{
		viewport.refresh = TRUE;

		viewport.rectangle.x = viewport.rectangle.y = 0;
		viewport.point.x = viewport.point.y = 0.0f;

		viewport.rectangle.width = viewport.width = width;
		viewport.rectangle.height = viewport.height = height;

		viewport.clipFactor.x = viewport.viewFactor.x = (FLOAT)viewport.width / (FLOAT)selectedResolution->width;
		viewport.clipFactor.y = viewport.viewFactor.y = (FLOAT)viewport.height / (FLOAT)selectedResolution->height;

		if (forced.aspect && viewport.viewFactor.x != viewport.viewFactor.y)
		{
			if (viewport.viewFactor.x > viewport.viewFactor.y)
			{
				FLOAT fw = (FLOAT)selectedResolution->width * viewport.viewFactor.y;
				viewport.rectangle.width = (DWORD)MathRound(fw);

				viewport.point.x = ((FLOAT)viewport.width - fw) / 2.0f;
				viewport.rectangle.x = (DWORD)MathRound(viewport.point.x);

				viewport.clipFactor.x = viewport.viewFactor.y;
			}
			else
			{
				FLOAT fh = (FLOAT)selectedResolution->height * viewport.viewFactor.x;
				viewport.rectangle.height = (DWORD)MathRound(fh);

				viewport.point.y = ((FLOAT)viewport.height - fh) / 2.0f;
				viewport.rectangle.y = (DWORD)MathRound(viewport.point.y);

				viewport.clipFactor.y = viewport.viewFactor.x;
			}
		}
	}

	static WNDPROC OldWindowProc = NULL;
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_SYSCOMMAND:
			return DefWindowProc(hWnd, message, wParam, lParam);

		case WM_SIZE:
		{
			ChangeView(lParam & 0xFFFF, lParam >> 16);
			return CallWindowProc(OldWindowProc, hWnd, message, wParam, lParam);
		}

		case WM_KILLFOCUS:
		case WM_SETFOCUS:
		case WM_ACTIVATE:
		case WM_NCACTIVATE:
		case WM_ACTIVATEAPP:
			return DefWindowProc(hWnd, message, wParam, lParam);

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_XBUTTONDBLCLK:
		case WM_MOUSEMOVE:
		{
			INT xPos = GET_X_LPARAM(lParam);
			INT yPos = GET_Y_LPARAM(lParam);

			if (xPos < INT(viewport.rectangle.x))
				xPos = 0;
			else if (xPos >= INT(viewport.rectangle.x + viewport.rectangle.width))
				xPos = selectedResolution->width - 1;
			else
				xPos = (INT)MathRound((FLOAT)(xPos - viewport.rectangle.x) / viewport.clipFactor.x);

			if (yPos < INT(viewport.rectangle.y))
				yPos = 0;
			else if (yPos >= INT(viewport.rectangle.y + viewport.rectangle.height))
				yPos = selectedResolution->height - 1;
			else
				yPos = (INT)MathRound((FLOAT)(yPos - viewport.rectangle.y) / viewport.clipFactor.y);

			lParam = MAKELONG(xPos, yPos);
			return CallWindowProc(OldWindowProc, hWnd, message, wParam, lParam);
		}

		default:
			return CallWindowProc(OldWindowProc, hWnd, message, wParam, lParam);
			break;
		}

		return NULL;
	}

	LRESULT CALLBACK WindowWindowedProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_SIZE:
		{
			ChangeView(lParam & 0xFFFF, lParam >> 16);
			return CallWindowProc(OldWindowProc, hWnd, message, wParam, lParam);
		}

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_XBUTTONDBLCLK:
		case WM_MOUSEMOVE:
		{
			INT xPos = GET_X_LPARAM(lParam);
			INT yPos = GET_Y_LPARAM(lParam);

			if (xPos < INT(viewport.rectangle.x))
				xPos = 0;
			else if (xPos >= INT(viewport.rectangle.x + viewport.rectangle.width))
				xPos = selectedResolution->width - 1;
			else
				xPos = (INT)MathRound((FLOAT)(xPos - viewport.rectangle.x) / viewport.clipFactor.x);

			if (yPos < INT(viewport.rectangle.y))
				yPos = 0;
			else if (yPos >= INT(viewport.rectangle.y + viewport.rectangle.height))
				yPos = selectedResolution->height - 1;
			else
				yPos = (INT)MathRound((FLOAT)(yPos - viewport.rectangle.y) / viewport.clipFactor.y);

			lParam = MAKELONG(xPos, yPos);
			return CallWindowProc(OldWindowProc, hWnd, message, wParam, lParam);
		}

		default:
			return CallWindowProc(OldWindowProc, hWnd, message, wParam, lParam);
			break;
		}

		return NULL;
	}

	BOOL WINAPI EnumNamesFunc(HMODULE hModule, LPCTSTR lpType, LPTSTR lpName, LONG lParam)
	{
		HICON hIcon = (HICON)LoadIcon(hModule, lpName);
		if (hIcon)
		{
			SetClassLong((HWND)lParam, GCL_HICON, (LONG)hIcon);
			return FALSE;
		}

		return TRUE;
	}

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
					if (resList->colorDepth < oldRes->colorDepth || resList->width < oldRes->width || resList->height < oldRes->height)
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
		MemoryZero(resolutionsList, resolutionsListCount);

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
		MemoryZero(&display, sizeof(DISPLAY_DEVICE));
		display.cb = sizeof(DISPLAY_DEVICE);
		if (EnumDisplayDevices(NULL, displayIndex, &display, NULL))
			displayName = display.DeviceName;

		DEVMODE devMode;
		MemoryZero(&devMode, sizeof(DEVMODE));
		devMode.dmSize = sizeof(DEVMODE);

		EnumDisplaySettings(displayName, ENUM_REGISTRY_SETTINGS, &devMode);
		desktopMode.width = devMode.dmPelsWidth;
		desktopMode.height = devMode.dmPelsHeight;

		DWORD dbp = devMode.dmBitsPerPel;
		if (forced.windowed || appWindowed)
			forced.colorDepth = devMode.dmBitsPerPel;
		else
		{
			BOOL found = FALSE;
			DWORD maxAvailable = 0;

			MemoryZero(&devMode, sizeof(DEVMODE));
			devMode.dmSize = sizeof(DEVMODE);

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

				MemoryZero(&devMode, sizeof(DEVMODE));
				devMode.dmSize = sizeof(DEVMODE);
			}

			if (!found)
				forced.colorDepth = maxAvailable;
		}

		DWORD res = FALSE;

		MemoryZero(&devMode, sizeof(DEVMODE));
		devMode.dmSize = sizeof(DEVMODE);

		for (DWORD i = 0; EnumDisplaySettings(displayName, i, &devMode); ++i)
		{
			if (devMode.dmPelsWidth >= 640 && devMode.dmPelsHeight >= 480 && devMode.dmBitsPerPel == forced.colorDepth)
			{
				if (Resolution::Add(&devMode))
					res = TRUE;
				else
					break;
			}

			MemoryZero(&devMode, sizeof(DEVMODE));
			devMode.dmSize = sizeof(DEVMODE);
		}

		return res;
	}

	BOOL __stdcall Change(DWORD unknown, HWND hWnd, UINT msg, DWORD resolutionIndex, DWORD maxPanding, BOOL* result)
	{
		selectedResolution = &resolutionsList[resolutionIndex];

		RECT rect;
		if (!appWindowed)
		{
			switch (forced.resolution)
			{
			case 0:
				ChangeView(selectedResolution->width, selectedResolution->height);
				break;
			case 1:
				ChangeView(desktopMode.width, desktopMode.height);
				break;
			default:
			{
				WORD* res = &forcedResolutionList[(forced.resolution - 2) << 1];
				ChangeView(*res++, *res);
				break;
			}
			}

			LONG dwStyle;
			if (forced.windowed)
			{
				HMONITOR hMon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
				MONITORINFO mi = { sizeof(mi) };
				GetMonitorInfo(hMon, &mi);

				dwStyle = WS_CLIPCHILDREN | WS_VISIBLE | WS_OVERLAPPEDWINDOW;

				rect.left = mi.rcWork.left + (mi.rcWork.right - mi.rcWork.left - viewport.width) >> 1;
				rect.top = mi.rcWork.top + (mi.rcWork.bottom - mi.rcWork.top - viewport.height) >> 1;
				rect.right = rect.left + viewport.width;
				rect.bottom = rect.top + viewport.height;

				AdjustWindowRect(&rect, dwStyle, FALSE);

				if (rect.right - rect.left >= mi.rcWork.right - mi.rcWork.left || rect.bottom - rect.top >= mi.rcWork.bottom - mi.rcWork.top)
					dwStyle |= WS_MAXIMIZE;

				SetWindowLong(hWnd, GWL_STYLE, dwStyle);
				SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME);

				HICON hIcon = (HICON)GetClassLong(hWnd, GCL_HICON);
				if (!hIcon)
				{
					HINSTANCE hIntance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
					EnumResourceNames(hIntance, RT_GROUP_ICON, EnumNamesFunc, (LONG_PTR)hWnd);
				}
				else
				{
					if (!SendMessage(hWnd, WM_GETICON, ICON_BIG, NULL))
						SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

					if (!SendMessage(hWnd, WM_SETICON, ICON_SMALL, NULL))
						SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
				}

				if (dwStyle & WS_MAXIMIZE)
					SetWindowPos(hWnd, HWND_NOTOPMOST, mi.rcWork.left, mi.rcWork.top, mi.rcWork.right - mi.rcWork.left, mi.rcWork.bottom - mi.rcWork.top, NULL);

				WINDOWPLACEMENT plc;
				MemoryZero(&plc, sizeof(WINDOWPLACEMENT));
				plc.length = sizeof(WINDOWPLACEMENT);

				GetWindowPlacement(hWnd, &plc);
				plc.rcNormalPosition = rect;
				SetWindowPlacement(hWnd, &plc);

				GetClientRect(hWnd, &rect);
				ChangeView(rect.right, rect.bottom);

				if (!OldWindowProc)
					OldWindowProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
			}
			else
			{
				DISPLAY_DEVICE display;
				MemoryZero(&display, sizeof(DISPLAY_DEVICE));
				display.cb = sizeof(DISPLAY_DEVICE);

				DEVMODE devMode;
				MemoryZero(&devMode, sizeof(DEVMODE));
				devMode.dmSize = sizeof(DEVMODE);

				EnumDisplayDevices(NULL, displayIndex, &display, NULL);
				EnumDisplaySettings(display.DeviceName, ENUM_REGISTRY_SETTINGS, &devMode);

				dwStyle = GetWindowLong(hWnd, GWL_STYLE) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

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

				AdjustWindowRect(&rect, dwStyle, FALSE);
				SetWindowPos(hWnd, HWND_NOTOPMOST, rect.left + devMode.dmPosition.x, rect.top + devMode.dmPosition.y, rect.right - rect.left, rect.bottom - rect.top, NULL);
			}
		}
		else
		{
			GetClientRect(hWnd, &rect);
			ChangeView(rect.right - rect.left, rect.bottom - rect.top);

			if (!OldWindowProc)
				OldWindowProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WindowWindowedProc);
		}

		if (result)
		{
			SetEvent(hChangeHandle);
			*result = TRUE;
		}
		return TRUE;
	}

	BOOL __stdcall Restore(DWORD a1, HWND hWnd, UINT msg, DWORD resolutionIndex, DWORD maxPanding, BOOL* result)
	{
		if (!forced.windowed && !appWindowed)
			ChangeDisplaySettings(NULL, NULL);

		if (result)
		{
			SetEvent(hRestoreHandle);
			*result = TRUE;
		}
		return TRUE;
	}
}