#pragma once

struct ThrashFunctions
{
	VOID* uncnown_0;
	HWND(__stdcall *GetHWND)();
	VOID(__stdcall *Event)(UINT, PROC);
	VOID* uncnown_3;
	VOID* uncnown_4;
	VOID(__stdcall *LockStatus)(BOOL);
	DWORD(__stdcall *ErrorMessageBox)(DWORD, LPCSTR);
	VOID* uncnown_7;
};


struct ThrashFunctionsExt : ThrashFunctions
{
	VOID*(__stdcall *Malloc)(size_t);
	VOID(__stdcall *Free)(VOID*);
	DWORD(__stdcall *State)(DWORD, DWORD);
};