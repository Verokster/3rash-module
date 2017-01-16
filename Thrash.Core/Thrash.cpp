#include "stdafx.h"
#include "Thrash.hpp"

ThrashAbout about;

HDC hDc;
HWND hWnd;
HANDLE hChangeHandle;
HANDLE hRestoreHandle;
HINSTANCE hDllModule;

DWORD(__stdcall *funcErrorMessageBox)(DWORD, LPCSTR);
HWND(__stdcall *funcGetHWND)();
DWORD(__stdcall *funcState)(DWORD, DWORD);
VOID*(__stdcall *funcMalloc)(size_t);
VOID(__stdcall *funcFree)(VOID*);
VOID(__stdcall *funcResolution)(UINT, PROC);