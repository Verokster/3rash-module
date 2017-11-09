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
	VOID THRASHAPI Draw(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3)
	{
		Texture::CheckWrap();
		GLBegin(GL_TRIANGLES);
		{
			if (!extendedVertex)
			{
				Vertex::Draw((ThrashVertexV1*)vertex1);
				Vertex::Draw((ThrashVertexV1*)vertex2);
				Vertex::Draw((ThrashVertexV1*)vertex3);
			}
			else
			{
				Vertex::Draw((ThrashVertexV2*)vertex1);
				Vertex::Draw((ThrashVertexV2*)vertex2);
				Vertex::Draw((ThrashVertexV2*)vertex3);
			}
		}
		GLEnd();
	}

	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[])
	{
		Vertex::DrawArray(GL_TRIANGLE_STRIP, 2, count, vertexArray);
	}

	VOID THRASHAPI DrawStrip(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		Vertex::DrawIndexedArray(GL_TRIANGLE_STRIP, 2, count, vertexArray, indexes);
	}

	VOID THRASHAPI DrawFan(DWORD count, ThrashVertex vertexArray[])
	{
		Vertex::DrawArray(GL_TRIANGLE_FAN, 2, count, vertexArray);
	}

	VOID THRASHAPI DrawFan(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		Vertex::DrawIndexedArray(GL_TRIANGLE_FAN, 2, count, vertexArray, indexes);
	}

	VOID THRASHAPI DrawMesh(DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		Vertex::DrawIndexedArray(GL_TRIANGLES, 0, count * 3, vertexArray, indexes);
	}
}