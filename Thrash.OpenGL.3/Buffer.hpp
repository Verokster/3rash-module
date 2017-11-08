#pragma once

namespace Buffer
{
	VOID __fastcall Create();
	VOID __fastcall Release();
	VOID __fastcall Draw();
	BOOL __fastcall Check(DWORD type);
	DWORD __fastcall AddVertex(ThrashVertex* vertex);
	VOID __fastcall AddIndex(DWORD index);
}