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

namespace State
{
	VOID __fastcall InternalSet(ThrashState key, DWORD tmu, DWORD value)
	{
		INT index = GetKeyIndex(key);
		if (index >= 0)
			stateValueArray[tmu][index] = value;
	}

	DWORD __fastcall Set(ThrashState key, DWORD tmu, DWORD value)
	{
		DWORD res = 1;

		BOOL force = tmu & 0xFFFF0000;
		tmu &= 0x0000FFFF;

		DWORD* stateStorage = NULL;
		INT index = GetKeyIndex(key);
		if (index >= 0)
			stateStorage = &stateValueArray[tmu][index];

		if (key == DrawPrimitives)
		{
			ThrashPrimitiveIndexed* indexedList = (ThrashPrimitiveIndexed*)value;
			specularEnabled = indexedList->type & 0x080; // D3DFVF_SPECULAR
			if (indexedList->indices && indexedList->indicesCount)
			{
				switch (indexedList->type)
				{
				case PT_POINTLIST:
					Vertex::DrawIndexedArray(GL_POINTS, 0, indexedList->indicesCount, indexedList->vertexArray, indexedList->indices);
					break;
				case PT_LINELIST:
					Vertex::DrawIndexedArray(GL_LINES, 0, indexedList->indicesCount, indexedList->vertexArray, indexedList->indices);
					break;
				case PT_LINESTRIP:
					Vertex::DrawIndexedArray(GL_LINE_STRIP, 0, indexedList->indicesCount, indexedList->vertexArray, indexedList->indices);
					break;
				case PT_TRIANGLELIST:
					Vertex::DrawIndexedArray(GL_TRIANGLES, 0, indexedList->indicesCount, indexedList->vertexArray, indexedList->indices);
					break;
				case PT_TRIANGLESTRIP:
					Vertex::DrawIndexedArray(GL_TRIANGLE_STRIP, 0, indexedList->indicesCount, indexedList->vertexArray, indexedList->indices);
					break;
				case PT_TRIANGLEFAN:
					Vertex::DrawIndexedArray(GL_TRIANGLE_FAN, 0, indexedList->indicesCount, indexedList->vertexArray, indexedList->indices);
					break;
				default:
					return NULL;
				}
			}
			else
			{
				ThrashPrimitiveList* list = (ThrashPrimitiveList*)value;
				switch (list->type)
				{
				case PT_POINTLIST:
					Vertex::DrawArray(GL_POINTS, 0, list->vertexCount, list->vertexArray);
					break;
				case PT_LINELIST:
					Vertex::DrawArray(GL_LINES, 0, list->vertexCount, list->vertexArray);
					break;
				case PT_LINESTRIP:
					Vertex::DrawArray(GL_LINE_STRIP, 0, list->vertexCount, list->vertexArray);
					break;
				case PT_TRIANGLELIST:
					Vertex::DrawArray(GL_TRIANGLES, 0, list->vertexCount, list->vertexArray);
					break;
				case PT_TRIANGLESTRIP:
					Vertex::DrawArray(GL_TRIANGLE_STRIP, 0, list->vertexCount, list->vertexArray);
					break;
				case PT_TRIANGLEFAN:
					Vertex::DrawArray(GL_TRIANGLE_FAN, 0, list->vertexCount, list->vertexArray);
					break;
				default:
					return NULL;
				}
			}
		}
		else
		{
			if (!force && stateStorage && *stateStorage == value)
				return 1;

			switch (key)
			{
#pragma region Texture
			case SetTexture:
				texturesEnabled = value >= MIN_TEX_ADDRESS;

				if (texturesEnabled)
					GLEnable(GL_TEXTURE_2D);
				else
					GLDisable(GL_TEXTURE_2D);

				Texture::Bind((ThrashTexture*)value);

				break;

			case TextureMipMap:
				textureMipMap = value;
				break;

			case TextureFilter:
			case Texture0Filter:
				textureFilterEnabled = value;
				break;

			case TextureLodBias:
				GLTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, *(GLfloat*)&value);
				break;

			case TexturesCombine:
				if (glVersion < GL_VER_1_3)
					GLTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				else
				{
					switch (value)
					{
					case 0:
						GLTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
						break;

					case 1:
						GLTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

						GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD);
						GLTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE, 1);
						GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
						GLTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);
						GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
						GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);

						GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_ADD);
						GLTexEnvi(GL_TEXTURE_ENV, GL_ALPHA_SCALE, 1);
						GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
						GLTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, GL_PRIMARY_COLOR);
						GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
						GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
						break;

						/*case 2:
							GLTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

							GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE, 1);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_PRIMARY_COLOR);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);

							GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_ALPHA_SCALE, 1);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_PRIMARY_COLOR);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
							break;*/

					case 3:
						GLTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

						GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
						GLTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE, 1);
						GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
						GLTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);
						GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
						GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);

						GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
						GLTexEnvi(GL_TEXTURE_ENV, GL_ALPHA_SCALE, 1);
						GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
						GLTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, GL_PRIMARY_COLOR);
						GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
						GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
						break;

						/*case 4:
							GLTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

							GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_SUBTRACT);
							GLTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE, 1);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);

							GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_ALPHA_SCALE, 1);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
							break;

						case 5:
							GLTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

							GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE, 1);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_PREVIOUS);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);

							GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_ADD_SIGNED);
							GLTexEnvi(GL_TEXTURE_ENV, GL_ALPHA_SCALE, 1);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, GL_PREVIOUS);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
							break;

						case 6:
							GLTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

							GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE, 2);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);

							GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_ALPHA_SCALE, 2);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, GL_PRIMARY_COLOR);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
							break;

						case 7:
							GLTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

							GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE, 4);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);

							GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_ALPHA_SCALE, 4);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, GL_PRIMARY_COLOR);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
							break;

						case 8:
							GLTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

							GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE, 1);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PRIMARY_COLOR);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);

							GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_ALPHA_SCALE, 1);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, GL_PRIMARY_COLOR);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
							break;

						case 97:
							GLTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

							GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD_SIGNED);
							GLTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE, 1);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);

							GLTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_ADD_SIGNED);
							GLTexEnvi(GL_TEXTURE_ENV, GL_ALPHA_SCALE, 1);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
							GLTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, GL_PRIMARY_COLOR);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
							GLTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
							break;*/

							/*case 98:
								GLTexEnvi(GL_TEXTURE_ENV, D3DTSS_COLOROP, D3DTOP_ADD);
								GLTexEnvi(GL_TEXTURE_ENV, D3DTSS_COLORARG1, D3DTA_TEXTURE);
								GLTexEnvi(GL_TEXTURE_ENV, D3DTSS_COLORARG2, D3DTA_CURRENT);

								GLTexEnvi(GL_TEXTURE_ENV, D3DTSS_ALPHAOP, D3DTOP_BLENDFACTORALPHA);
								GLTexEnvi(GL_TEXTURE_ENV, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
								GLTexEnvi(GL_TEXTURE_ENV, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
								break;

							case 99:
								GLTexEnvi(GL_TEXTURE_ENV, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
								GLTexEnvi(GL_TEXTURE_ENV, D3DTSS_COLORARG1, D3DTA_TEXTURE);
								GLTexEnvi(GL_TEXTURE_ENV, D3DTSS_COLORARG2, D3DTA_CURRENT);

								GLTexEnvi(GL_TEXTURE_ENV, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
								GLTexEnvi(GL_TEXTURE_ENV, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
								GLTexEnvi(GL_TEXTURE_ENV, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
								break;*/

					default:
						return NULL;
					}
				}

				break;

#pragma endregion
#pragma region Texture clamp
			case TextureClamp:
				switch (value)
				{
				case 0:
					if (glCapsClampToEdge)
						textureClampS = textureClampT = GL_CLAMP_TO_EDGE;
					else
						textureClampS = textureClampT = GL_CLAMP;
					break;

				case 1:
					textureClampS = textureClampT = GL_REPEAT;
					break;

				case 2:
					if (glCapsMirroredRepeat)
						textureClampS = textureClampT = GL_MIRRORED_REPEAT;
					else
						return NULL;
					break;

				default:
					return NULL;
				}

				break;

			case TextureClampS:
				switch (value)
				{
				case 0:
					if (glCapsClampToEdge)
						textureClampS = GL_CLAMP_TO_EDGE;
					else
						textureClampS = GL_CLAMP;
					break;

				case 1:
					textureClampS = GL_REPEAT;
					break;

				case 2:
					if (glCapsMirroredRepeat)
						textureClampS = GL_MIRRORED_REPEAT;
					else
						return NULL;
					break;

				default:
					return NULL;
				}

				break;

			case TextureClampT:
				switch (value)
				{
				case 0:
					if (glCapsClampToEdge)
						textureClampT = GL_CLAMP_TO_EDGE;
					else
						textureClampT = GL_CLAMP;
					break;

				case 1:
					textureClampT = GL_REPEAT;
					break;

				case 2:
					if (glCapsMirroredRepeat)
						textureClampT = GL_MIRRORED_REPEAT;
					else
						return NULL;
					break;

				default:
					return NULL;
				}

				break;

#pragma endregion
#pragma region Vertex
			case VertexLayout:
				Set(VertexSize, !value ? sizeof(ThrashVertexV1) : sizeof(ThrashVertexV2));
				break;

			case VertexSize:
				switch (value)
				{
				case sizeof(ThrashVertexV1) :
					extendedVertex = FALSE;
					break;

				case sizeof(ThrashVertexV2) :
					extendedVertex = TRUE;
					break;

				default:
					return NULL;
				}
				break;

#pragma endregion
#pragma region Depth Buffer
			case EnableDepthBuffer:
				switch (value)
				{
				case 0:
					depthEnabled = FALSE;
					GLDisable(GL_DEPTH_TEST);
					break;

				case 1:
					depthEnabled = TRUE;
					GLEnable(GL_DEPTH_TEST);
					GLDepthFunc(depthCmp);
					break;

				default:
					return NULL;
				}

				break;

			case DepthCompare:
				switch (value)
				{
				case 0:
					depthCmp = GL_NEVER;
					break;

				case 1:
					depthCmp = GL_LESS;
					break;

				case 2:
					depthCmp = GL_EQUAL;
					break;

				case 3:
					depthCmp = GL_LEQUAL;
					break;

				case 4:
					depthCmp = GL_GREATER;
					break;

				case 5:
					depthCmp = GL_NOTEQUAL;
					break;

				case 6:
					depthCmp = GL_GEQUAL;
					break;

				case 7:
					depthCmp = GL_ALWAYS;
					break;

				default:
					return NULL;
				}

				GLDepthFunc(depthCmp);
				break;

			case DepthBias:
			case DepthBias2:
				depthBias = (FLOAT)*(INT*)&value * DEPTH_CORRECTION;

				break;

			case EnableDepthWrite:
			case EnableDepthWrite2:
				GLDepthMask((GLboolean)value);
				break;

#pragma endregion
#pragma region Alpha & Blend
			case EnableAlphaBlend:
				switch (value)
				{
				case 0:
				case 1:
					GLDisable(GL_ALPHA_TEST);
					GLDisable(GL_BLEND);
					break;

				case 2:
				case 3:
					GLEnable(GL_ALPHA_TEST);
					GLAlphaFunc(alphaCmp, alphaVal);

					GLEnable(GL_BLEND);
					GLBlendFunc(blendSrc, blendDest);
					break;

				default:
					return NULL;
				}

				break;

			case AlphaMode:
				alphaVal = value != 0 ? (FLOAT)value / FLOAT_255 : 0.0f;
				GLAlphaFunc(alphaCmp, alphaVal);
				break;

			case AlphaCompare:
				switch (value)
				{
				case 0:
					alphaCmp = GL_NEVER;
					break;

				case 1:
					alphaCmp = GL_LESS;
					break;

				case 2:
					alphaCmp = GL_EQUAL;
					break;

				case 3:
					alphaCmp = GL_LEQUAL;
					break;

				case 4:
					alphaCmp = GL_GREATER;
					break;

				case 5:
					alphaCmp = GL_NOTEQUAL;
					break;

				case 6:
					alphaCmp = GL_GEQUAL;
					break;

				case 7:
					alphaCmp = GL_ALWAYS;
					break;

				default:
					return NULL;
				}

				GLAlphaFunc(alphaCmp, alphaVal);
				break;

			case BlendMode:
			case BlendMode2:
				switch (value)
				{
				case 0:
					blendSrc = GL_SRC_ALPHA;
					blendDest = GL_ONE_MINUS_SRC_ALPHA;
					InternalSet(BlendSourceFactor, tmu, 2);
					InternalSet(BlendDestinationFactor, tmu, 3);
					break;
				case 1:
					blendSrc = GL_SRC_ALPHA;
					blendDest = GL_ONE;
					InternalSet(BlendSourceFactor, tmu, 2);
					InternalSet(BlendDestinationFactor, tmu, 0);
					break;
				case 2:
					blendSrc = GL_ZERO;
					blendDest = GL_ONE_MINUS_SRC_ALPHA;
					InternalSet(BlendSourceFactor, tmu, 1);
					InternalSet(BlendDestinationFactor, tmu, 3);
					break;
				case 3:
					InternalSet(BlendSourceFactor, tmu, 7);
					InternalSet(BlendDestinationFactor, tmu, 1);
					blendSrc = GL_DST_COLOR;
					blendDest = GL_ZERO;
					break;
				default:
					return NULL;
				}

				GLBlendFunc(blendSrc, blendDest);
				break;

			case BlendSourceFactor:
				switch (value)
				{
				case 0:
					blendSrc = GL_ONE;
					break;
				case 1:
					blendSrc = GL_ZERO;
					break;
				case 2:
					blendSrc = GL_SRC_ALPHA;
					break;
				case 3:
					blendSrc = GL_ONE_MINUS_SRC_ALPHA;
					break;
				case 4:
					blendSrc = GL_DST_ALPHA;
					break;
				case 5:
					blendSrc = GL_ONE_MINUS_DST_ALPHA;
					break;
				case 6:
					blendSrc = GL_SRC_COLOR;
					break;
				case 7:
					blendSrc = GL_DST_COLOR;
					break;
				case 8:
					blendSrc = GL_ONE_MINUS_SRC_COLOR;
					break;
				case 9:
					blendSrc = GL_ONE_MINUS_DST_COLOR;
					break;
				default:
					return NULL;
				}

				GLBlendFunc(blendSrc, blendDest);
				InternalSet(BlendMode, tmu, 4);
				InternalSet(BlendMode2, tmu, 4);

				break;

			case BlendDestinationFactor:
				switch (value)
				{
				case 0:
					blendDest = GL_ONE;
					break;
				case 1:
					blendDest = GL_ZERO;
					break;
				case 2:
					blendDest = GL_SRC_ALPHA;
					break;
				case 3:
					blendDest = GL_ONE_MINUS_SRC_ALPHA;
					break;
				case 4:
					blendDest = GL_DST_ALPHA;
					break;
				case 5:
					blendDest = GL_ONE_MINUS_DST_ALPHA;
					break;
				case 6:
					blendDest = GL_SRC_COLOR;
					break;
				case 7:
					blendDest = GL_DST_COLOR;
					break;
				case 8:
					blendDest = GL_ONE_MINUS_SRC_COLOR;
					break;
				case 9:
					blendDest = GL_ONE_MINUS_DST_COLOR;
					break;
				default:
					return NULL;
				}

				GLBlendFunc(blendSrc, blendDest);
				InternalSet(BlendMode, tmu, 4);
				InternalSet(BlendMode2, tmu, 4);

				break;

#pragma endregion
#pragma region Fog
			case EnableFog:
			case EnableFog2:
			case FogMode:
				switch (value)
				{
				case Disabled:
					fogEnabled = FALSE;
					GLDisable(GL_FOG);
					break;

				case Enabled:
					fogEnabled = TRUE;
					GLEnable(GL_FOG);
					if (GLFogCoordf)
						GLFogi(GL_FOG_COORD_SRC, GL_FOG_COORD);
					break;

				case Linear:
					GLFogi(GL_FOG_MODE, GL_LINEAR);
					break;

				case Exp:
					GLFogi(GL_FOG_MODE, GL_EXP);
					break;

				case Exp2:
					GLFogi(GL_FOG_MODE, GL_EXP2);
					break;

				default:
					return NULL;
				}

				break;

			case FogColor:
			{
				GLfloat color[4];
				color[2] = (FLOAT)(UINT8)value / FLOAT_255;
				color[1] = (FLOAT)(UINT8)(value >> 8) / FLOAT_255;
				color[0] = (FLOAT)(UINT8)(value >> 16) / FLOAT_255;
				color[3] = (FLOAT)(UINT8)(value >> 24) / FLOAT_255;
				GLFogfv(GL_FOG_COLOR, color);

				break;
			}

			case FogStart:
				GLFogf(GL_FOG_START, value & 0xFFFF0000 ? *(GLfloat*)&value * FLOAT_65536 : (GLfloat)value);
				break;

			case FogEnd:
				GLFogf(GL_FOG_END, value & 0xFFFF0000 ? *(GLfloat*)&value * FLOAT_65536 : (GLfloat)value);
				break;

			case FogDensity:
				if (!value || value & 0xFFFFF000)
					GLFogf(GL_FOG_DENSITY, *(GLfloat*)&value);
				else
					GLFogf(GL_FOG_DENSITY, GLfloat(1.0 / value));

				break;

#pragma endregion
#pragma region Functions
			case Functions:
				if (value)
					memcpy(&functions, (DWORD*)value, sizeof(ThrashFunctions));
				else
					memset(&functions, NULL, sizeof(ThrashFunctions));

				break;

			case MessageBoxFunction:
				functions.ErrorMessageBox = (DWORD(__stdcall *)(DWORD, LPCSTR))value;
				break;

			case EventFuncion:
				functions.Event = (VOID(__stdcall *)(UINT, PROC))value;
				break;

			case LockStatusFunction:
				functions.LockStatus = (VOID(__stdcall *)(BOOL))value;
				break;

			case MallocFunction:
				functions.Malloc = (VOID* (__stdcall *)(size_t))value;
				break;

			case FreeFunction:
				functions.Free = (VOID(__stdcall *)(VOID*))value;
				break;

			case StateFunction:
				functions.State = (DWORD(__stdcall *)(DWORD, DWORD))value;
				break;

#pragma endregion
#pragma region Stencil buffer
			case EnableStencilBuffer:
				if (value)
				{
					GLEnable(GL_STENCIL_TEST);
					GLStencilFunc(stencilFunc, 2, 0xFF);
					GLStencilOp(stencilFail, stencilDepthFail, stencilPass);
				}
				else
					GLDisable(GL_STENCIL_TEST);

				stencilEnabled = value;
				break;

			case StencilFunc:
				switch (value)
				{
				case 0:
					stencilFunc = GL_NEVER;
					break;

				case 1:
					stencilFunc = GL_LESS;
					break;

				case 2:
					stencilFunc = GL_EQUAL;
					break;

				case 3:
					stencilFunc = GL_LEQUAL;
					break;

				case 4:
					stencilFunc = GL_GREATER;
					break;

				case 5:
					stencilFunc = GL_NOTEQUAL;
					break;

				case 6:
					stencilFunc = GL_GEQUAL;
					break;

				case 7:
					stencilFunc = GL_ALWAYS;
					break;

				default:
					return NULL;
				}

				GLStencilFunc(stencilFunc, 2, 0xFF);

				break;

			case StencilFail:
				switch (value)
				{
				case 0:
					stencilFail = GL_KEEP;
					break;

				case 1:
					stencilFail = GL_ZERO;
					break;

				case 2:
					stencilFail = GL_REPLACE;
					break;

				case 3:
					stencilFail = GL_INCR;
					break;

				case 4:
					stencilFail = GL_DECR;
					break;

				case 5:
					stencilFail = GL_INVERT;
					break;

				case 6:
					stencilFail = GL_INCR_WRAP;
					break;

				case 7:
					stencilFail = GL_DECR_WRAP;
					break;

				default:
					return NULL;
				}

				GLStencilOp(stencilFail, stencilDepthFail, stencilPass);
				break;

			case StencilDepthFail:
				switch (value)
				{
				case 0:
					stencilDepthFail = GL_KEEP;
					break;

				case 1:
					stencilDepthFail = GL_ZERO;
					break;

				case 2:
					stencilDepthFail = GL_REPLACE;
					break;

				case 3:
					stencilDepthFail = GL_INCR;
					break;

				case 4:
					stencilDepthFail = GL_DECR;
					break;

				case 5:
					stencilDepthFail = GL_INVERT;
					break;

				case 6:
					stencilDepthFail = GL_INCR_WRAP;
					break;

				case 7:
					stencilDepthFail = GL_DECR_WRAP;
					break;

				default:
					return NULL;
				}

				GLStencilOp(stencilFail, stencilDepthFail, stencilPass);
				break;

			case StencilPass:
				switch (value)
				{
				case 0:
					stencilPass = GL_KEEP;
					break;

				case 1:
					stencilPass = GL_ZERO;
					break;

				case 2:
					stencilPass = GL_REPLACE;
					break;

				case 3:
					stencilPass = GL_INCR;
					break;

				case 4:
					stencilPass = GL_DECR;
					break;

				case 5:
					stencilPass = GL_INVERT;
					break;

				case 6:
					stencilPass = GL_INCR_WRAP;
					break;

				case 7:
					stencilPass = GL_DECR_WRAP;
					break;

				default:
					return NULL;
				}

				GLStencilOp(stencilFail, stencilDepthFail, stencilPass);
				break;

#pragma endregion
#pragma region Others
			case CullFace:
				switch (value)
				{
				case 0:
					GLDisable(GL_CULL_FACE);
					break;

				case 1:
					GLEnable(GL_CULL_FACE);
					GLFrontFace(GL_CW);
					break;

				case 2:
					GLEnable(GL_CULL_FACE);
					GLFrontFace(GL_CCW);
					break;

				default:
					return NULL;
				}

				break;

			case ClearColor:
				GLClearColor(
					GLclampf((FLOAT)(UINT8)(value >> 16) / FLOAT_255),
					GLclampf((FLOAT)(UINT8)(value >> 8) / FLOAT_255),
					GLclampf((FLOAT)(UINT8)(value) / FLOAT_255),
					GLclampf((FLOAT)(UINT8)(value >> 24) / FLOAT_255));

				break;

			case ClearDepth:
				GLClearDepth(*(FLOAT*)&value);
				break;

			case LineWidth:
				GLLineWidth((FLOAT)value);
				break;

			case EnableDither:
				if (value)
					GLEnable(GL_DITHER);
				else
					GLDisable(GL_DITHER);

				break;

			case ShadeModel:
				switch (value)
				{
				case 0:
					GLShadeModel(GL_FLAT);
					specularEnabled = FALSE;
					break;

				case 1:
					GLShadeModel(GL_SMOOTH);
					specularEnabled = FALSE;
					break;

				case 2:
					GLShadeModel(GL_SMOOTH);
					specularEnabled = TRUE;
					break;

				default:
					return NULL;
				}
				break;

			case EnableSmoothPolygon:
				switch (value)
				{
				case 0:
					GLDisable(GL_POINT_SMOOTH);
					GLDisable(GL_LINE_SMOOTH);
					GLDisable(GL_POLYGON_SMOOTH);
					break;

				case 1:
					GLEnable(GL_POINT_SMOOTH);
					GLEnable(GL_LINE_SMOOTH);
					GLEnable(GL_POLYGON_SMOOTH);
					break;

				default:
					return NULL;
				}

				break;

			case WindowHandle:
				hWnd = (HWND)value;
				break;

			case CurrentWindow:
				if (!value && !force)
					return NULL;
				break;

			case ThrashApiVersion:
				API_VERSION = value;
				break;

			case BufferMode:
				bufferMode = value;
				break;

			case Gamma:
			case Gamma2:
				GammaRamp::Set(*(FLOAT*)&value);
				break;

				/*case Hint:
					if (value)
					{
						GLHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
						GLHint(GL_FOG_HINT, GL_NICEST);
						GLHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
						GLHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
						GLHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
					}
					else
					{
						GLHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
						GLHint(GL_FOG_HINT, GL_FASTEST);
						GLHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
						GLHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
						GLHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
					}

					break;*/

#pragma endregion

			default: break;
			}
		}

		if (stateStorage)
			*stateStorage = value;

		if (functions.State)
			functions.State(index, value);

		return res;
	}
}