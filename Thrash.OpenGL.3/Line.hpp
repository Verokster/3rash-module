#pragma once

namespace Line
{
	VOID __fastcall DrawStrip(DWORD count, ThrashVertex vertexArray[], WORD indexes[]);
	VOID __fastcall DrawMesh(DWORD count, ThrashVertex vertexArray[]);
	VOID __fastcall DrawMesh(DWORD count, ThrashVertex vertexArray[], WORD indexes[]);
}