#pragma once

namespace Point
{
	VOID THRASHAPI Draw(ThrashVertex* vertex);
	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[]);
	VOID THRASHAPI DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[]);
}