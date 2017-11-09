#include "stdafx.h"
#include "Global.hpp"

namespace Sprite
{
	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2)
	{
		ThrashVertexV1* v1 = (ThrashVertexV1*)vertex1;
		ThrashVertexV1* v2 = (ThrashVertexV1*)vertex2;

		ThrashVertexV1 vertexData = *v1;
		vertexData.vertCoord.x = (*v2).vertCoord.x;
		vertexData.texCoord0.u = (*v2).texCoord0.u;
		Buffer::AddTri(v1, &vertexData, v2);

		vertexData = *v2;
		vertexData.vertCoord.x = (*v1).vertCoord.x;
		vertexData.texCoord0.u = (*v1).texCoord0.u;
		Buffer::AddTri(v1, v2, &vertexData);
	}

	VOID THRASHAPI DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;
			ThrashVertexV1 vertexData;

			do
			{
				ThrashVertexV1* v1 = &vArray[*indexes++];
				ThrashVertexV1* v2 = &vArray[*indexes++];

				vertexData = *v1;
				vertexData.vertCoord.x = (*v2).vertCoord.x;
				vertexData.texCoord0.u = (*v2).texCoord0.u;
				Buffer::AddTri(v1, &vertexData, v2);

				vertexData = *v2;
				vertexData.vertCoord.x = (*v1).vertCoord.x;
				vertexData.texCoord0.u = (*v1).texCoord0.u;
				Buffer::AddTri(v1, v2, &vertexData);
			} while (--count);
		}
	}
}