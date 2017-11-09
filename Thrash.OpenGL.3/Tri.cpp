/*
	MIT License

	Copyright (c) 2017 Oleksiy Ryabchun

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "stdafx.h"
#include "Global.hpp"

namespace Tri
{
	BOOL __fastcall CheckCullFace(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3)
	{
		if (cullFace)
		{
			ThrashVertexV1* v1 = (ThrashVertexV1*)vertex1;
			ThrashVertexV1* v2 = (ThrashVertexV1*)vertex2;
			ThrashVertexV1* v3 = (ThrashVertexV1*)vertex3;

			FLOAT check = (v3->vertCoord.y - v1->vertCoord.y) * (v2->vertCoord.x - v1->vertCoord.x)
				- (v2->vertCoord.y - v1->vertCoord.y) * (v3->vertCoord.x - v1->vertCoord.x);

			if (cullFace == 1)
				return check > 0.0; // GL_CW
			else
				return check < 0.0; // GL_CCW
		}

		return TRUE;
	}

	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3)
	{
		Buffer::AddTri(vertex1, vertex2, vertex3);
	}

	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[])
	{
		if (count > 0)
		{
			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			ThrashVertex* vertex1 = vArray++;
			ThrashVertex* vertex2 = vArray++;
			ThrashVertex* vertex3 = vArray++;

			Buffer::AddTri(vertex1, vertex2, vertex3);

			BOOL cw = FALSE;
			while (--count)
			{
				vertex1 = vertex2;
				vertex2 = vertex3;
				vertex3 = vArray++;

				if (cw)
					Buffer::AddTri(vertex1, vertex2, vertex3);
				else
					Buffer::AddTri(vertex2, vertex1, vertex3);

				cw = !cw;
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
			ThrashVertex* vertex3 = &vArray[*indexes++];

			Buffer::AddTri(vertex1, vertex2, vertex3);

			BOOL cw = FALSE;
			while (--count)
			{
				vertex1 = vertex2;
				vertex2 = vertex3;
				vertex3 = &vArray[*indexes++];

				if (cw)
					Buffer::AddTri(vertex1, vertex2, vertex3);
				else
					Buffer::AddTri(vertex2, vertex1, vertex3);

				cw = !cw;
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
			ThrashVertex* vertex3 = &vArray[*indexes++];

			Buffer::AddTri(vertex1, vertex2, vertex3);

			BOOL cw = FALSE;
			while (--count)
			{
				vertex1 = vertex2;
				vertex2 = vertex3;
				vertex3 = &vArray[*indexes++];

				if (cw)
					Buffer::AddTri(vertex1, vertex2, vertex3);
				else
					Buffer::AddTri(vertex2, vertex1, vertex3);

				cw = !cw;
			}
		}
	}

	VOID THRASHAPI DrawFan(DWORD count, ThrashVertex vertexArray[])
	{
		if (count > 0)
		{
			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			ThrashVertex* vertex1 = vArray++;
			ThrashVertex* vertex2 = vArray++;
			ThrashVertex* vertex3 = vArray++;

			Buffer::AddTri(vertex1, vertex2, vertex3);

			while (--count)
			{
				vertex2 = vertex3;
				vertex3 = vArray++;

				Buffer::AddTri(vertex1, vertex2, vertex3);
			}
		}
	}

	VOID __fastcall DrawFan(DWORD count, ThrashVertex vertexArray[], WORD indexes[])
	{
		if (count > 0)
		{
			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			ThrashVertex* vertex1 = &vArray[*indexes++];
			ThrashVertex* vertex2 = &vArray[*indexes++];
			ThrashVertex* vertex3 = &vArray[*indexes++];

			Buffer::AddTri(vertex1, vertex2, vertex3);

			while (--count)
			{
				vertex2 = vertex3;
				vertex3 = &vArray[*indexes++];

				Buffer::AddTri(vertex1, vertex2, vertex3);
			}
		}
	}

	VOID THRASHAPI DrawFan(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			ThrashVertex* vertex1 = &vArray[*indexes++];
			ThrashVertex* vertex2 = &vArray[*indexes++];
			ThrashVertex* vertex3 = &vArray[*indexes++];

			Buffer::AddTri(vertex1, vertex2, vertex3);

			while (--count)
			{
				vertex2 = vertex3;
				vertex3 = &vArray[*indexes++];

				Buffer::AddTri(vertex1, vertex2, vertex3);
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
				ThrashVertex* vertex3 = vArray++;

				Buffer::AddTri(vertex1, vertex2, vertex3);
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
				ThrashVertex* vertex1 = &vArray[*indexes++];
				ThrashVertex* vertex2 = &vArray[*indexes++];
				ThrashVertex* vertex3 = &vArray[*indexes++];

				Buffer::AddTri(vertex1, vertex2, vertex3);
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
				ThrashVertex* vertex3 = &vArray[*indexes++];

				Buffer::AddTri(vertex1, vertex2, vertex3);
			} while (--count);
		}
	}
}