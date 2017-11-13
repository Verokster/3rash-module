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

ThrashTexture* lastTexture;
ThrashTexture* bindedTexture;

ThrashTexColorIndexFormats textureIndexFormats = {
	0, // BGR_4
	0, // PALETTE6666
	0,
	1, // BGR_8
	1, // BGRA_8
	0xFFFFFFFF // END
};

ThrashTexColorFormats textureFormats = {
	0,
	0, // Indexed_4
	0, // Indexed_8
	1, // BGR5_A1_16
	1, // RGB565_16
	1, // BGR_24
	1, // BGRA_32
	1, // BGRA4_16
	0, // L4_A4_8
	0, // A_8
	0xFFFFFFFF // END
};

namespace Texture
{
	VOID __fastcall CheckWrap()
	{
		if (texturesEnabled && bindedTexture)
		{
			GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureClampS);
			GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureClampT);

			GLTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, textureLodBias);

			switch (forced.filtering)
			{
			case 1:
				GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMipMap != 0 && bindedTexture->level != 0
					? GL_LINEAR_MIPMAP_LINEAR
					: GL_LINEAR);
				break;

			case 2:
				GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMipMap != 0 && bindedTexture->level != 0
					? GL_NEAREST_MIPMAP_NEAREST
					: GL_NEAREST);
				break;

			default:
				if (textureFilterEnabled)
				{
					GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					switch (textureMipMap)
					{
					case 0:
						GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						break;

					case 1:
						GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, bindedTexture->level != 0
							? GL_LINEAR_MIPMAP_NEAREST
							: GL_LINEAR);
						break;

					default:
						GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, bindedTexture->level != 0
							? GL_LINEAR_MIPMAP_LINEAR
							: GL_LINEAR);
						break;
					}
				}
				else
				{
					GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

					switch (textureMipMap)
					{
					case 0:
						GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						break;

					case 1:
						GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, bindedTexture->level != 0
							? GL_NEAREST_MIPMAP_NEAREST
							: GL_NEAREST);
						break;

					default:
						GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, bindedTexture->level != 0
							? GL_NEAREST_MIPMAP_LINEAR
							: GL_NEAREST);
						break;
					}
				}
				break;
			}
		}
	}

	VOID __fastcall Bind(ThrashTexture* texture)
	{
		DWORD address = (DWORD)texture;
		if (!texture || !lastTexture || address < MIN_TEX_ADDRESS)
			bindedTexture = NULL;
		else if (bindedTexture != texture)
		{
			bindedTexture = texture;
			GLBindTexture(GL_TEXTURE_2D, texture->id);
		}
	}

	LPTHRASHTEXTURE THRASHAPI Allocate(DWORD width, DWORD height, ThrashColorFormatIndex colorFormatIndex, ThrashIndexFormatIndex indexFormatIndex, DWORD level)
	{
		if (level & 0xFFFF0000)
			Main::ShowError("Multitexturing required", __FILE__, "Allocate", __LINE__);
		else if (((DWORD*)&textureFormats)[(DWORD)colorFormatIndex] <= 0)
		{
			CHAR fname[15];
			sprintf(fname, "Bad color format: %d", colorFormatIndex);
			Main::ShowError(fname, __FILE__, "Allocate", __LINE__);
		}
		else if ((colorFormatIndex == INDEXED_4 || colorFormatIndex == INDEXED_8) && ((DWORD*)&textureIndexFormats)[(DWORD)indexFormatIndex] <= 0)
		{
			CHAR fname[15];
			sprintf(fname, "Bad color index format: %d", indexFormatIndex);
			Main::ShowError(fname, __FILE__, "Allocate", __LINE__);
		}

		ThrashTexture* texture = (ThrashTexture*)Memory::Allocate(sizeof(ThrashTexture));
		if (texture)
		{
			GLGenTextures(1, (GLuint*)&texture->id);
			texture->width = width;
			texture->height = height;
			texture->tmu = level >> 16;
			texture->level = level & 0x0000FFFF;
			texture->colorFormatIndex = colorFormatIndex;
			texture->indexFormatIndex = indexFormatIndex;
			texture->reconvert = FALSE;
			texture->previousTexture = lastTexture;
			lastTexture = texture;

			DWORD size = texture->width * texture->height;
			++level;
			texture->pixels = 0;
			do
			{
				texture->pixels += size;
				size >>= 2;
				--level;
			} while (level);

			switch (texture->colorFormatIndex)
			{
			case INDEXED_4:
				switch (texture->indexFormatIndex)
				{
				case INDEXED_BGR:
					texture->format = GL_RGB;
					texture->internalFormat = GL_RGB8;
					texture->type = GL_UNSIGNED_BYTE;
					texture->size = 3;
					texture->stride = 0;
					texture->reconvert = TRUE;
					break;

				case INDEXED_BGRA:
					texture->format = GL_RGBA;
					texture->internalFormat = GL_RGBA8;
					texture->type = GL_UNSIGNED_BYTE;
					texture->size = 4;
					texture->stride = 0;
					texture->reconvert = TRUE;
					break;

				default:
					Main::ShowError("Indexed - Bad pixel format", __FILE__, "Allocate", __LINE__);
				}

				break;

			case INDEXED_8:
				switch (texture->indexFormatIndex)
				{
				case INDEXED_BGR:
					texture->format = GL_BGR;
					texture->internalFormat = GL_RGB8;
					texture->type = GL_UNSIGNED_BYTE;
					texture->size = 3;
					texture->reconvert = TRUE;
					break;

				case INDEXED_BGRA:
					texture->format = GL_BGRA;
					texture->internalFormat = GL_RGBA8;
					texture->type = GL_UNSIGNED_BYTE;
					texture->size = 4;
					texture->reconvert = TRUE;
					break;

				default:
					Main::ShowError("Indexed - Bad pixel format", __FILE__, "Allocate", __LINE__);
				}

				break;

			case BGR5_A1_16:
				texture->format = GL_BGRA;
				texture->internalFormat = GL_RGB5_A1;
				texture->type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
				texture->size = 2;
				break;

			case RGB565_16:
				texture->format = GL_RGB;
				texture->internalFormat = GL_RGB565;
				texture->type = GL_UNSIGNED_SHORT_5_6_5;
				texture->size = 2;
				break;

			case BGR_24:
				texture->format = GL_BGR;
				texture->internalFormat = GL_RGB8;
				texture->type = GL_UNSIGNED_BYTE;
				texture->size = 3;
				break;

			case BGRA_32:
				texture->format = GL_BGRA;
				texture->internalFormat = GL_RGBA8;
				texture->type = GL_UNSIGNED_BYTE;
				texture->size = 4;
				break;

			case BGRA4_16:
				texture->format = GL_BGRA;
				texture->internalFormat = GL_RGBA4;
				texture->type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
				texture->size = 2;
				break;

				/*case L4_A4_8:
				texture->format = GL_LUMINANCE_ALPHA;
				texture->internalFormat = GL_LUMINANCE4_ALPHA4;
				texture->type = GL_UNSIGNED_BYTE_4_4_MESA;
				texture->size = 1;
				texture->stride = 0;

				break;*/

			default:
				Main::ShowError("Bad pixel format", __FILE__, "Allocate", texture->colorFormatIndex);
			}

			Buffer::Draw();
			Texture::Bind(texture);

			GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, texture->level);

			if (texture->level >= 0)
			{
				level = 0;
				do
				{
					GLTexImage2D(GL_TEXTURE_2D, level, texture->internalFormat, width, height, GL_NONE, texture->format, texture->type, NULL);
					width >>= 1;
					height >>= 1;
					++level;
				} while (level <= texture->level);
			}
		}
		else
			Main::ShowError("Out of memory.", __FILE__, "Allocate", __LINE__);

		return texture;
	}

	LPTHRASHTEXTURE THRASHAPI Update(ThrashTexture* texture, VOID* memory, BYTE* pallete)
	{
		if (!texture || !memory || texture->colorFormatIndex == COLOR_NONE)
			return NULL;

		switch (texture->colorFormatIndex)
		{
		case INDEXED_4:
			switch (texture->indexFormatIndex)
			{
			case 3:
				memory = Convert_BGR_4_To_RGB_24(texture, memory, pallete);
				break;

			case 4:
				memory = Convert_BGRA_4_To_RGBA_32(texture, memory, pallete);
				break;

			default: break;
			}

			break;

		case INDEXED_8:
			switch (texture->indexFormatIndex)
			{
			case 3:
				memory = Convert_BGR_8_To_RGB_24(texture, memory, pallete);
				break;

			case 4:
				memory = Convert_BGRA_8_To_RGBA_32(texture, memory, pallete);
				break;

			default: break;
			}

			break;

		default:
			break;
		}


		Buffer::Draw();
		Texture::Bind(texture);

		if (texture->level >= 0)
		{
			DWORD width = texture->width;
			DWORD height = texture->height;
			BYTE* pixelsData = (BYTE*)memory;
			DWORD level = 0;
			do
			{
				GLTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height, texture->format, texture->type, pixelsData);
				pixelsData += texture->size * width * height;
				width >>= 1;
				height >>= 1;
				++level;
			} while (level <= texture->level);
		}

		if (texture->reconvert)
			Memory::Free(memory);

		return texture;
	}

	BOOL THRASHAPI Set(ThrashTexture* texture)
	{
		return State::Set(State::SetTexture, (DWORD)texture);
	}

	BOOL THRASHAPI Free(ThrashTexture* texture)
	{
		if (lastTexture)
		{
			if (bindedTexture == texture)
				State::Set(State::SetTexture, NULL);

			if (lastTexture == texture)
			{
				lastTexture = texture->previousTexture;
				GLDeleteTextures(1, (GLuint*)&texture->id);
				Memory::Free(texture);
				return TRUE;
			}
			else
			{
				ThrashTexture* currTexture = lastTexture;
				do
				{
					if (currTexture->previousTexture == texture)
					{
						currTexture->previousTexture = texture->previousTexture;
						GLDeleteTextures(1, (GLuint*)&texture->id);
						Memory::Free(texture);
						return TRUE;
					}

					currTexture = currTexture->previousTexture;
				} while (currTexture);
			}
		}

		return FALSE;
	}

	BOOL THRASHAPI Reset()
	{
		if (lastTexture)
		{
			State::Set(State::SetTexture, NULL);

			ThrashTexture* texture = lastTexture;
			do
			{
				ThrashTexture* prev = texture->previousTexture;
				GLDeleteTextures(1, (GLuint*)&texture->id);
				Memory::Free(texture);
				texture = prev;
			} while (texture);

			lastTexture = NULL;
		}

		return TRUE;
	}
}