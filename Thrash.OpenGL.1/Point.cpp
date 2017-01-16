#include "stdafx.h"
#include "Global.hpp"

namespace Point
{
	VOID __stdcall Draw(ThrashVertex* vertex)
	{
		GLBegin(GL_POINTS);
		{
			Vertex::Draw(vertex);
		}
		GLEnd();
	}

	VOID __stdcall DrawStrip(DWORD count, ThrashVertex vertexArray[])
	{
		Vertex::DrawArray(GL_POINTS, 0, count, vertexArray);
	}

	VOID __stdcall DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		Vertex::DrawIndexedArray(GL_POINTS, 0, count, vertexArray, indexes);
	}
}