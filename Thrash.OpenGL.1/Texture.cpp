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

BOOL palleteInit = FALSE;

namespace Texture
{
	VOID __fastcall CheckWrap()
	{
		if (texturesEnabled && bindedTexture)
		{
			GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureClampS);
			GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureClampT);

			switch (forced.filtering)
			{
			case 1:
				GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMipMap && bindedTexture->level
					? GL_LINEAR_MIPMAP_LINEAR
					: GL_LINEAR);
				break;

			case 2:
				GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMipMap && bindedTexture->level
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
						GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, bindedTexture->level
							? GL_LINEAR_MIPMAP_NEAREST
							: GL_LINEAR);
						break;

					default:
						GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, bindedTexture->level
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
						GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, bindedTexture->level
							? GL_NEAREST_MIPMAP_NEAREST
							: GL_NEAREST);
						break;

					default:
						GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, bindedTexture->level
							? GL_NEAREST_MIPMAP_LINEAR
							: GL_NEAREST);
						break;
					}
				}
				break;
			}
		}
	}

	VOID __fastcall CheckPallete(ThrashTexture* texture)
	{
		if (texture->indexes && (!texture->pallete || memcmp(texture->pallete, colorPallete, sizeof(colorPallete))))
			Update(texture, texture->indexes, NULL);
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

	VOID __fastcall Delete(ThrashTexture* texture)
	{
		GLDeleteTextures(1, (GLuint*)&texture->id);

		if (texture->indexes)
			Memory::Free(texture->indexes);
		if (texture->data)
			Memory::Free(texture->data);
		if (texture->pallete)
			Memory::Free(texture->pallete);

		Memory::Free(texture);
	}

	LPTHRASHTEXTURE THRASHAPI Allocate(DWORD width, DWORD height, ThrashColorFormat colorFormat, ThrashIndexFormat indexFormat, DWORD level)
	{
		if (level & 0xFFFF0000)
			Main::ShowError("Multitexturing required", __FILE__, "Allocate", __LINE__);
		else if (!about.colorFormats[colorFormat])
		{
			CHAR fname[15];
			sprintf(fname, "Bad color format: %d", colorFormat);
			Main::ShowError(fname, __FILE__, "Allocate", __LINE__);
		}
		else if ((colorFormat == COLOR_INDEX_4 || colorFormat == COLOR_INDEX_8) && !about.indexFormats[indexFormat])
		{
			CHAR fname[15];
			sprintf(fname, "Bad color index format: %d", indexFormat);
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
			texture->colorFormat = colorFormat;
			texture->indexFormat = indexFormat;
			texture->reconvert = FALSE;
			texture->indexes = NULL;
			texture->data = NULL;
			texture->pallete = NULL;
			texture->previousTexture = lastTexture;
			lastTexture = texture;

			DWORD size = texture->width * texture->height;
			level = texture->level + 1;
			texture->pixels = 0;
			do
			{
				texture->pixels += size;
				size >>= 2;
			} while (--level);

			switch (texture->colorFormat)
			{
			case COLOR_INDEX_4:
				switch (texture->indexFormat)
				{
				case INDEX_RGB:
					if (forced.reconvert)
					{
						texture->format = GL_RGBA;
						texture->internalFormat = GL_RGBA8;
						texture->type = GL_UNSIGNED_BYTE;
						texture->size = 4;
						texture->stride = 0;
						texture->reconvert = TRUE;
						texture->indexes = (BYTE*)Memory::Allocate(texture->pixels >> 1);
					}
					else if (!GLColorTable)
					{
						texture->format = GL_RGB;
						texture->internalFormat = GL_RGB8;
						texture->type = GL_UNSIGNED_BYTE;
						texture->size = 3;
						texture->stride = 0;
						texture->reconvert = TRUE;
						texture->indexes = (BYTE*)Memory::Allocate(texture->pixels >> 1);
					}
					else
					{
						texture->format = GL_COLOR_INDEX;
						texture->internalFormat = GL_COLOR_INDEX4_EXT;
						texture->type = GL_UNSIGNED_BYTE;
						texture->size = 1;
						texture->stride = 1;
					}

					break;

				case INDEX_ARGB:
					if (forced.reconvert || !GLColorTable)
					{
						texture->format = GL_RGBA;
						texture->internalFormat = GL_RGBA8;
						texture->type = GL_UNSIGNED_BYTE;
						texture->size = 4;
						texture->stride = 0;
						texture->reconvert = TRUE;
						texture->indexes = (BYTE*)Memory::Allocate(texture->pixels >> 1);
					}
					else
					{
						texture->format = GL_COLOR_INDEX;
						texture->internalFormat = GL_COLOR_INDEX4_EXT;
						texture->type = GL_UNSIGNED_BYTE;
						texture->size = 1;
						texture->stride = 1;
					}

					break;

				default:
					Main::ShowError("Indexed - Bad pixel format", __FILE__, "Allocate", __LINE__);
				}

				break;

			case COLOR_INDEX_8:
				switch (texture->indexFormat)
				{
				case INDEX_RGB:
					if (forced.reconvert)
					{
						texture->format = GL_BGRA;
						texture->internalFormat = GL_RGBA8;
						texture->type = GL_UNSIGNED_BYTE;
						texture->size = 4;
						texture->stride = 0;
						texture->reconvert = TRUE;
						texture->indexes = (BYTE*)Memory::Allocate(texture->pixels);
					}
					else if (!GLColorTable)
					{
						texture->format = GL_BGR;
						texture->internalFormat = GL_RGB8;
						texture->type = GL_UNSIGNED_BYTE;
						texture->size = 3;
						texture->stride = 0;
						texture->reconvert = TRUE;
						texture->indexes = (BYTE*)Memory::Allocate(texture->pixels);
					}
					else
					{
						texture->format = GL_COLOR_INDEX;
						texture->internalFormat = GL_COLOR_INDEX8_EXT;
						texture->type = GL_UNSIGNED_BYTE;
						texture->size = 1;
						texture->stride = 0;
					}

					break;

				case INDEX_ARGB:
					if (forced.reconvert || !GLColorTable)
					{
						texture->format = GL_BGRA;
						texture->internalFormat = GL_RGBA8;
						texture->type = GL_UNSIGNED_BYTE;
						texture->size = 4;
						texture->stride = 0;
						texture->reconvert = TRUE;
						texture->indexes = (BYTE*)Memory::Allocate(texture->pixels);
					}
					else
					{
						texture->format = GL_COLOR_INDEX;
						texture->internalFormat = GL_COLOR_INDEX8_EXT;
						texture->type = GL_UNSIGNED_BYTE;
						texture->size = 1;
						texture->stride = 0;
					}

					break;

				default:
					Main::ShowError("Indexed - Bad pixel format", __FILE__, "Allocate", __LINE__);
				}

				break;

			case COLOR_ARGB_1555:
				if (forced.reconvert || glVersion < GL_VER_1_2)
				{
					texture->format = GL_RGBA;
					texture->internalFormat = GL_RGBA8;
					texture->type = GL_UNSIGNED_BYTE;
					texture->size = 4;
					texture->stride = 0;
					texture->reconvert = TRUE;
				}
				else
				{
					texture->format = GL_BGRA;
					texture->internalFormat = GL_RGB5_A1;
					texture->type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
					texture->size = 2;
					texture->stride = 0;
				}

				break;

			case COLOR_RGB_565:
				if (forced.reconvert)
				{
					texture->format = GL_RGBA;
					texture->internalFormat = GL_RGBA8;
					texture->type = GL_UNSIGNED_BYTE;
					texture->size = 4;
					texture->stride = 0;
					texture->reconvert = TRUE;
				}
				else if (glVersion < GL_VER_1_2)
				{
					texture->format = GL_RGB;
					texture->internalFormat = GL_RGB8;
					texture->type = GL_UNSIGNED_BYTE;
					texture->size = 3;
					texture->stride = 0;
					texture->reconvert = TRUE;
				}
				else
				{
					texture->format = GL_RGB;
					texture->internalFormat = GL_RGB;
					texture->type = GL_UNSIGNED_SHORT_5_6_5;
					texture->size = 2;
					texture->stride = 0;
				}

				break;

			case COLOR_RGB_888:
				if (forced.reconvert)
				{
					texture->format = GL_RGBA;
					texture->internalFormat = GL_RGBA8;
					texture->type = GL_UNSIGNED_BYTE;
					texture->size = 4;
					texture->stride = 0;
					texture->reconvert = TRUE;
				}
				else if (!glCapsBGR)
				{
					texture->format = GL_RGB;
					texture->internalFormat = GL_RGB8;
					texture->type = GL_UNSIGNED_BYTE;
					texture->size = 3;
					texture->stride = 0;
					texture->reconvert = TRUE;
				}
				else
				{
					texture->format = GL_BGR;
					texture->internalFormat = GL_RGB8;
					texture->type = GL_UNSIGNED_BYTE;
					texture->size = 3;
					texture->stride = 0;
				}

				break;

			case COLOR_ARGB_8888:
				if (forced.reconvert || !glCapsBGRA)
				{
					texture->format = GL_RGBA;
					texture->internalFormat = GL_RGBA8;
					texture->type = GL_UNSIGNED_BYTE;
					texture->size = 4;
					texture->stride = 0;
					texture->reconvert = TRUE;
				}
				else
				{
					texture->format = GL_BGRA;
					texture->internalFormat = GL_RGBA8;
					texture->type = GL_UNSIGNED_BYTE;
					texture->size = 4;
					texture->stride = 0;
				}

				break;

			case COLOR_ARGB_4444:
				if (forced.reconvert || glVersion < GL_VER_1_2)
				{
					texture->format = GL_RGBA;
					texture->internalFormat = GL_RGBA8;
					texture->type = GL_UNSIGNED_BYTE;
					texture->size = 4;
					texture->stride = 0;
					texture->reconvert = TRUE;
				}
				else
				{
					texture->format = GL_BGRA;
					texture->internalFormat = GL_RGBA4;
					texture->type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
					texture->size = 2;
					texture->stride = 0;
				}

				break;

				/*case L4_A4_8:
					texture->format = GL_LUMINANCE_ALPHA;
					texture->internalFormat = GL_LUMINANCE4_ALPHA4;
					texture->type = GL_UNSIGNED_BYTE_4_4_MESA;
					texture->size = 1;
					texture->stride = 0;

					break;*/

			default:
				Main::ShowError("Bad pixel format", __FILE__, "Allocate", texture->colorFormat);
			}

			Texture::Bind(texture);

			if (glVersion >= GL_VER_1_2)
			{
				GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
				GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, texture->level);
			}

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
		if (pallete)
		{
			if (GLColorTable)
				GLColorTable(GL_TEXTURE_2D, GL_RGBA8, 256, GL_BGRA, GL_UNSIGNED_BYTE, pallete);

			palleteInit = TRUE;
			memcpy(colorPallete, pallete, sizeof(colorPallete));
		}

		if (memory)
		{
			BOOL upload = TRUE;

			switch (texture->colorFormat)
			{
			case COLOR_INDEX_4:
				switch (texture->indexFormat)
				{
				case INDEX_RGB:
					if (forced.reconvert)
					{
						if (memory != texture->indexes)
							memcpy(texture->indexes, memory, texture->pixels >> 1);

						if (palleteInit)
							memory = Convert_BGR_4_To_RGBA_32(texture);
						else
							upload = FALSE;
					}
					else if (texture->reconvert)
					{
						if (memory != texture->indexes)
							memcpy(texture->indexes, memory, texture->pixels >> 1);

						if (palleteInit)
							memory = Convert_BGR_4_To_RGB_24(texture);
						else
							upload = FALSE;
					}

					break;

				case INDEX_ARGB:
					if (texture->reconvert)
					{
						if (memory != texture->indexes)
							memcpy(texture->indexes, memory, texture->pixels >> 1);

						if (palleteInit)
							memory = Convert_BGRA_4_To_RGBA_32(texture);
						else
							upload = FALSE;
					}
					break;

				default: break;
				}

				break;

			case COLOR_INDEX_8:
				switch (texture->indexFormat)
				{
				case INDEX_RGB:
					if (forced.reconvert)
					{
						if (memory != texture->indexes)
							memcpy(texture->indexes, memory, texture->pixels);

						if (palleteInit)
							memory = Convert_BGR_8_To_RGBA_32(texture);
						else
							upload = FALSE;
					}
					else if (texture->reconvert)
					{
						if (memory != texture->indexes)
							memcpy(texture->indexes, memory, texture->pixels);

						if (palleteInit)
							memory = Convert_BGR_8_To_RGB_24(texture);
						else
							upload = FALSE;
					}
					break;

				case INDEX_ARGB:
					if (texture->reconvert)
					{
						if (memory != texture->indexes)
							memcpy(texture->indexes, memory, texture->pixels);

						if (palleteInit)
							memory = Convert_RGBA_8_To_RGBA_32(texture);
						else
							upload = FALSE;
					}
					break;

				default: break;
				}

				break;

			case COLOR_ARGB_1555:
				if (texture->reconvert)
					memory = Convert_BGR5_A1_To_RGBA_32(texture, memory);
				break;

			case COLOR_RGB_565:
				if (forced.reconvert)
					memory = Convert_RGB565_To_RGBA_32(texture, memory);
				else if (texture->reconvert)
					memory = Convert_RGB565_To_RGB_24(texture, memory);
				break;

			case COLOR_RGB_888:
				if (forced.reconvert)
					memory = Convert_BGR_24_To_RGBA_32(texture, memory);
				else if (texture->reconvert)
					memory = Convert_BGR_24_To_RGB_24(texture, memory);
				break;

			case COLOR_ARGB_8888:
				if (texture->reconvert)
					memory = Convert_BGRA_32_To_RGBA_32(texture, memory);
				break;

			case COLOR_ARGB_4444:
				if (texture->reconvert)
					memory = Convert_BGRA_16_To_RGBA_32(texture, memory);
				break;

			default: break;
			}

			Texture::Bind(texture);

			if (upload && texture->level >= 0)
			{
				DWORD width = texture->width;
				DWORD height = texture->height;
				BYTE* pixelsData = (BYTE*)memory;
				DWORD level = 0;
				do
				{
					GLTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height, texture->format, texture->type, pixelsData);
					pixelsData += (texture->size * width * height) >> texture->stride;
					width >>= 1;
					height >>= 1;
					++level;
				} while (level <= texture->level);
			}
		}

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
				Delete(texture);
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
						Delete(texture);
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
				Delete(texture);
				texture = prev;
			} while (texture);

			lastTexture = NULL;
		}

		return TRUE;
	}
}