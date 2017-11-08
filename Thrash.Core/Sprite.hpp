#pragma once

namespace Sprite
{
	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2);
	VOID THRASHAPI DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[]);
}