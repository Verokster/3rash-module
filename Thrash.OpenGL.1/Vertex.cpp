#include "stdafx.h"
#include "Global.hpp"

namespace Vertex
{
	VOID __fastcall Draw(ThrashVertexV1* vertex)
	{
		DWORD diffuse = Color::Swap(*((DWORD*)&vertex->diffuseColor));
		GLColor4ubv((GLubyte*)&diffuse);

		/*if (specularEnabled && GLSecondaryColor3ubv && *((DWORD*)&vertex->specularColor))
		{
			DWORD specular = Color::Swap(*((DWORD*)&vertex->specularColor));
			GLSecondaryColor3ubv((GLubyte*)&specular);
		}*/

		GLfloat w = (GLfloat)(1.0 / vertex->vertCoord.rhw);
		if (fogEnabled && GLFogCoordf)
			GLFogCoordf(w);

		if (vertex->vertCoord.z <= 1.0)
		{
			if (texturesEnabled)
				GLTexCoord2f(vertex->texCoord0.u, vertex->texCoord0.v);

			GLVertex4f((GLfloat)(vertex->vertCoord.x * w), (GLfloat)(vertex->vertCoord.y * w), (GLfloat)(vertex->vertCoord.z * w), w);
		}
		else
		{
			if (texturesEnabled)
				GLTexCoord4f((GLfloat)(vertex->texCoord0.u * vertex->vertCoord.rhw), (GLfloat)(vertex->texCoord0.v * vertex->vertCoord.rhw), 0.0, vertex->vertCoord.rhw);

			GLVertex2f(vertex->vertCoord.x, vertex->vertCoord.y);
		}
	}

	VOID __fastcall Draw(ThrashVertexV2* vertex)
	{
		DWORD color = Color::Swap(*((DWORD*)&vertex->diffuseColor));
		GLColor4ubv((GLubyte*)&color);

		/*if (specularEnabled && GLSecondaryColor3ubv && *((DWORD*)&vertex->specularColor))
		{
			DWORD specular = Color::Swap(*((DWORD*)&vertex->specularColor));
			GLSecondaryColor3ubv((GLubyte*)&specular);
		}*/

		GLfloat w = (GLfloat)(1.0 / vertex->vertCoord.rhw);
		if (fogEnabled && GLFogCoordf)
			GLFogCoordf(w);

		if (vertex->vertCoord.z <= 1.0)
		{
			if (texturesEnabled)
			{
				GLMultiTexCoord2f(GL_TEXTURE0, vertex->texCoord0.u, vertex->texCoord0.v);
				GLMultiTexCoord2f(GL_TEXTURE1, vertex->texCoord1.u, vertex->texCoord1.v);
			}

			GLVertex4f((GLfloat)(vertex->vertCoord.x * w), (GLfloat)(vertex->vertCoord.y * w), (GLfloat)(vertex->vertCoord.z * w), w);
		}
		else
		{
			if (texturesEnabled)
			{
				GLMultiTexCoord4f(GL_TEXTURE0, (GLfloat)(vertex->texCoord0.u * vertex->vertCoord.rhw), (GLfloat)(vertex->texCoord0.v * vertex->vertCoord.rhw), 0.0, vertex->vertCoord.rhw);
				GLMultiTexCoord4f(GL_TEXTURE1, (GLfloat)(vertex->texCoord1.u * vertex->vertCoord.rhw), (GLfloat)(vertex->texCoord1.v * vertex->vertCoord.rhw), 0.0, vertex->vertCoord.rhw);
			}

			GLVertex2f(vertex->vertCoord.x, vertex->vertCoord.y);
		}
	}

	VOID __fastcall DrawArray(GLenum mode, DWORD step, DWORD count, ThrashVertex vertexArray[])
	{
		if (count > 0)
		{
			Texture::CheckWrap();

			count += step;
			GLBegin(mode);
			{
				if (!extendedVertex)
				{
					ThrashVertexV1* vertex = (ThrashVertexV1*)vertexArray;
					do
					{
						Draw(vertex++);
					} while (--count);
				}
				else
				{
					ThrashVertexV2* vertex = (ThrashVertexV2*)vertexArray;
					do
					{
						Draw(vertex++);
					} while (--count);
				}
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
				if (!extendedVertex)
				{
					ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;
					do
					{
						Draw(&vArray[*indexes++]);
					} while (--count);
				}
				else
				{
					ThrashVertexV2* vArray = (ThrashVertexV2*)vertexArray;
					do
					{
						Draw(&vArray[*indexes++]);
					} while (--count);
				}
			}
			GLEnd();
		}
	}

	VOID __fastcall DrawIndexedArray(GLenum mode, DWORD step, DWORD count, ThrashVertex vertexArray[], WORD indexes[])
	{
		if (count > 0)
		{
			Texture::CheckWrap();

			count += step;
			GLBegin(mode);
			{
				if (!extendedVertex)
				{
					ThrashVertexV1* vArray = (ThrashVertexV1*)vertexArray;
					do
					{
						Draw(&vArray[*indexes++]);
					} while (--count);
				}
				else
				{
					ThrashVertexV2* vArray = (ThrashVertexV2*)vertexArray;
					do
					{
						Draw(&vArray[*indexes++]);
					} while (--count);
				}
			}
			GLEnd();
		}
	}
}