#include "stdafx.h"
#include "Global.hpp"

namespace Quad
{
	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3, ThrashVertex* vertex4)
	{
		Buffer::Check(GL_TRIANGLES);

		DWORD index1, index3;
		BOOL check = Tri::CheckCullFace(vertex1, vertex2, vertex3);
		if (check)
		{
			index1 = Buffer::AddVertex(vertex1);
					 Buffer::AddVertex(vertex2);
			index3 = Buffer::AddVertex(vertex3);
		}

		if (Tri::CheckCullFace(vertex1, vertex3, vertex4))
		{
			if (!check || Buffer::Check(GL_TRIANGLES))
			{
				Buffer::AddVertex(vertex1);
				Buffer::AddVertex(vertex3);
			}
			else
			{
				Buffer::AddIndex(index1);
				Buffer::AddIndex(index3);
			}
			Buffer::AddVertex(vertex4);
		}
	}

	VOID THRASHAPI DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);

			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;
			DWORD index1, index3;

			do
			{
				Buffer::Check(GL_TRIANGLES);

				ThrashVertex* vertex1 = &vArray[*indexes++];
				ThrashVertex* vertex2 = &vArray[*indexes++];
				ThrashVertex* vertex3 = &vArray[*indexes++];
				ThrashVertex* vertex4 = &vArray[*indexes++];

				BOOL check = Tri::CheckCullFace(vertex1, vertex2, vertex3);
				if (check)
				{
					index1 = Buffer::AddVertex(vertex1);
							 Buffer::AddVertex(vertex2);
					index3 = Buffer::AddVertex(vertex3);
				}

				if (Tri::CheckCullFace(vertex1, vertex3, vertex4))
				{
					if (!check || Buffer::Check(GL_TRIANGLES))
					{
						Buffer::AddVertex(vertex1);
						Buffer::AddVertex(vertex3);
					}
					else
					{
						Buffer::AddIndex(index1);
						Buffer::AddIndex(index3);
					}

					Buffer::AddVertex(vertex4);
				}
			} while (--count);
		}
	}
}