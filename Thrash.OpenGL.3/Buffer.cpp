#include "stdafx.h"
#include "Global.hpp"

const DWORD VBO_SIZE = 3900;
ThrashVertexV1 vertexData[VBO_SIZE];
WORD indexData[VBO_SIZE];

ThrashBuffer buffer;

namespace Buffer
{
	VOID __fastcall Create()
	{
		if (forced.indexedVBO)
			buffer.iData = indexData;
		buffer.vData = vertexData;

		GLGenVertexArrays(1, &buffer.name);
		GLBindVertexArray(buffer.name);

		GLGenBuffers(1, &buffer.iName);
		GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.iName);
		GLBufferData(GL_ELEMENT_ARRAY_BUFFER, VBO_SIZE * sizeof(WORD), NULL, GL_STREAM_DRAW);

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

		GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
		GLDeleteBuffers(1, &buffer.iName);

		GLBindBuffer(GL_ARRAY_BUFFER, NULL);
		GLDeleteBuffers(1, &buffer.vName);
	}

	VOID __fastcall Draw()
	{
		if (buffer.iSize && buffer.type)
		{
			Texture::CheckWrap();

			GLBufferSubData(GL_ARRAY_BUFFER, 0, buffer.vSize * sizeof(ThrashVertexV1), buffer.vData);

			if (buffer.isIndexed)
			{
				GLBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, buffer.iSize * sizeof(WORD), buffer.iData);
				GLDrawElements(buffer.type, buffer.iSize, GL_UNSIGNED_SHORT, NULL);
			}
			else
				GLDrawArrays(buffer.type, 0, buffer.iSize);

			buffer.iSize = buffer.vSize = 0;
			buffer.isIndexed = FALSE;
			buffer.type = GL_NONE;
		}
	}

	BOOL __fastcall Check(DWORD type)
	{
		if (type != buffer.type || buffer.iSize == VBO_SIZE)
		{
			Draw();
			buffer.type = type;
			return TRUE;
		}
		else return FALSE;
	}

	DWORD __fastcall AddVertex(ThrashVertex* vertex)
	{
		ThrashVertexV1* bVertex = &buffer.vData[buffer.vSize];
		*bVertex = *(ThrashVertexV1*)vertex;
		bVertex->vertCoord.z += depthBias;

		if (forced.indexedVBO)
			buffer.iData[buffer.iSize] = buffer.vSize;

		++buffer.iSize;

		return buffer.vSize++;
	}

	VOID __fastcall AddIndex(DWORD index)
	{
		if (forced.indexedVBO)
		{
			buffer.iData[buffer.iSize] = index;
			buffer.isIndexed = TRUE;
		}
		else
			buffer.vData[buffer.vSize++] = buffer.vData[index];

		++buffer.iSize;
	}
}