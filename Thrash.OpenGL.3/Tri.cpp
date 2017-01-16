#include "stdafx.h"
#include "Global.hpp"

namespace Tri
{
	VOID __stdcall Draw(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3)
	{
		if (CheckCullFace(vertex1, vertex2, vertex3))
		{
			Buffer::Check(GL_TRIANGLES);
			Texture::CheckWrap();

			Buffer::AddVertex(vertex1);
			Buffer::AddVertex(vertex2);
			Buffer::AddVertex(vertex3);
		}
	}

	VOID __stdcall DrawStrip(DWORD count, ThrashVertex vertexArray[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);
			Texture::CheckWrap();

			DWORD index1, index2;
			
			ThrashVertex* vertex1 = vertexArray++;
			ThrashVertex* vertex2 = vertexArray++;
			ThrashVertex* vertex3 = vertexArray++;

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
				vertex3 = vertexArray++;

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

	VOID __stdcall DrawStrip(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);
			Texture::CheckWrap();

			DWORD index1, index2;
			
			ThrashVertex* vertex1 = &vertexArray[*indexes++];
			ThrashVertex* vertex2 = &vertexArray[*indexes++];
			ThrashVertex* vertex3 = &vertexArray[*indexes++];

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
				vertex3 = &vertexArray[*indexes++];

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

	VOID __stdcall DrawFan(DWORD count, ThrashVertex vertexArray[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);
			Texture::CheckWrap();

			DWORD index1, index3;

			ThrashVertex* vertex1 = vertexArray++;
			ThrashVertex* vertex2 = vertexArray++;
			ThrashVertex* vertex3 = vertexArray++;

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
				vertex3 = vertexArray++;

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

	VOID __stdcall DrawFan(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);
			Texture::CheckWrap();

			DWORD index1, index2;

			ThrashVertex* vertex1 = &vertexArray[*indexes++];
			ThrashVertex* vertex2 = &vertexArray[*indexes++];
			ThrashVertex* vertex3 = &vertexArray[*indexes++];

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
				vertex3 = &vertexArray[*indexes++];

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

	VOID __stdcall DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			Buffer::Check(GL_TRIANGLES);
			Texture::CheckWrap();

			do
			{
				ThrashVertex* vertex1 = &vertexArray[*indexes++];
				ThrashVertex* vertex2 = &vertexArray[*indexes++];
				ThrashVertex* vertex3 = &vertexArray[*indexes++];

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

	BOOL __fastcall CheckCullFace(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3)
	{
		if (cullFace != 0)
		{
			FLOAT check = ((*vertex3).vertCoord.y - (*vertex1).vertCoord.y) * ((*vertex2).vertCoord.x - (*vertex1).vertCoord.x)
						- ((*vertex2).vertCoord.y - (*vertex1).vertCoord.y) * ((*vertex3).vertCoord.x - (*vertex1).vertCoord.x);

			if (cullFace == 1)
				return check > 0.0; // GL_CW
			else
				return check < 0.0; // GL_CCW
		}

		return TRUE;
	}
}