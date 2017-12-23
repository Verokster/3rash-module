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
#include "math.h"

BOOL specularEnabled;
DWORD fogMode;

namespace State
{
	ThrashState drawStates[] = {
		SetTexture,
		TextureMipMap,
		TextureFilter, Texture0Filter,
		TextureLodBias,
		TextureClamp,
		TextureClampS,
		TextureClampT,
		EnableDepthBuffer,
		DepthBias, DepthBias2,
		DepthCompare,
		EnableDepthWrite, EnableDepthWrite2,
		EnableAlphaBlend,
		AlphaValue,
		AlphaCompare,
		BlendMode, BlendMode2,
		BlendSourceFactor,
		BlendDestinationFactor,
		EnableFog, FogTable, FogMode,
		FogColor,
		FogStart,
		FogEnd,
		FogDensity,
		ClearColor,
		ClearDepth,
		DitherMode,
		ShadeModel,
		EnableSmoothPolygon,
		Gamma, Gamma2
	};

	VOID __fastcall SetSpecular(BOOL value)
	{
		if (specularEnabled != value)
		{
			specularEnabled = value;
			GLUniform1ui(uniSpecularEnabledLoc, value);
		}
	}

	VOID __fastcall InternalSet(ThrashState key, DWORD tmu, DWORD value)
	{
		INT index = GetKeyIndex(key);
		if (index >= 0)
			stateValueArray[tmu][index] = value;
	}

	DWORD __fastcall Set(ThrashState key, DWORD tmu, DWORD value)
	{
		BOOL force = tmu & 0xFFFF0000;
		tmu &= 0x0000FFFF;

		DWORD* stateStorage = NULL;
		INT index = GetKeyIndex(key);
		if (index >= 0)
			stateStorage = &stateValueArray[tmu][index];

		if (key == DrawPrimitives)
		{
			ThrashPrimitiveIndexed* indexedList = (ThrashPrimitiveIndexed*)value;

			SetSpecular(indexedList->type & 0x080); // D3DFVF_SPECULAR
			if (indexedList->indices && indexedList->indicesCount)
			{
				switch (indexedList->type)
				{
				case PT_POINTLIST:
					Point::DrawMesh(indexedList->indicesCount, indexedList->vertexArray, indexedList->indices);
					break;
				case PT_LINELIST:
					Line::DrawMesh(indexedList->indicesCount / 2, indexedList->vertexArray, indexedList->indices);
					break;
				case PT_LINESTRIP:
					Line::DrawStrip(indexedList->indicesCount - 1, indexedList->vertexArray, indexedList->indices);
					break;
				case PT_TRIANGLELIST:
					Tri::DrawMesh(indexedList->indicesCount / 3, indexedList->vertexArray, indexedList->indices);
					break;
				case PT_TRIANGLESTRIP:
					Tri::DrawStrip(indexedList->indicesCount - 2, indexedList->vertexArray, indexedList->indices);
					break;
				case PT_TRIANGLEFAN:
					Tri::DrawFan(indexedList->indicesCount - 2, indexedList->vertexArray, indexedList->indices);
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
					Point::DrawStrip(list->vertexCount, list->vertexArray);
					break;
				case PT_LINELIST:
					Line::DrawMesh(list->vertexCount / 2, list->vertexArray);
					break;
				case PT_LINESTRIP:
					Line::DrawStrip(list->vertexCount - 1, list->vertexArray);
					break;
				case PT_TRIANGLELIST:
					Tri::DrawMesh(list->vertexCount / 3, list->vertexArray);
					break;
				case PT_TRIANGLESTRIP:
					Tri::DrawStrip(list->vertexCount - 2, list->vertexArray);
					break;
				case PT_TRIANGLEFAN:
					Tri::DrawFan(list->vertexCount - 2, list->vertexArray);
					break;
				default:
					return NULL;
				}
			}
		}
		else
		{
			if (stateStorage && !force)
			{
				BOOL found = FALSE;
				for (DWORD i = 0; i < sizeof(drawStates) / sizeof(ThrashState); ++i)
				{
					if (drawStates[i] == key)
					{
						if (*stateStorage != value)
							Buffer::Draw();
						else
							return 1;

						found = TRUE;
						break;
					}
				}

				if (!found && *stateStorage == value)
					return 1;
			}

			switch (key)
			{
#pragma region Texture
			case SetTexture:
				texturesEnabled = value >= MIN_TEX_ADDRESS;

				if (texturesEnabled)
				Texture::CheckPallete((ThrashTexture*)value);

				GLUniform1ui(uniTexEnabledLoc, texturesEnabled);
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
				textureLodBias = *(FLOAT*)&value;
				break;

#pragma endregion
#pragma region Texture clamp
			case TextureClamp:
				switch (value)
				{
				case 0:
					textureClampS = textureClampT = GL_CLAMP_TO_EDGE;
					break;

				case 1:
					textureClampS = textureClampT = GL_REPEAT;
					break;

				case 2:
					textureClampS = textureClampT = GL_MIRRORED_REPEAT;
					break;

				default:
					return NULL;
				}

				break;

			case TextureClampS:
				switch (value)
				{
				case 0:
					textureClampS = GL_CLAMP_TO_EDGE;
					break;

				case 1:
					textureClampS = GL_REPEAT;
					break;

				case 2:
					textureClampS = GL_MIRRORED_REPEAT;
					break;

				default:
					return NULL;
				}

				break;

			case TextureClampT:
				switch (value)
				{
				case 0:
					textureClampT = GL_CLAMP_TO_EDGE;
					break;

				case 1:
					textureClampT = GL_REPEAT;
					break;

				case 2:
					textureClampT = GL_MIRRORED_REPEAT;
					break;

				default:
					return NULL;
				}

				break;

#pragma endregion
#pragma region Vertex
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
				GLDepthMask((BOOL)value);
				break;

#pragma endregion
#pragma region Alpha & Blend
			case EnableAlphaBlend:
				if (API_VERSION >= 107)
				{
					switch (value)
					{
					case 0:
					case 1:
						GLUniform1ui(uniAlphaFuncLoc, 7);
						GLDisable(GL_BLEND);
						break;

					case 2:
					case 3:
						GLUniform1ui(uniAlphaFuncLoc, 4);

						GLEnable(GL_BLEND);
						GLBlendFunc(blendSrc, blendDest);
						break;

					default:
						return NULL;
					}
				}
				else
				{
					switch (value)
					{
					case 0:
						GLUniform1ui(uniAlphaFuncLoc, 7);
						GLDisable(GL_BLEND);
						break;

					case 1:
					case 2:
						GLUniform1ui(uniAlphaFuncLoc, 4);

						GLEnable(GL_BLEND);
						GLBlendFunc(blendSrc, blendDest);
						break;

					default:
						return NULL;
					}
				}

				break;

			case AlphaValue:
				GLUniform1f(uniAlphaValLoc, value != 0 ? (FLOAT)value / FLOAT_255 : 0.0f);
				break;

			case AlphaCompare:
				GLUniform1ui(uniAlphaFuncLoc, alphaCmp);
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
			case FogTable:
			case FogMode:
				if (value <= 1)
					GLUniform1ui(uniFogModeLoc, value ? fogMode : value);
				else
				{
					fogMode = value;
					GLUniform1ui(uniFogModeLoc, fogMode);
				}
				break;

			case FogColor:
				GLUniform4f(uniFogColorLoc,
					GLfloat((UINT8)(value >> 16) / FLOAT_255),
					GLfloat((UINT8)(value >> 8) / FLOAT_255),
					GLfloat((UINT8)(value) / FLOAT_255),
					GLfloat((UINT8)(value >> 24) / FLOAT_255));

				break;

			case FogStart:
				GLUniform1f(uniFogStartLoc, (FLOAT)value);
				break;

			case FogEnd:
				GLUniform1f(uniFogEndLoc, (FLOAT)value);
				break;

			case FogDensity:
				GLUniform1f(uniFogDensityLoc, value == 0 || value & 0xFFFFF000 ? *(GLfloat*)&value : (GLfloat)(1.0 / value));
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
				if (value >= 0 && value <= 2)
					cullFace = value;
				else
					return NULL;

				break;

			case ClearColor:
			{
				FLOAT tempFloat = 1.0f / (gamma * forced.gamma);
				GLClearColor(
					GLclampf(pow((UINT8)(value >> 16) / FLOAT_255, tempFloat)),
					GLclampf(pow((UINT8)(value >> 8) / FLOAT_255, tempFloat)),
					GLclampf(pow((UINT8)(value) / FLOAT_255, tempFloat)),
					GLclampf(pow((UINT8)(value >> 24) / FLOAT_255, tempFloat)));
			}

			break;

			case ClearDepth:
				GLClearDepth(*(FLOAT*)&value);
				break;

			case DitherMode:
				if (value)
					GLEnable(GL_DITHER);
				else
					GLDisable(GL_DITHER);

				break;

			case ShadeModel:
				switch (value)
				{
				case 0:
					GLUniform1ui(uniShadeModelLoc, FALSE);
					SetSpecular(FALSE);
					break;

				case 1:
					GLUniform1ui(uniShadeModelLoc, TRUE);
					SetSpecular(FALSE);
					break;

				case 2:
					GLUniform1ui(uniShadeModelLoc, TRUE);
					SetSpecular(TRUE);
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
				gamma = *(FLOAT*)&value;
				GLUniform1f(uniGammaLoc, gamma * forced.gamma);
				break;

			case Windowed:
				appWindowed = value;
				break;

				/*case Hint:
					if (value)
					{
						GLHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
						GLHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
						GLHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
						GLHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
					}
					else
					{
						GLHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
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

		return 1;
	}
}