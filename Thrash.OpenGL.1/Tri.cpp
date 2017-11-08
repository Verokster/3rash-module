#include "stdafx.h"
#include "Global.hpp"

namespace Tri
{
	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3)
	{
		Texture::CheckWrap();
		GLBegin(GL_TRIANGLES);
		{
			if (!extendedVertex)
			{
				Vertex::Draw((ThrashVertexV1*)vertex1);
				Vertex::Draw((ThrashVertexV1*)vertex2);
				Vertex::Draw((ThrashVertexV1*)vertex3);
			}
			else
			{
				Vertex::Draw((ThrashVertexV2*)vertex1);
				Vertex::Draw((ThrashVertexV2*)vertex2);
				Vertex::Draw((ThrashVertexV2*)vertex3);
			}
		}
		GLEnd();
	}

	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[])
	{
		Vertex::DrawArray(GL_TRIANGLE_STRIP, 2, count, vertexArray);
	}

	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		Vertex::DrawIndexedArray(GL_TRIANGLE_STRIP, 2, count, vertexArray, indexes);
	}

	VOID THRASHAPI DrawFan(DWORD count, ThrashVertex vertexArray[])
	{
		Vertex::DrawArray(GL_TRIANGLE_FAN, 2, count, vertexArray);
	}

	VOID THRASHAPI DrawFan(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		Vertex::DrawIndexedArray(GL_TRIANGLE_FAN, 2, count, vertexArray, indexes);
	}

	VOID THRASHAPI DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		Vertex::DrawIndexedArray(GL_TRIANGLES, 0, count * 3, vertexArray, indexes);
	}
}