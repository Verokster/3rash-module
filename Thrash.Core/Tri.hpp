#pragma once

namespace Tri
{
	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3);
	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[]);
	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[], DWORD indexes[]);
	VOID THRASHAPI DrawFan(DWORD count, ThrashVertex vertexArray[]);
	VOID THRASHAPI DrawFan(DWORD count, ThrashVertex vertexArray[], DWORD indexes[]);
	VOID THRASHAPI DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[]);
}