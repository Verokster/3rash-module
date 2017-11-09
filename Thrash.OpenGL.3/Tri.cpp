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