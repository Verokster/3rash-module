#include "stdafx.h"
#include "Global.hpp"

namespace Sprite
{
	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2)
	{
		Texture::CheckWrap();
		GLBegin(GL_TRIANGLE_FAN);
		{
			if (!extendedVertex)
			{
				ThrashVertexV1* v1 = (ThrashVertexV1*)vertex1;
				ThrashVertexV1* v2 = (ThrashVertexV1*)vertex2;

				Vertex::Draw(v1);

				ThrashVertexV1 vertexData;

				vertexData.vertCoord.x = v2->vertCoord.x;
				vertexData.vertCoord.y = v1->vertCoord.y;
				vertexData.vertCoord.z = v1->vertCoord.z;
				vertexData.vertCoord.rhw = v1->vertCoord.rhw;
				vertexData.diffuseColor = v1->diffuseColor;
				vertexData.specularColor = v1->specularColor;
				vertexData.texCoord0.u = v2->texCoord0.u;
				vertexData.texCoord0.v = v1->texCoord0.v;
				Vertex::Draw(&vertexData);

				Vertex::Draw(v2);

				vertexData.vertCoord.x = v1->vertCoord.x;
				vertexData.vertCoord.y = v2->vertCoord.y;
				vertexData.vertCoord.z = v2->vertCoord.z;
				vertexData.vertCoord.rhw = v2->vertCoord.rhw;
				vertexData.diffuseColor = v2->diffuseColor;
				vertexData.specularColor = v2->specularColor;
				vertexData.texCoord0.u = v1->texCoord0.u;
				vertexData.texCoord0.v = v2->texCoord0.v;
				Vertex::Draw(&vertexData);
			}
			else
			{
				ThrashVertexV2* v1 = (ThrashVertexV2*)vertex1;
				ThrashVertexV2* v2 = (ThrashVertexV2*)vertex2;

				Vertex::Draw(v1);

				ThrashVertexV2 vertexData;

				vertexData.vertCoord.x = v2->vertCoord.x;
				vertexData.vertCoord.y = v1->vertCoord.y;
				vertexData.vertCoord.z = v1->vertCoord.z;
				vertexData.vertCoord.rhw = v1->vertCoord.rhw;
				vertexData.diffuseColor = v1->diffuseColor;
				vertexData.specularColor = v1->specularColor;
				vertexData.texCoord0.u = v2->texCoord0.u;
				vertexData.texCoord0.v = v1->texCoord0.v;
				vertexData.texCoord1.u = v2->texCoord1.u;
				vertexData.texCoord1.v = v1->texCoord1.v;
				Vertex::Draw(&vertexData);

				Vertex::Draw(v2);

				vertexData.vertCoord.x = v1->vertCoord.x;
				vertexData.vertCoord.y = v2->vertCoord.y;
				vertexData.vertCoord.z = v2->vertCoord.z;
				vertexData.vertCoord.rhw = v2->vertCoord.rhw;
				vertexData.diffuseColor = v2->diffuseColor;
				vertexData.specularColor = v2->specularColor;
				vertexData.texCoord0.u = v1->texCoord0.u;
				vertexData.texCoord0.v = v2->texCoord0.v;
				vertexData.texCoord1.u = v1->texCoord1.u;
				vertexData.texCoord1.v = v2->texCoord1.v;
				Vertex::Draw(&vertexData);
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
					ThrashVertexV1 vertexData;

					do
					{
						ThrashVertexV1* v1 = &vArray[*indexes++];
						ThrashVertexV1* v2 = &vArray[*indexes++];

						Vertex::Draw(v1);

						vertexData.vertCoord.x = v2->vertCoord.x;
						vertexData.vertCoord.y = v1->vertCoord.y;
						vertexData.vertCoord.z = v1->vertCoord.z;
						vertexData.vertCoord.rhw = v1->vertCoord.rhw;
						vertexData.diffuseColor = v1->diffuseColor;
						vertexData.specularColor = v1->specularColor;
						vertexData.texCoord0.u = v2->texCoord0.u;
						vertexData.texCoord0.v = v1->texCoord0.v;
						Vertex::Draw(&vertexData);

						Vertex::Draw(v2);

						vertexData.vertCoord.x = v1->vertCoord.x;
						vertexData.vertCoord.y = v2->vertCoord.y;
						vertexData.vertCoord.z = v2->vertCoord.z;
						vertexData.vertCoord.rhw = v2->vertCoord.rhw;
						vertexData.diffuseColor = v2->diffuseColor;
						vertexData.specularColor = v2->specularColor;
						vertexData.texCoord0.u = v1->texCoord0.u;
						vertexData.texCoord0.v = v2->texCoord0.v;
						Vertex::Draw(&vertexData);
					} while (--count);
				}
				else
				{
					ThrashVertexV2* vArray = (ThrashVertexV2*)vertexArray;
					ThrashVertexV2 vertexData;

					do
					{
						ThrashVertexV2* v1 = &vArray[*indexes++];
						ThrashVertexV2* v2 = &vArray[*indexes++];

						Vertex::Draw(v1);

						vertexData.vertCoord.x = v2->vertCoord.x;
						vertexData.vertCoord.y = v1->vertCoord.y;
						vertexData.vertCoord.z = v1->vertCoord.z;
						vertexData.vertCoord.rhw = v1->vertCoord.rhw;
						vertexData.diffuseColor = v1->diffuseColor;
						vertexData.specularColor = v1->specularColor;
						vertexData.texCoord0.u = v2->texCoord0.u;
						vertexData.texCoord0.v = v1->texCoord0.v;
						vertexData.texCoord1.u = v2->texCoord1.u;
						vertexData.texCoord1.v = v1->texCoord1.v;
						Vertex::Draw(&vertexData);

						Vertex::Draw(v2);

						vertexData.vertCoord.x = v1->vertCoord.x;
						vertexData.vertCoord.y = v2->vertCoord.y;
						vertexData.vertCoord.z = v2->vertCoord.z;
						vertexData.vertCoord.rhw = v2->vertCoord.rhw;
						vertexData.diffuseColor = v2->diffuseColor;
						vertexData.specularColor = v2->specularColor;
						vertexData.texCoord0.u = v1->texCoord0.u;
						vertexData.texCoord0.v = v2->texCoord0.v;
						vertexData.texCoord1.u = v1->texCoord1.u;
						vertexData.texCoord1.v = v2->texCoord1.v;
						Vertex::Draw(&vertexData);
					} while (--count);
				}
			}
			GLEnd();
		}
	}
}