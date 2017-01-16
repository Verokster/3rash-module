#include "stdafx.h"
#include "Global.hpp"

namespace Point
{
	VOID __stdcall Draw(ThrashVertex* vertex)
	{
		Buffer::Check(GL_POINTS);
		Buffer::AddVertex(vertex);
	}

	VOID __stdcall DrawStrip(DWORD count, ThrashVertex vertexArray[])
	{
		if (count > 0)
		{
			do
			{
				Buffer::Check(GL_POINTS);
				Buffer::AddVertex(vertexArray++);
			} while (--count);
		}
	}

	VOID __stdcall DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			do
			{
				Buffer::Check(GL_POINTS);
				Buffer::AddVertex(&vertexArray[*indexes++]);
			} while (--count);
		}
	}
}