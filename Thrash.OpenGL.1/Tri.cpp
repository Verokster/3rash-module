#include "stdafx.h"
#include "Global.hpp"

namespace Tri
{
	VOID __stdcall Draw(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3)
	{
		Texture::CheckWrap();
		GLBegin(GL_TRIANGLES);
		{
			Vertex::Draw(vertex1);
			Vertex::Draw(vertex2);
			Vertex::Draw(vertex3);
		}
		GLEnd();
	}

	VOID __stdcall DrawStrip(DWORD count, ThrashVertex vertexArray[])
	{
		Vertex::DrawArray(GL_TRIANGLE_STRIP, 2, count, vertexArray);
	}

	VOID __stdcall DrawStrip(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		Vertex::DrawIndexedArray(GL_TRIANGLE_STRIP, 2, count, vertexArray, indexes);
	}

	VOID __stdcall DrawFan(DWORD count, ThrashVertex vertexArray[])
	{
		Vertex::DrawArray(GL_TRIANGLE_FAN, 2, count, vertexArray);
	}

	VOID __stdcall DrawFan(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		Vertex::DrawIndexedArray(GL_TRIANGLE_FAN, 2, count, vertexArray, indexes);
	}

	VOID __stdcall DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		Vertex::DrawIndexedArray(GL_TRIANGLES, 0, count * 3, vertexArray, indexes);
	}
}