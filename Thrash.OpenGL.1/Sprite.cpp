#include "stdafx.h"
#include "Global.hpp"

namespace Sprite
{
	VOID __stdcall Draw(ThrashVertex* vertex1, ThrashVertex* vertex2)
	{
		Texture::CheckWrap();
		GLBegin(GL_TRIANGLE_FAN);
		{
			Vertex::Draw(vertex1);
			ThrashVertex vertexData = *vertex1;
			vertexData.vertCoord.x = (*vertex2).vertCoord.x;
			vertexData.texCoord.u = (*vertex2).texCoord.u;
			Vertex::Draw(&vertexData);

			Vertex::Draw(vertex2);
			vertexData = *vertex2;
			vertexData.vertCoord.x = (*vertex1).vertCoord.x;
			vertexData.texCoord.u = (*vertex1).texCoord.u;
			Vertex::Draw(&vertexData);
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
					ThrashVertex* vertex3 = &vertexArray[*indexes++];

					Vertex::Draw(vertex1);

					ThrashVertex vertexData = *vertex1;
					vertexData.vertCoord.x = (*vertex3).vertCoord.x;
					vertexData.texCoord.u = (*vertex3).texCoord.u;
					Vertex::Draw(&vertexData);

					Vertex::Draw(vertex3);

					Vertex::Draw(vertex1);
					Vertex::Draw(vertex3);
					
					vertexData = *vertex3;
					vertexData.vertCoord.x = (*vertex1).vertCoord.x;
					vertexData.texCoord.u = (*vertex1).texCoord.u;
					Vertex::Draw(&vertexData);

					--count;
				} while (count);
			}
			GLEnd();
		}
	}
}