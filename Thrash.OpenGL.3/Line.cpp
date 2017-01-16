#include "stdafx.h"
#include "Global.hpp"

namespace Line
{
	VOID __stdcall Draw(ThrashVertex* vertex1, ThrashVertex* vertex2)
	{
		Buffer::Check(GL_LINES);
		Buffer::AddVertex(vertex1);
		Buffer::AddVertex(vertex2);
	}

	VOID __stdcall DrawStrip(DWORD count, ThrashVertex vertexArray[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_LINES);

			Buffer::AddVertex(vertexArray++);
			ThrashVertex* vertex = vertexArray++;
			DWORD index = Buffer::AddVertex(vertex);

			while(--count)
			{
				if (Buffer::Check(GL_LINES))
					Buffer::AddVertex(vertex);
				else
					Buffer::AddIndex(index);

				index = Buffer::AddVertex(vertexArray++);
			}
		}
	}

	VOID __stdcall DrawStrip(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_LINES);

			Buffer::AddVertex(&vertexArray[*indexes++]);
			ThrashVertex* vertex = &vertexArray[*indexes++];
			DWORD index = Buffer::AddVertex(vertex);

			while(--count)
			{
				if (Buffer::Check(GL_LINES))
					Buffer::AddVertex(vertex);
				else
					Buffer::AddIndex(index);

				index = Buffer::AddVertex(&vertexArray[*indexes++]);
			}
		}
	}

	VOID __stdcall DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			do
			{
				Buffer::Check(GL_LINES);
				Buffer::AddVertex(&vertexArray[*indexes++]);
				Buffer::AddVertex(&vertexArray[*indexes++]);
			} while (--count);
		}
	}
}