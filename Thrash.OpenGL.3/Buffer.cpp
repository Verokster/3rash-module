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

#define VBO_SIZE 3900
#define STREAM_BUFFER_CAPACITY 4194304
ThrashVertexV1 vertexData[VBO_SIZE];

ThrashBuffer buffer;

namespace Buffer
{
	VOID __fastcall Reset()
	{
		GLBufferData(GL_ARRAY_BUFFER, STREAM_BUFFER_CAPACITY, NULL, GL_STREAM_DRAW);

		GLEnableVertexAttribArray(attrCoordsLoc);
		GLVertexAttribPointer(attrCoordsLoc, 4, GL_FLOAT, GL_FALSE, sizeof(ThrashVertexV1), (GLvoid*)0);

		GLEnableVertexAttribArray(attrDiffuseLoc);
		GLVertexAttribPointer(attrDiffuseLoc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ThrashVertexV1), (GLvoid*)16);

		GLEnableVertexAttribArray(attrSpecularLoc);
		GLVertexAttribPointer(attrSpecularLoc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ThrashVertexV1), (GLvoid*)20);

		GLEnableVertexAttribArray(attrTexCoordsLoc);
		GLVertexAttribPointer(attrTexCoordsLoc, 2, GL_FLOAT, GL_FALSE, sizeof(ThrashVertexV1), (GLvoid*)24);

		buffer.offset = 0;
	}

	VOID __fastcall Create()
	{
		buffer.data = vertexData;

		GLGenVertexArrays(1, &buffer.name);
		GLBindVertexArray(buffer.name);

		GLGenBuffers(1, &buffer.vName);
		GLBindBuffer(GL_ARRAY_BUFFER, buffer.vName);
		
		Reset();

		buffer.type = GL_NONE;
	}

	VOID __fastcall Release()
	{
		GLBindVertexArray(NULL);
		GLDeleteVertexArrays(1, &buffer.name);

		GLBindBuffer(GL_ARRAY_BUFFER, NULL);
		GLDeleteBuffers(1, &buffer.vName);
	}

	VOID __fastcall Draw()
	{
		if (buffer.type && buffer.size)
		{
			Texture::CheckWrap();

			DWORD streamDataSize = buffer.size * sizeof(ThrashVertexV1);
			if (buffer.offset + streamDataSize > STREAM_BUFFER_CAPACITY)
				Reset();

			VOID* data = GLMapBufferRange(GL_ARRAY_BUFFER, buffer.offset, streamDataSize, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
			if (data)
				MemoryCopy(data, buffer.data, streamDataSize);
			GLUnmapBuffer(GL_ARRAY_BUFFER);

			GLDrawArrays(buffer.type, buffer.offset / sizeof(ThrashVertexV1), buffer.size);
			buffer.offset += streamDataSize;

			buffer.size = 0;
			buffer.type = GL_NONE;
		}
	}

	VOID __fastcall Check(DWORD type)
	{
		if (type != buffer.type || buffer.size == VBO_SIZE)
		{
			Draw();
			buffer.type = type;
		}
	}

	VOID __fastcall AddVertex(ThrashVertex* vertex)
	{
		ThrashVertexV1* bVertex = &buffer.data[buffer.size++];
		*bVertex = *(ThrashVertexV1*)vertex;
		bVertex->vertCoord.z += depthBias;
	}

	VOID __fastcall AddPoint(ThrashVertex* vertex)
	{
		Check(GL_POINTS);
		AddVertex(vertex);
	}

	VOID __fastcall AddLine(ThrashVertex* vertex1, ThrashVertex* vertex2)
	{
		Check(GL_LINES);
		AddVertex(vertex1);
		AddVertex(vertex2);
	}

	VOID __fastcall AddTri(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3)
	{
		BOOL res = TRUE;
		if (cullFace)
		{
			ThrashVertexV1* v1 = (ThrashVertexV1*)vertex1;
			ThrashVertexV1* v2 = (ThrashVertexV1*)vertex2;
			ThrashVertexV1* v3 = (ThrashVertexV1*)vertex3;

			FLOAT check = (v3->vertCoord.y - v1->vertCoord.y) * (v2->vertCoord.x - v1->vertCoord.x)
				- (v2->vertCoord.y - v1->vertCoord.y) * (v3->vertCoord.x - v1->vertCoord.x);

			if (cullFace == 1)
				res = check > 0.0; // GL_CW
			else
				res = check < 0.0; // GL_CCW
		}

		if (res)
		{
			Check(GL_TRIANGLES);

			AddVertex(vertex1);
			AddVertex(vertex2);
			AddVertex(vertex3);
		}
	}
}