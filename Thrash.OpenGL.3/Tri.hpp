#pragma once

namespace Tri
{
	BOOL __fastcall CheckCullFace(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3);

	VOID __fastcall DrawStrip(DWORD count, ThrashVertex vertexArray[], WORD indexes[]);
	VOID __fastcall DrawFan(DWORD count, ThrashVertex vertexArray[], WORD indexes[]);
	VOID __fastcall DrawMesh(DWORD count, ThrashVertex vertexArray[]);
	VOID __fastcall DrawMesh(DWORD count, ThrashVertex vertexArray[], WORD indexes[]);
}