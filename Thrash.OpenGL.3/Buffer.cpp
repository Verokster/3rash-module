#include "stdafx.h"
#include "Global.hpp"

const DWORD VBO_SIZE = 3900;
ThrashBufferVertex vertexData[VBO_SIZE];
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

		if (forced.indexedVBO)
		{
			GLGenBuffers(1, &buffer.iName);
			GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.iName);
			GLBufferData(GL_ELEMENT_ARRAY_BUFFER, VBO_SIZE * sizeof(WORD), NULL, GL_STREAM_DRAW);
		}

		GLGenBuffers(1, &buffer.vName);
		GLBindBuffer(GL_ARRAY_BUFFER, buffer.vName);
		GLBufferData(GL_ARRAY_BUFFER, VBO_SIZE * sizeof(ThrashBufferVertex), NULL, GL_STREAM_DRAW);

		GLEnableVertexAttribArray(attrCoordsLoc);
		GLVertexAttribPointer(attrCoordsLoc, 4, GL_FLOAT, GL_FALSE, sizeof(ThrashBufferVertex), (GLvoid*)0);

		GLEnableVertexAttribArray(attrColorLoc);
		GLVertexAttribPointer(attrColorLoc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ThrashBufferVertex), (GLvoid*)16);

		GLEnableVertexAttribArray(attrTexCoordsLoc);
		GLVertexAttribPointer(attrTexCoordsLoc, 2, GL_FLOAT, GL_FALSE, sizeof(ThrashBufferVertex), (GLvoid*)20);

		GLEnableVertexAttribArray(attrTexUnitLoc);
		GLVertexAttribIPointer(attrTexUnitLoc, 1, GL_UNSIGNED_INT, sizeof(ThrashBufferVertex), (GLvoid*)28);

		buffer.type = GL_NONE;
	}

	VOID __fastcall Release()
	{
		GLBindVertexArray(NULL);
		GLDeleteVertexArrays(1, &buffer.name);

		if (forced.indexedVBO)
		{
			GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
			GLDeleteBuffers(1, &buffer.iName);
		}

		GLBindBuffer(GL_ARRAY_BUFFER, NULL);
		GLDeleteBuffers(1, &buffer.vName);
	}

	VOID __fastcall Draw()
	{
		if (buffer.iSize > 0 && buffer.type != GL_NONE)
		{
			GLBufferSubData(GL_ARRAY_BUFFER, 0, buffer.vSize * sizeof(ThrashBufferVertex), buffer.vData);

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

		Texture::ResetUsed();
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
		ThrashBufferVertex* bVertex = &buffer.vData[buffer.vSize];

		(*bVertex).vertCoord = (*vertex).vertCoord;
		(*bVertex).color = (*vertex).diffuseColor;
		(*bVertex).texCoord = (*vertex).texCoord;
		(*bVertex).texUnit = texturesEnabled ? drawTextureUnit : texturesEnabled;

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