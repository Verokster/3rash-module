#include "stdafx.h"
#include "Global.hpp"

namespace Point
{
	VOID THRASHAPI Draw(ThrashVertex* vertex)
	{
		Buffer::AddPoint(vertex);
	}

	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[])
	{
		if (count > 0)
		{
			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			do
			{
				Buffer::AddPoint(vArray++);
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
				Buffer::AddPoint(&vArray[*indexes++]);
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
				Buffer::AddPoint(&vArray[*indexes++]);
			} while (--count);
		}
	}
}