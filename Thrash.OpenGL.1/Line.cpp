#include "stdafx.h"
#include "Global.hpp"

namespace Line
{
	VOID __stdcall Draw(ThrashVertex* vertex1, ThrashVertex* vertex2)
	{
		GLBegin(GL_LINES);
		{
			Vertex::Draw(vertex1);
			Vertex::Draw(vertex2);
		}
		GLEnd();
	}

	VOID __stdcall DrawStrip(DWORD count, ThrashVertex vertexArray[])
	{
		Vertex::DrawArray(GL_LINE_STRIP, 1, count, vertexArray);
	}

	VOID __stdcall DrawStrip(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		Vertex::DrawIndexedArray(GL_LINE_STRIP, 1, count, vertexArray, indexes);
	}

	VOID __stdcall DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		Vertex::DrawIndexedArray(GL_LINES, 0, count * 2, vertexArray, indexes);
	}
}