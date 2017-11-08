#include "stdafx.h"
#include "Global.hpp"

namespace Quad
{
	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3, ThrashVertex* vertex4)
	{
		Texture::CheckWrap();
		GLBegin(GL_TRIANGLE_FAN);
		{
			if (!extendedVertex)
			{
				Vertex::Draw((ThrashVertexV1*)vertex1);
				Vertex::Draw((ThrashVertexV1*)vertex2);
				Vertex::Draw((ThrashVertexV1*)vertex3);
				Vertex::Draw((ThrashVertexV1*)vertex4);
			}
			else
			{
				Vertex::Draw((ThrashVertexV2*)vertex1);
				Vertex::Draw((ThrashVertexV2*)vertex2);
				Vertex::Draw((ThrashVertexV2*)vertex3);
				Vertex::Draw((ThrashVertexV2*)vertex4);
			}
		}
		GLEnd();
	}

	VOID THRASHAPI DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			Texture::CheckWrap();
			GLBegin(GL_TRIANGLES);
			{
				if (!extendedVertex)
				{
					ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

					do
					{
						ThrashVertexV1* vertex1 = (ThrashVertexV1*)&vArray[*indexes++];
						Vertex::Draw(vertex1);

						Vertex::Draw((ThrashVertexV1*)&vArray[*indexes++]);

						ThrashVertexV1* vertex3 = (ThrashVertexV1*)&vArray[*indexes++];
						Vertex::Draw(vertex3);

						Vertex::Draw(vertex1);
						Vertex::Draw(vertex3);
						Vertex::Draw((ThrashVertexV1*)&vArray[*indexes++]);
					} while (--count);
				}
				else
				{
					ThrashVertexV2* vArray = (ThrashVertexV2*)vertexArray;

					do
					{
						ThrashVertexV2* vertex1 = (ThrashVertexV2*)&vArray[*indexes++];
						Vertex::Draw(vertex1);

						Vertex::Draw((ThrashVertexV2*)&vArray[*indexes++]);

						ThrashVertexV2* vertex3 = (ThrashVertexV2*)&vArray[*indexes++];
						Vertex::Draw(vertex3);

						Vertex::Draw(vertex1);
						Vertex::Draw(vertex3);
						Vertex::Draw((ThrashVertexV2*)&vArray[*indexes++]);
					} while (--count);
				}
			}
			GLEnd();
		}
	}
}