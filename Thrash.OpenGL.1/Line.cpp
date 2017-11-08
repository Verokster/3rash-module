#include "stdafx.h"
#include "Global.hpp"

namespace Line
{
	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2)
	{
		GLBegin(GL_LINES);
		{
			if (!extendedVertex)
			{
				Vertex::Draw((ThrashVertexV1*)vertex1);
				Vertex::Draw((ThrashVertexV1*)vertex2);
			}
			else
			{
				Vertex::Draw((ThrashVertexV2*)vertex1);
				Vertex::Draw((ThrashVertexV2*)vertex2);
			}
		}
		GLEnd();
	}

	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[])
	{
		Vertex::DrawArray(GL_LINE_STRIP, 1, count, vertexArray);
	}

	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		Vertex::DrawIndexedArray(GL_LINE_STRIP, 1, count, vertexArray, indexes);
	}

	VOID THRASHAPI DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		Vertex::DrawIndexedArray(GL_LINES, 0, count * 2, vertexArray, indexes);
	}
}