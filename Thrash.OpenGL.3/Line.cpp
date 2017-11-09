#include "stdafx.h"
#include "Global.hpp"

namespace Line
{
	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2)
	{
		Buffer::AddLine(vertex1, vertex2);
	}

	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[])
	{
		if (count > 0)
		{
			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			ThrashVertex* vertex1 = vArray++;
			ThrashVertex* vertex2 = vArray++;

			Buffer::AddLine(vertex1, vertex2);

			while (--count)
			{
				vertex1 = vertex2;
				vertex2 = vArray++;

				Buffer::AddLine(vertex1, vertex2);
			}
		}
	}

	VOID __fastcall DrawStrip(DWORD count, ThrashVertex vertexArray[], WORD indexes[])
	{
		if (count > 0)
		{
			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			ThrashVertex* vertex1 = &vArray[*indexes++];
			ThrashVertex* vertex2 = &vArray[*indexes++];

			Buffer::AddLine(vertex1, vertex2);

			while (--count)
			{
				vertex1 = vertex2;
				vertex2 = &vArray[*indexes++];

				Buffer::AddLine(vertex1, vertex2);
			}
		}
	}

	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			ThrashVertex* vertex1 = &vArray[*indexes++];
			ThrashVertex* vertex2 = &vArray[*indexes++];

			Buffer::AddLine(vertex1, vertex2);

			while (--count)
			{
				vertex1 = vertex2;
				vertex2 = &vArray[*indexes++];

				Buffer::AddLine(vertex1, vertex2);
			}
		}
	}

	VOID __fastcall DrawMesh(DWORD count, ThrashVertex vertexArray[])
	{
		if (count > 0)
		{
			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			do
			{
				ThrashVertex* vertex1 = vArray++;
				ThrashVertex* vertex2 = vArray++;

				Buffer::AddLine(vertex1, vertex2);
			} while (--count);
		}
	}

	VOID __fastcall DrawMesh(DWORD count, ThrashVertex vertexArray[], WORD indexes[])
	{
		if (count > 0)
		{
			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			do
			{
				ThrashVertex* vertex1 = vArray++;
				ThrashVertex* vertex2 = vArray++;

				Buffer::AddLine(vertex1, vertex2);
			} while (--count);
		}
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

				Buffer::AddLine(vertex1, vertex2);
			} while (--count);
		}
	}
}