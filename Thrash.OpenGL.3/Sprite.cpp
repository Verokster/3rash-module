#include "stdafx.h"
#include "Global.hpp"

namespace Sprite
{
	VOID __stdcall Draw(ThrashVertex* vertex1, ThrashVertex* vertex2)
	{
		Buffer::Check(GL_TRIANGLES);
		Texture::CheckWrap();
		
		DWORD index1, index2;

		ThrashVertex vertexData = *vertex1;
		vertexData.vertCoord.x = (*vertex2).vertCoord.x;
		vertexData.texCoord.u = (*vertex2).texCoord.u;

		BOOL check = Tri::CheckCullFace(vertex1, &vertexData, vertex2);
		if (check)
		{
			index1 = Buffer::AddVertex(vertex1);
					 Buffer::AddVertex(&vertexData);
			index2 = Buffer::AddVertex(vertex2);
		}

		vertexData = *vertex2;
		vertexData.vertCoord.x = (*vertex1).vertCoord.x;
		vertexData.texCoord.u = (*vertex1).texCoord.u;

		if (Tri::CheckCullFace(vertex1, vertex2, &vertexData))
		{
			if (!check || Buffer::Check(GL_TRIANGLES))
			{
				Buffer::AddVertex(vertex1);
				Buffer::AddVertex(vertex2);
			}
			else
			{
				Buffer::AddIndex(index1);
				Buffer::AddIndex(index2);
			}
			Buffer::AddVertex(&vertexData);
		}
	}

	VOID __stdcall DrawMesh(DWORD count, ThrashVertex vertexPtrArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);
			Texture::CheckWrap();

			DWORD index1, index2;

			do
			{
				Buffer::Check(GL_TRIANGLES);

				ThrashVertex* vertex1 = &vertexPtrArray[*indexes++];
				ThrashVertex* vertex2 = &vertexPtrArray[*indexes++];
				ThrashVertex vertexData = *vertex1;
				vertexData.vertCoord.x = (*vertex2).vertCoord.x;
				vertexData.texCoord.u = (*vertex2).texCoord.u;

				BOOL check = Tri::CheckCullFace(vertex1, &vertexData, vertex2);
				if (check)
				{
					index1 = Buffer::AddVertex(vertex1);
							 Buffer::AddVertex(&vertexData);
					index2 = Buffer::AddVertex(vertex2);
				}

				vertexData = *vertex2;
				vertexData.vertCoord.x = (*vertex1).vertCoord.x;
				vertexData.texCoord.u = (*vertex1).texCoord.u;

				if (Tri::CheckCullFace(vertex1, vertex2, &vertexData))
				{
					if (!check || Buffer::Check(GL_TRIANGLES))
					{
						Buffer::AddVertex(vertex1);
						Buffer::AddVertex(vertex2);
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