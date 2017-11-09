#include "stdafx.h"
#include "Global.hpp"

#define VBO_SIZE 3900
ThrashVertexV1 vertexData[VBO_SIZE];

ThrashBuffer buffer;

namespace Buffer
{
	VOID __fastcall Create()
	{
		buffer.data = vertexData;

		GLGenVertexArrays(1, &buffer.name);
		GLBindVertexArray(buffer.name);

		GLGenBuffers(1, &buffer.vName);
		GLBindBuffer(GL_ARRAY_BUFFER, buffer.vName);
		GLBufferData(GL_ARRAY_BUFFER, VBO_SIZE * sizeof(ThrashVertexV1), NULL, GL_STREAM_DRAW);

		GLEnableVertexAttribArray(attrCoordsLoc);
		GLVertexAttribPointer(attrCoordsLoc, 4, GL_FLOAT, GL_FALSE, sizeof(ThrashVertexV1), (GLvoid*)0);

		GLEnableVertexAttribArray(attrDiffuseLoc);
		GLVertexAttribPointer(attrDiffuseLoc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ThrashVertexV1), (GLvoid*)16);

		GLEnableVertexAttribArray(attrSpecularLoc);
		GLVertexAttribPointer(attrSpecularLoc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ThrashVertexV1), (GLvoid*)20);

		GLEnableVertexAttribArray(attrTexCoordsLoc);
		GLVertexAttribPointer(attrTexCoordsLoc, 2, GL_FLOAT, GL_FALSE, sizeof(ThrashVertexV1), (GLvoid*)24);

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
		if (buffer.size && buffer.type)
		{
			Texture::CheckWrap();

			GLBufferSubData(GL_ARRAY_BUFFER, 0, buffer.size * sizeof(ThrashVertexV1), buffer.data);

			GLDrawArrays(buffer.type, 0, buffer.size);

			buffer.size = 0;
			buffer.type = GL_NONE;
		}
	}

	BOOL __fastcall Check(DWORD type)
	{
		if (type != buffer.type || buffer.size == VBO_SIZE)
		{
			Draw();
			buffer.type = type;
			return TRUE;
		}
		else return FALSE;
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