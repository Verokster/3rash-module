#include "stdafx.h"
#include "Global.hpp"

namespace Sprite
{
	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2)
	{
		Buffer::Check(GL_TRIANGLES);
		
		DWORD index1, index2;

		ThrashVertexV1* v1 = (ThrashVertexV1*)vertex1;
		ThrashVertexV1* v2 = (ThrashVertexV1*)vertex2;

		ThrashVertexV1 vertexData = *v1;
		vertexData.vertCoord.x = (*v2).vertCoord.x;
		vertexData.texCoord0.u = (*v2).texCoord0.u;

		BOOL check = Tri::CheckCullFace(v1, &vertexData, v2);
		if (check)
		{
			index1 = Buffer::AddVertex(v1);
					 Buffer::AddVertex(&vertexData);
			index2 = Buffer::AddVertex(v2);
		}

		vertexData = *v2;
		vertexData.vertCoord.x = (*v1).vertCoord.x;
		vertexData.texCoord0.u = (*v1).texCoord0.u;

		if (Tri::CheckCullFace(v1, v2, &vertexData))
		{
			if (!check || Buffer::Check(GL_TRIANGLES))
			{
				Buffer::AddVertex(v1);
				Buffer::AddVertex(v2);
			}
			else
			{
				Buffer::AddIndex(index1);
				Buffer::AddIndex(index2);
			}
			Buffer::AddVertex(&vertexData);
		}
	}

	VOID THRASHAPI DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);

			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;
			ThrashVertexV1 vertexData;
			DWORD index1, index2;

			do
			{
				Buffer::Check(GL_TRIANGLES);

				ThrashVertexV1* v1 = &vArray[*indexes++];
				ThrashVertexV1* v2 = &vArray[*indexes++];

				vertexData = *v1;
				vertexData.vertCoord.x = (*v2).vertCoord.x;
				vertexData.texCoord0.u = (*v2).texCoord0.u;

				BOOL check = Tri::CheckCullFace(v1, &vertexData, v2);
				if (check)
				{
					index1 = Buffer::AddVertex(v1);
							 Buffer::AddVertex(&vertexData);
					index2 = Buffer::AddVertex(v2);
				}

				vertexData = *v2;
				vertexData.vertCoord.x = (*v1).vertCoord.x;
				vertexData.texCoord0.u = (*v1).texCoord0.u;

				if (Tri::CheckCullFace(v1, v2, &vertexData))
				{
					if (!check || Buffer::Check(GL_TRIANGLES))
					{
						Buffer::AddVertex(v1);
						Buffer::AddVertex(v2);
					}
					else
					{
						Buffer::AddIndex(index1);
						Buffer::AddIndex(index2);
					}
					Buffer::AddVertex(&vertexData);
				}
			} while (--count);
		}
	}
}