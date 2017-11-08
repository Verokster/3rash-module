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
		if (CheckCullFace(vertex1, vertex2, vertex3))
		{
			Buffer::Check(GL_TRIANGLES);

			Buffer::AddVertex(vertex1);
			Buffer::AddVertex(vertex2);
			Buffer::AddVertex(vertex3);
		}
	}

	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);

			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;
			DWORD index1, index2;
			
			ThrashVertex* vertex1 = vArray++;
			ThrashVertex* vertex2 = vArray++;
			ThrashVertex* vertex3 = vArray++;

			BOOL check = Tri::CheckCullFace(vertex1, vertex2, vertex3);
			if (check)
			{
						 Buffer::AddVertex(vertex1);
				index1 = Buffer::AddVertex(vertex2);
				index2 = Buffer::AddVertex(vertex3);
			}
			
			BOOL cw = FALSE;
			while (--count)
			{
				vertex1 = vertex2;
				vertex2 = vertex3;
				vertex3 = vArray++;

				BOOL newCheck;
				if (cw)
				{
					newCheck = Tri::CheckCullFace(vertex1, vertex2, vertex3);
					if (newCheck)
					{
						if (!check || Buffer::Check(GL_TRIANGLES))
						{
							Buffer::AddVertex(vertex1);
							index1 = Buffer::AddVertex(vertex2);
						}
						else
						{
							Buffer::AddIndex(index1);
							Buffer::AddIndex(index2);
							index1 = index2;
						}
						index2 = Buffer::AddVertex(vertex3);
					}
				}
				else
				{
					newCheck = Tri::CheckCullFace(vertex2, vertex1, vertex3);
					if (newCheck)
					{
						if (!check || Buffer::Check(GL_TRIANGLES))
						{
							index1 = Buffer::AddVertex(vertex2);
							Buffer::AddVertex(vertex1);
						}
						else
						{
							Buffer::AddIndex(index2);
							Buffer::AddIndex(index1);
						
							index1 = index2;
						}
						index2 = Buffer::AddVertex(vertex3);
					}
				}

				check = newCheck;
				cw = !cw;
			}
		}
	}

	VOID __fastcall DrawStrip(DWORD count, ThrashVertex vertexArray[], WORD indexes[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);

			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;
			DWORD index1, index2;

			ThrashVertex* vertex1 = &vArray[*indexes++];
			ThrashVertex* vertex2 = &vArray[*indexes++];
			ThrashVertex* vertex3 = &vArray[*indexes++];

			BOOL check = Tri::CheckCullFace(vertex1, vertex2, vertex3);
			if (check)
			{
				Buffer::AddVertex(vertex1);
				index1 = Buffer::AddVertex(vertex2);
				index2 = Buffer::AddVertex(vertex3);
			}

			BOOL cw = FALSE;
			while (--count)
			{
				vertex1 = vertex2;
				vertex2 = vertex3;
				vertex3 = &vArray[*indexes++];

				BOOL newCheck;
				if (cw)
				{
					newCheck = Tri::CheckCullFace(vertex1, vertex2, vertex3);
					if (newCheck)
					{
						if (!check || Buffer::Check(GL_TRIANGLES))
						{
							Buffer::AddVertex(vertex1);
							index1 = Buffer::AddVertex(vertex2);
						}
						else
						{
							Buffer::AddIndex(index1);
							Buffer::AddIndex(index2);
							index1 = index2;
						}
						index2 = Buffer::AddVertex(vertex3);
					}
				}
				else
				{
					newCheck = Tri::CheckCullFace(vertex2, vertex1, vertex3);
					if (newCheck)
					{
						if (!check || Buffer::Check(GL_TRIANGLES))
						{
							index1 = Buffer::AddVertex(vertex2);
							Buffer::AddVertex(vertex1);
						}
						else
						{
							Buffer::AddIndex(index2);
							Buffer::AddIndex(index1);

							index1 = index2;
						}
						index2 = Buffer::AddVertex(vertex3);
					}
				}

				check = newCheck;
				cw = !cw;
			}
		}
	}

	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);

			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;
			DWORD index1, index2;
			
			ThrashVertex* vertex1 = &vArray[*indexes++];
			ThrashVertex* vertex2 = &vArray[*indexes++];
			ThrashVertex* vertex3 = &vArray[*indexes++];

			BOOL check = Tri::CheckCullFace(vertex1, vertex2, vertex3);
			if (check)
			{
						 Buffer::AddVertex(vertex1);
				index1 = Buffer::AddVertex(vertex2);
				index2 = Buffer::AddVertex(vertex3);
			}
			
			BOOL cw = FALSE;
			while (--count)
			{
				vertex1 = vertex2;
				vertex2 = vertex3;
				vertex3 = &vArray[*indexes++];

				BOOL newCheck;
				if (cw)
				{
					newCheck = Tri::CheckCullFace(vertex1, vertex2, vertex3);
					if (newCheck)
					{
						if (!check || Buffer::Check(GL_TRIANGLES))
						{
							Buffer::AddVertex(vertex1);
							index1 = Buffer::AddVertex(vertex2);
						}
						else
						{
							Buffer::AddIndex(index1);
							Buffer::AddIndex(index2);
							index1 = index2;
						}
						index2 = Buffer::AddVertex(vertex3);
					}
				}
				else
				{
					newCheck = Tri::CheckCullFace(vertex2, vertex1, vertex3);
					if (newCheck)
					{
						if (!check || Buffer::Check(GL_TRIANGLES))
						{
							index1 = Buffer::AddVertex(vertex2);
							Buffer::AddVertex(vertex1);
						}
						else
						{
							Buffer::AddIndex(index2);
							Buffer::AddIndex(index1);
						
							index1 = index2;
						}
						index2 = Buffer::AddVertex(vertex3);
					}
				}

				check = newCheck;
				cw = !cw;
			}
		}
	}

	VOID THRASHAPI DrawFan(DWORD count, ThrashVertex vertexArray[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);

			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;
			DWORD index1, index3;

			ThrashVertex* vertex1 = vArray++;
			ThrashVertex* vertex2 = vArray++;
			ThrashVertex* vertex3 = vArray++;

			BOOL check = Tri::CheckCullFace(vertex1, vertex2, vertex3);
			if (check)
			{
				index1 = Buffer::AddVertex(vertex1);
						 Buffer::AddVertex(vertex2);
				index3 = Buffer::AddVertex(vertex3);
			}

			while (--count)
			{
				vertex2 = vertex3;
				vertex3 = vArray++;

				BOOL newCheck = Tri::CheckCullFace(vertex1, vertex2, vertex3);
				if (newCheck)
				{
					if (!check || Buffer::Check(GL_TRIANGLES))
					{
						index1 = Buffer::AddVertex(vertex1);
						Buffer::AddVertex(vertex2);
					}
					else
					{
						Buffer::AddIndex(index1);
						Buffer::AddIndex(index3);
					}
					index3 = Buffer::AddVertex(vertex3);
				}

				check = newCheck;
			}
		}
	}

	VOID __fastcall DrawFan(DWORD count, ThrashVertex vertexArray[], WORD indexes[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);

			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;
			DWORD index1, index2;

			ThrashVertex* vertex1 = &vArray[*indexes++];
			ThrashVertex* vertex2 = &vArray[*indexes++];
			ThrashVertex* vertex3 = &vArray[*indexes++];

			BOOL check = Tri::CheckCullFace(vertex1, vertex2, vertex3);
			if (check)
			{
				index1 = Buffer::AddVertex(vertex1);
				Buffer::AddVertex(vertex2);
				index2 = Buffer::AddVertex(vertex3);
			}

			while (--count)
			{
				vertex2 = vertex3;
				vertex3 = &vArray[*indexes++];

				BOOL newCheck = Tri::CheckCullFace(vertex1, vertex2, vertex3);
				if (newCheck)
				{
					if (!check || Buffer::Check(GL_TRIANGLES))
					{
						index1 = Buffer::AddVertex(vertex1);
						Buffer::AddVertex(vertex2);
					}
					else
					{
						Buffer::AddIndex(index1);
						Buffer::AddIndex(index2);
					}
					index2 = Buffer::AddVertex(vertex3);
				}

				check = newCheck;
			}
		}
	}

	VOID THRASHAPI DrawFan(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);

			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;
			DWORD index1, index2;

			ThrashVertex* vertex1 = &vArray[*indexes++];
			ThrashVertex* vertex2 = &vArray[*indexes++];
			ThrashVertex* vertex3 = &vArray[*indexes++];

			BOOL check = Tri::CheckCullFace(vertex1, vertex2, vertex3);
			if (check)
			{
				index1 = Buffer::AddVertex(vertex1);
						 Buffer::AddVertex(vertex2);
				index2 = Buffer::AddVertex(vertex3);
			}

			while (--count)
			{
				vertex2 = vertex3;
				vertex3 = &vArray[*indexes++];

				BOOL newCheck = Tri::CheckCullFace(vertex1, vertex2, vertex3);
				if (newCheck)
				{
					if (!check || Buffer::Check(GL_TRIANGLES))
					{
						index1 = Buffer::AddVertex(vertex1);
						Buffer::AddVertex(vertex2);
					}
					else
					{
						Buffer::AddIndex(index1);
						Buffer::AddIndex(index2);
					}
					index2 = Buffer::AddVertex(vertex3);
				}

				check = newCheck;
			}
		}
	}

	VOID __fastcall DrawMesh(DWORD count, ThrashVertex vertexArray[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);

			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			do
			{
				ThrashVertex* vertex1 = vArray++;
				ThrashVertex* vertex2 = vArray++;
				ThrashVertex* vertex3 = vArray++;

				if (CheckCullFace(vertex1, vertex2, vertex3))
				{
					Buffer::Check(GL_TRIANGLES);

					Buffer::AddVertex(vertex1);
					Buffer::AddVertex(vertex2);
					Buffer::AddVertex(vertex3);
				}
			} while (--count);
		}
	}

	VOID __fastcall DrawMesh(DWORD count, ThrashVertex vertexArray[], WORD indexes[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);

			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			do
			{
				ThrashVertex* vertex1 = &vArray[*indexes++];
				ThrashVertex* vertex2 = &vArray[*indexes++];
				ThrashVertex* vertex3 = &vArray[*indexes++];

				if (CheckCullFace(vertex1, vertex2, vertex3))
				{
					Buffer::Check(GL_TRIANGLES);

					Buffer::AddVertex(vertex1);
					Buffer::AddVertex(vertex2);
					Buffer::AddVertex(vertex3);
				}
			} while (--count);
		}
	}

	VOID THRASHAPI DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);

			ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

			do
			{
				ThrashVertex* vertex1 = &vArray[*indexes++];
				ThrashVertex* vertex2 = &vArray[*indexes++];
				ThrashVertex* vertex3 = &vArray[*indexes++];

				if (CheckCullFace(vertex1, vertex2, vertex3))
				{
					Buffer::Check(GL_TRIANGLES);

					Buffer::AddVertex(vertex1);
					Buffer::AddVertex(vertex2);
					Buffer::AddVertex(vertex3);
				}
			} while (--count);
		}
	}
}