#include "stdafx.h"
#include "Global.hpp"

namespace Quad
{
	VOID __stdcall Draw(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3, ThrashVertex* vertex4)
	{
		Texture::CheckWrap();
		GLBegin(GL_TRIANGLE_FAN);
		{
			Vertex::Draw(vertex1);
			Vertex::Draw(vertex2);
			Vertex::Draw(vertex3);
			Vertex::Draw(vertex4);
		}
		GLEnd();
	}

	VOID __stdcall DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			Texture::CheckWrap();
			GLBegin(GL_TRIANGLES);
			{
				do
				{
					ThrashVertex* vertex1 = &vertexArray[*indexes++];
					Vertex::Draw(vertex1);

					Vertex::Draw(&vertexArray[*indexes++]);

					ThrashVertex* vertex3 = &vertexArray[*indexes++];
					Vertex::Draw(vertex3);

					Vertex::Draw(vertex1);
					Vertex::Draw(vertex3);
					Vertex::Draw(&vertexArray[*indexes++]);

					--count;
				} while (count);
			}
			GLEnd();
		}
	}
}