#include "stdafx.h"
#include "Global.hpp"

namespace State
{
	BOOL __stdcall Set(ThrashState key, DWORD value)
	{
		FLOAT tempFloat;

		switch (key)
		{
			case SetTexture:
				texturesEnabled = value;
				Texture::Bind((ThrashTexture*)value);

				break;

			case CullFace:
				if (value >= 0 && value <= 2)
				{
					cullFace = value;
					break;
				}
				else return false;

			case ClearColor:
				Buffer::Draw();

				tempFloat = 1.0 / (*(FLOAT*)&gamma * forced.gamma);
				GLClearColor(
					GLclampf(pow((UINT8)(value >> 16) / FLOAT_255, tempFloat)),
					GLclampf(pow((UINT8)(value >> 8) / FLOAT_255, tempFloat)),
					GLclampf(pow((UINT8)(value) / FLOAT_255, tempFloat)),
					GLclampf(pow((UINT8)(value >> 24) / FLOAT_255, tempFloat)));

				break;

			case EnableDepthBuffer:
				if (value)
				{
					if (value == 1)
					{
						Buffer::Draw();
						GLEnable(GL_DEPTH_TEST);
					}
					else return false;
				}
				else
				{
					Buffer::Draw();
					GLDisable(GL_DEPTH_TEST);
				}

				break;

			case EnableDither:
				Buffer::Draw();

				if (value)
					GLEnable(GL_DITHER);
				else
					GLDisable(GL_DITHER);

				break;

			case ShadeModel:
				switch (value)
				{
					case 0:
						if (shadeModel)
						{
							Buffer::Draw();
							shadeModel = FALSE;
							GLUniform1ui(uniShadeModelLoc, shadeModel);
						}
						break;

					case 1:
					case 2:
						if (!shadeModel)
						{
							Buffer::Draw();
							shadeModel = TRUE;
							GLUniform1ui(uniShadeModelLoc, shadeModel);
						}
						break;

					case 3:
						return false;

					default:
						break;
				}

				break;

			case TextureFilter:
				textureFilterEnabled = value;
				break;

			case EnableSmoothPolygon:
				if (value)
				{
					if (value == 1)
					{
						Buffer::Draw();

						GLEnable(GL_POINT_SMOOTH);
						GLEnable(GL_LINE_SMOOTH);
						GLEnable(GL_POLYGON_SMOOTH);
					}
					else return false;
				}
				else
				{
					Buffer::Draw();

					GLDisable(GL_POINT_SMOOTH);
					GLDisable(GL_LINE_SMOOTH);
					GLDisable(GL_POLYGON_SMOOTH);
				}

				break;

			case EnableAlphaBlend:
				if (value)
				{
					if (value == 2 && !alphaEnabled)
					{
						Buffer::Draw();

						GLEnable(GL_BLEND);

						alphaEnabled = TRUE;
						GLUniform1ui(uniAlphaEnabledLoc, alphaEnabled);
					}
					else return false;
				}
				else if (alphaEnabled)
				{
					Buffer::Draw();

					GLDisable(GL_BLEND);

					alphaEnabled = FALSE;
					GLUniform1ui(uniAlphaEnabledLoc, alphaEnabled);
				}

				break;

			case TextureMipMap:
				textureMipMap = value;
				break;

			case TextureWrap:
				if (textureWrap != value)
				{
					Buffer::Draw();
					textureWrap = value;
				}
				break;

			case FogDensity:
				if (fogDensity != value)
				{
					Buffer::Draw();
					fogDensity = value;
					GLUniform1f(uniFogDensityLoc, value != 0 ? 1.0 / value : 0.0);
				}

				break;

			case FogColor:
				if (fogColor != value)
				{
					Buffer::Draw();

					fogColor = value;
					GLUniform4f(uniFogColorLoc,
						GLfloat((UINT8)(value >> 16) / FLOAT_255),
						GLfloat((UINT8)(value >> 8) / FLOAT_255),
						GLfloat((UINT8)(value) / FLOAT_255),
						GLfloat((UINT8)(value >> 24) / FLOAT_255));
				}

				break;

			case Functions:
				if (value)
				{
					funcGetHWND = (HWND(__stdcall *)())*((DWORD*)value + 1);
					funcResolution = (VOID(__stdcall *)(UINT, PROC))*((DWORD*)value + 2);
					//dword_6000A81C = *(value + 5);
					funcErrorMessageBox = (DWORD(__stdcall *)(DWORD, LPCSTR))*((DWORD*)value + 6);
				}
				else
				{
					funcGetHWND = NULL;
					funcResolution = NULL;
					//dword_6000A81C = 0;
					funcErrorMessageBox = NULL;
				}

				break;

			case EnableFog:
			case FogMode:
				if (value <= 1)
				{
					if (fogEnabled != value)
					{
						Buffer::Draw();
						fogEnabled = value;
						if (!fogEnabled)
							GLUniform1ui(uniFogModeLoc, 0);
						else
							GLUniform1ui(uniFogModeLoc, fogMode);
					}
				}
				else if (fogMode != value)
				{
					Buffer::Draw();
					fogMode = value;
					GLUniform1ui(uniFogModeLoc, fogMode);
				}

				break;

			case FogStart:
				if (fogStart != value)
				{
					Buffer::Draw();
					fogStart = value;
					GLUniform1f(uniFogStartLoc, fogStart);
				}
				break;

			case FogEnd:
				if (fogEnd != value)
				{
					Buffer::Draw();
					fogEnd = value;
					GLUniform1f(uniFogEndLoc, fogEnd);
				}
				break;

			case DepthMode:
			case DepthMode2:
				//if (!value)// && depthFunctionBinded != value)
					//GLDepthFunc(GL_LEQUAL);
				break;

			case WindowHandle:
				hWnd = (HWND)value;
				break;

			case MessageBoxFunction:
				funcErrorMessageBox = (DWORD(__stdcall *)(DWORD, LPCSTR))value;
				break;

			case ThrashApiVersion: // set api version
				API_VERSION = value;
				//if (value < 104 || (API_VERSION = (int)a2, (signed int)a2 > 107) )
					//API_VERSION = 107;
				break;

			case MallocFunction:
				funcMalloc = (VOID* (__stdcall *)(size_t))value;
				break;

			case FreeFunction:
				funcFree = (VOID(__stdcall *)(VOID*))value;
				break;

			case AlphaMode:
				if (alphaFunc != value)
				{
					Buffer::Draw();
					alphaFunc = value;
					GLUniform1f(uniAlphaFuncLoc, value != 0 ? (FLOAT)value / FLOAT_255 : 0.0);
				}

				break;

			case Unknown_39: // unknown function
				funcState = (DWORD(__stdcall *)(DWORD, DWORD))value;
				break;

			case DepthCompare:
				Buffer::Draw();

				switch (value)
				{
					case 0:
						depthCmp = GL_NEVER;
						GLDepthFunc(depthCmp);
						break;

					case 1:
						depthCmp = GL_LESS;
						GLDepthFunc(depthCmp);
						break;

					case 2:
						depthCmp = GL_EQUAL;
						GLDepthFunc(depthCmp);
						break;

					case 3:
						depthCmp = GL_LEQUAL;
						GLDepthFunc(depthCmp);
						break;

					case 4:
						depthCmp = GL_GREATER;
						GLDepthFunc(depthCmp);
						break;

					case 5:
						depthCmp = GL_NOTEQUAL;
						GLDepthFunc(depthCmp);
						break;

					case 6:
						depthCmp = GL_GEQUAL;
						GLDepthFunc(depthCmp);
						break;

					case 7:
						depthCmp = GL_ALWAYS;
						GLDepthFunc(depthCmp);
						break;

					default: break;
				}

				break;

			case EnableStencilBuffer:
				Buffer::Draw();

				if (value)
					GLEnable(GL_STENCIL_TEST);
				else
					GLDisable(GL_STENCIL_TEST);

				break;

			case StencilFunc:
				Buffer::Draw();

				switch (value)
				{
					case 0:
						GLStencilFunc(GL_NEVER, 2, 0xFF);
						break;

					case 1:
						GLStencilFunc(GL_LESS, 2, 0xFF);
						break;

					case 2:
						GLStencilFunc(GL_EQUAL, 2, 0xFF);
						break;

					case 3:
						GLStencilFunc(GL_LEQUAL, 2, 0xFF);
						break;

					case 4:
						GLStencilFunc(GL_GREATER, 2, 0xFF);
						break;

					case 5:
						GLStencilFunc(GL_NOTEQUAL, 2, 0xFF);
						break;

					case 6:
						GLStencilFunc(GL_GEQUAL, 2, 0xFF);
						break;

					case 7:
						GLStencilFunc(GL_ALWAYS, 2, 0xFF);
						break;

					default: break;
				}

				break;

			case StencilFail:
				Buffer::Draw();

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

					default: break;
				}

				GLStencilOp(stencilFail, stencilDepthFail, stencilPass);
				break;

			case StencilDepthFail:
				Buffer::Draw();

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

					default: break;
				}

				GLStencilOp(stencilFail, stencilDepthFail, stencilPass);
				break;

			case StencilPass:
				Buffer::Draw();

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

					default: break;
				}

				GLStencilOp(stencilFail, stencilDepthFail, stencilPass);
				break;

			case EnableDepthWrite:
			case EnableDepthWrite2:
				Buffer::Draw();

				GLDepthMask((GLboolean)value);
				break;

			case BlendMode: // set blend function
			case BlendMode2:
				switch (value)
				{
					case 0:
						Buffer::Draw();
						GLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						break;
					case 1:
						Buffer::Draw();
						GLBlendFunc(GL_SRC_ALPHA, GL_ONE);
						break;
					case 2:
						Buffer::Draw();
						GLBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
						break;
					case 3:
						Buffer::Draw();
						GLBlendFunc(GL_DST_COLOR, GL_ZERO);
						break;
					default:
						return false;
				}

				break;

			case Gamma:
			case Gamma2:
				if (gamma != value)
				{
					Buffer::Draw();
					gamma = value;
					GLUniform1f(uniGammaLoc, *(FLOAT*)&gamma * forced.gamma);
				}
				
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

			default:
				break;
		}

		if (value)
		{
			DWORD index = GetKeyIndex(key);
			if (index >= 0)
				stateValueArray[index] = value;

			if (funcState != NULL)
				funcState(index, value);
		}

		return true;
	}

	DWORD __stdcall Get(ThrashState key)
	{
		DWORD index = GetKeyIndex(key);
		if (index >= 0)
			return stateValueArray[index];

		return index;
	}
}