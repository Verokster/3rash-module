#include "stdafx.h"
#include "Global.hpp"

namespace Vertex
{
	VOID __fastcall Draw(ThrashVertex* vertex)
	{
		DWORD color = _rotr(_byteswap_ulong(*((DWORD*)&(*vertex).diffuseColor)), 8);
		GLColor4ubv((GLubyte*)&color);

		GLfloat w = (GLfloat)(1.0 / (*vertex).vertCoord.rhw);
		if (fogEnabled)
			GLFogCoordf(w);

		if ((*vertex).vertCoord.z <= 1.0)
		{
			if (texturesEnabled)
				GLTexCoord2f((*vertex).texCoord.u, (*vertex).texCoord.v);

			GLVertex4f((GLfloat)((*vertex).vertCoord.x * w), (GLfloat)((*vertex).vertCoord.y * w), (GLfloat)((*vertex).vertCoord.z * w), w);
		}
		else
		{
			if (texturesEnabled)
				GLTexCoord4f((GLfloat)((*vertex).texCoord.u * (*vertex).vertCoord.rhw), (GLfloat)((*vertex).texCoord.v * (*vertex).vertCoord.rhw), 0, (*vertex).vertCoord.rhw);

			GLVertex2f((*vertex).vertCoord.x, (*vertex).vertCoord.y);
		}
	};

	VOID __fastcall DrawArray(GLenum mode, DWORD step, DWORD count, ThrashVertex vertexArray[])
	{
		if (count > 0)
		{
			Texture::CheckWrap();

			count += step;
			GLBegin(mode);
			{
				do
				{
					Draw(vertexArray++);
					--count;
				} while (count);
			}
			GLEnd();
		}
	}

	VOID __fastcall DrawIndexedArray(GLenum mode, DWORD step, DWORD count, ThrashVertex vertexArray[], DWORD indexes[])
	{
		if (count > 0)
		{
			Texture::CheckWrap();

			count += step;
			GLBegin(mode);
			{
				do
				{
					Draw(&vertexArray[*indexes++]);
					--count;
				} while (count);
			}
			GLEnd();
		}
	}
}