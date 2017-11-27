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

namespace Quad
{
	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3, ThrashVertex* vertex4)
	{
		Texture::CheckWrap();
		GLBegin(GL_TRIANGLE_FAN);
		{
			if (!extendedVertex)
			{
				Vertex::Draw((ThrashVertexV1*)vertex1);
				Vertex::Draw((ThrashVertexV1*)vertex2);
				Vertex::Draw((ThrashVertexV1*)vertex3);
				Vertex::Draw((ThrashVertexV1*)vertex4);
			}
			else
			{
				Vertex::Draw((ThrashVertexV2*)vertex1);
				Vertex::Draw((ThrashVertexV2*)vertex2);
				Vertex::Draw((ThrashVertexV2*)vertex3);
				Vertex::Draw((ThrashVertexV2*)vertex4);
			}
		}
		GLEnd();
	}

	VOID THRASHAPI DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count)
		{
			Texture::CheckWrap();
			GLBegin(GL_TRIANGLES);
			{
				if (!extendedVertex)
				{
					ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;

					do
					{
						ThrashVertexV1* vertex1 = (ThrashVertexV1*)&vArray[*indexes++];
						Vertex::Draw(vertex1);

						Vertex::Draw((ThrashVertexV1*)&vArray[*indexes++]);

						ThrashVertexV1* vertex3 = (ThrashVertexV1*)&vArray[*indexes++];
						Vertex::Draw(vertex3);

						Vertex::Draw(vertex1);
						Vertex::Draw(vertex3);
						Vertex::Draw((ThrashVertexV1*)&vArray[*indexes++]);
					} while (--count);
				}
				else
				{
					ThrashVertexV2* vArray = (ThrashVertexV2*)vertexArray;

					do
					{
						ThrashVertexV2* vertex1 = (ThrashVertexV2*)&vArray[*indexes++];
						Vertex::Draw(vertex1);

						Vertex::Draw((ThrashVertexV2*)&vArray[*indexes++]);

						ThrashVertexV2* vertex3 = (ThrashVertexV2*)&vArray[*indexes++];
						Vertex::Draw(vertex3);

						Vertex::Draw(vertex1);
						Vertex::Draw(vertex3);
						Vertex::Draw((ThrashVertexV2*)&vArray[*indexes++]);
					} while (--count);
				}
			}
			GLEnd();
		}
	}
}