#include "stdafx.h"
#include "Global.hpp"

namespace Point
{
	VOID THRASHAPI Draw(ThrashVertex* vertex)
	{
		GLBegin(GL_POINTS);
		{
			if (!extendedVertex)
				Vertex::Draw((ThrashVertexV1*)vertex);
			else
				Vertex::Draw((ThrashVertexV2*)vertex);
		}
		GLEnd();
	}

	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[])
	{
		Vertex::DrawArray(GL_POINTS, 0, count, vertexArray);
	}

	VOID THRASHAPI DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		Vertex::DrawIndexedArray(GL_POINTS, 0, count, vertexArray, indexes);
	}
}