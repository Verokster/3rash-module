#include "stdafx.h"
#include "Global.hpp"

namespace Quad
{
	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3, ThrashVertex* vertex4)
	{
		Buffer::AddTri(vertex1, vertex2, vertex3);
		Buffer::AddTri(vertex1, vertex3, vertex4);
	}

	VOID THRASHAPI DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			do
			{
				ThrashVertex* vertex1 = &vArray[*indexes++];
				ThrashVertex* vertex2 = &vArray[*indexes++];
				ThrashVertex* vertex3 = &vArray[*indexes++];
				ThrashVertex* vertex4 = &vArray[*indexes++];

				Buffer::AddTri(vertex1, vertex2, vertex3);
				Buffer::AddTri(vertex1, vertex3, vertex4);
			} while (--count);
		}
	}
}