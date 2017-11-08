#pragma once

namespace Line
{
	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2);
	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[]);
	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[], DWORD indexes[]);
	VOID THRASHAPI DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[]);
}