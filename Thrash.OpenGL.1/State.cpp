#include "stdafx.h"
#include "Global.hpp"

namespace State
{
	BOOL __stdcall Set(ThrashState key, DWORD value)
	{
		GLfloat color[4];
		FLOAT gammaFactor;
		switch (key)
		{
			case SetTexture:
				if (value)
				{
					if (!texturesEnabled)
					{
						texturesEnabled = true;
						GLEnable(GL_TEXTURE_2D);
					}

					if (bindedTexture != (ThrashTexture*)value)
					{
						bindedTexture = (ThrashTexture*)value;
						GLBindTexture(GL_TEXTURE_2D, (*bindedTexture).id);
					}
				}
				else if (texturesEnabled)
				{
					texturesEnabled = false;
					GLDisable(GL_TEXTURE_2D);
				}

				break;

			case CullFace:
				if (value >= 0 && value <= 2)
				{
					if (cullFaceMode != value)
					{
						cullFaceMode = value;
						switch (value)
						{
							case 1:
								GLEnable(GL_CULL_FACE);
								GLFrontFace(GL_CW);
								break;

							case 2:
								GLEnable(GL_CULL_FACE);
								GLFrontFace(GL_CCW);
								break;

							default:
								GLDisable(GL_CULL_FACE);
								break;
						}
					}

					break;
				}
				else return false;

			case ClearColor:
				GLClearColor(
					GLclampf((UINT8)(value >> 16) / FLOAT_255),
					GLclampf((UINT8)(value >> 8) / FLOAT_255),
					GLclampf((UINT8)(value) / FLOAT_255),
					GLclampf((UINT8)(value >> 24) / FLOAT_255));

				break;

			case EnableDepthBuffer:
				if (value)
				{
					if (value == 1)
						GLEnable(GL_DEPTH_TEST);
					else return false;
				}
				else
					GLDisable(GL_DEPTH_TEST);

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
						//GLDisable(GL_LIGHTING);
						break;

					case 1:
						GLShadeModel(GL_SMOOTH);
						//GLDisable(GL_LIGHTING);
						break;

					case 2:
						GLShadeModel(GL_SMOOTH);
						//GLEnable(GL_LIGHTING);
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
						GLEnable(GL_POINT_SMOOTH);
						GLEnable(GL_LINE_SMOOTH);
						GLEnable(GL_POLYGON_SMOOTH);
					}
					else return false;
				}
				else
				{
					GLDisable(GL_POINT_SMOOTH);
					GLDisable(GL_LINE_SMOOTH);
					GLDisable(GL_POLYGON_SMOOTH);
				}

				break;

			case EnableAlphaBlend:
				if (value)
				{
					if (value == 2)
					{
						GLEnable(GL_BLEND);
						GLEnable(GL_ALPHA_TEST);
					}
					else return false;
				}
				else
				{
					GLDisable(GL_BLEND);
					GLDisable(GL_ALPHA_TEST);
				}

				break;

			case TextureMipMap:
				textureMipMap = value;
				break;

			case TextureWrap:
				textureWrap = value;
				break;

			case FogDensity:
				if (value != 0)
					GLFogf(GL_FOG_DENSITY, (GLfloat)(1.0 / value));
				else
					GLFogf(GL_FOG_DENSITY, GL_ZERO);

				break;

			case FogColor:
				if (fogColor != value)
				{
					fogColor = value;
					color[2] = (UINT8)value / FLOAT_255;
					color[1] = (UINT8)(value >> 8) / FLOAT_255;
					color[0] = (UINT8)(value >> 16) / FLOAT_255;
					color[3] = (UINT8)(value >> 24) / FLOAT_255;
					GLFogfv(GL_FOG_COLOR, color);
				}

				break;

			case LineWidth:
				GLLineWidth(value);
				break;

			case Functions:
				if (value)
				{
					funcGetHWND = (HWND(__stdcall *)())*((DWORD*)value + 1);
					funcResolution = (VOID(__stdcall *)(UINT, PROC))*((DWORD*)value + 2);
					funcErrorMessageBox = (DWORD(__stdcall *)(DWORD, LPCSTR))*((DWORD*)value + 6);
				}
				else
				{
					funcGetHWND = NULL;
					funcResolution = NULL;
					funcErrorMessageBox = NULL;
				}

				break;

			case EnableFog:
			case FogMode:
				switch (value)
				{
					case Disabled:
						if (fogEnabled)
						{
							fogEnabled = FALSE;
							GLDisable(GL_FOG);
						}

						break;
					case Enabled:
						if (!fogEnabled)
						{
							fogEnabled = TRUE;
							GLEnable(GL_FOG);
							GLFogi(GL_FOG_COORD_SRC, GL_FOG_COORD);
						}

						break;
					case Linear:
						if (fogMode != value)
						{
							fogMode = value;
							GLFogi(GL_FOG_MODE, GL_LINEAR);
						}

						break;
					case Exp:
						if (fogMode != value)
						{
							fogMode = value;
							GLFogi(GL_FOG_MODE, GL_EXP);
						}

						break;
					case Exp2:
						if (fogMode != value)
						{
							fogMode = value;
							GLFogi(GL_FOG_MODE, GL_EXP2);
						}

						break;
					default:
						return false;
				}
				break;

			case FogStart:
				GLFogf(GL_FOG_START, (GLfloat)value);
				break;

			case FogEnd:
				GLFogf(GL_FOG_END, (GLfloat)value);
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
				if (value)
					GLAlphaFunc(GL_GEQUAL, (GLclampf)(value / FLOAT_255));
				else
					GLAlphaFunc(GL_ALWAYS, GL_ZERO);
				break;

			case Unknown_39:
				funcState = (DWORD(__stdcall *)(DWORD, DWORD))value;
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

					default: break;
				}

				GLDepthFunc(depthCmp);

				break;

			case EnableStencilBuffer:
				if (value)
					GLEnable(GL_STENCIL_TEST);
				else
					GLDisable(GL_STENCIL_TEST);
				break;

			case StencilFunc:
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
				GLDepthMask((GLboolean)value);
				break;

			case BlendMode: // set blend function
			case BlendMode2:
				switch (value)
				{
					case 0:
						GLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						break;
					case 1:
						GLBlendFunc(GL_SRC_ALPHA, GL_ONE);
						break;
					case 2:
						GLBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
						break;
					case 3:
						GLBlendFunc(GL_DST_COLOR, GL_ZERO);
						break;
					default:
						return false;
				}

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