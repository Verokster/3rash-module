#include "stdafx.h"
#include "Global.hpp"

namespace Line
{
	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2)
	{
		Buffer::Check(GL_LINES);
		Buffer::AddVertex(vertex1);
		Buffer::AddVertex(vertex2);
	}

	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_LINES);

			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			Buffer::AddVertex(vArray++);
			ThrashVertex* vertex = vArray++;
			DWORD index = Buffer::AddVertex(vertex);

			while(--count)
			{
				if (Buffer::Check(GL_LINES))
					Buffer::AddVertex(vertex);
				else
					Buffer::AddIndex(index);

				index = Buffer::AddVertex(vArray++);
			}
		}
	}

	VOID __fastcall DrawStrip(DWORD count, ThrashVertex vertexArray[], WORD indexes[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_LINES);

			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			Buffer::AddVertex(&vArray[*indexes++]);
			ThrashVertex* vertex = &vArray[*indexes++];
			DWORD index = Buffer::AddVertex(vertex);

			while (--count)
			{
				if (Buffer::Check(GL_LINES))
					Buffer::AddVertex(vertex);
				else
					Buffer::AddIndex(index);

				index = Buffer::AddVertex(&vArray[*indexes++]);
			}
		}
	}

	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_LINES);

			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			Buffer::AddVertex(&vArray[*indexes++]);
			ThrashVertex* vertex = &vArray[*indexes++];
			DWORD index = Buffer::AddVertex(vertex);

			while(--count)
			{
				if (Buffer::Check(GL_LINES))
					Buffer::AddVertex(vertex);
				else
					Buffer::AddIndex(index);

				index = Buffer::AddVertex(&vArray[*indexes++]);
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
				Buffer::Check(GL_LINES);
				Buffer::AddVertex(vArray++);
				Buffer::AddVertex(vArray++);
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
				Buffer::Check(GL_LINES);
				Buffer::AddVertex(&vArray[*indexes++]);
				Buffer::AddVertex(&vArray[*indexes++]);
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
				Buffer::Check(GL_LINES);
				Buffer::AddVertex(&vArray[*indexes++]);
				Buffer::AddVertex(&vArray[*indexes++]);
			} while (--count);
		}
	}
}