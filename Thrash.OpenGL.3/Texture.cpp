#include "stdafx.h"
#include "Global.hpp" 


const DWORD MAX_BINDED_TEXTURES = 4;
ThrashTexture* lastTexture;
ThrashTexture* bindedTexture;
ThrashBindedTextureId bindedTexturesIds[MAX_BINDED_TEXTURES];

namespace Texture
{
	VOID __fastcall CheckWrap()
	{
		if (texturesEnabled && bindedTexture != NULL && (*bindedTexture).wrap != textureWrap)
		{
			(*bindedTexture).wrap = textureWrap;

			switch (textureWrap)
			{
				case 0:
					GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					break;

				case 1:
					GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					break;

				case 2:
					GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
					GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
					break;

				default:
					break;
			}
		}
	}

	VOID __fastcall ResetActive()
	{
		ThrashBindedTextureId* id = bindedTexturesIds;
		DWORD count = MAX_BINDED_TEXTURES;
		do
		{
			(*id).isActive = (*id).isUsed = FALSE;
			++id;
		}
		while (--count);
	}

	VOID __fastcall ResetUsed()
	{
		ThrashBindedTextureId* id = bindedTexturesIds;
		DWORD count = MAX_BINDED_TEXTURES;
		do
			(*id++).isUsed = FALSE;
		while (--count);

		bindedTexturesIds[drawTextureUnit - 1].isUsed = TRUE;
		currTextureUnit = 0;
	}

	VOID __fastcall Bind(ThrashTexture* texture)
	{
		if (bindedTexture != texture)
		{
			if (texture != NULL)
			{
				DWORD foundIdx = 0;
				ThrashBindedTextureId* id = bindedTexturesIds;
				DWORD count = MAX_BINDED_TEXTURES;
				do
				{
					if ((*id).id == (*texture).id && (*id).isActive)
					{
						(*id).isUsed = TRUE;
						foundIdx = MAX_BINDED_TEXTURES - count + 1;
						break;
					}

					++id;
				}
				while (--count);

				if (foundIdx)
				{
					GLActiveTexture(GL_TEXTURE0 + foundIdx - 1);
					drawTextureUnit = foundIdx;
				}
				else
				{
					do
					{
						if (currTextureUnit == MAX_BINDED_TEXTURES)
						{
							Buffer::Draw();
							id = bindedTexturesIds;
							(*id).id = (*texture).id;
							(*id).isActive = (*id).isUsed = TRUE;
							break;
						}
						else
						{
							id = &bindedTexturesIds[currTextureUnit];
							if (!(*id).isUsed || !(*id).isActive)
							{
								(*id).id = (*texture).id;
								(*id).isActive = (*id).isUsed = TRUE;
								break;
							}
							else
								++currTextureUnit;
						}
					}
					while (TRUE);

					GLActiveTexture(GL_TEXTURE0 + currTextureUnit);
					GLBindTexture(GL_TEXTURE_2D, (*texture).id);
					drawTextureUnit = ++currTextureUnit;
				}
			}

			bindedTexture = texture;
		}
	}

	ThrashTexture* __stdcall Allocate(DWORD width, DWORD height, ThrashColorFormatIndex colorFormatIndex, DWORD indexFormatIndex, DWORD level)
	{
		ThrashTexture* texture = (ThrashTexture*)Memory::Allocate(sizeof(ThrashTexture));
		if (texture)
		{
			GLGenTextures(1, (GLuint*)texture);
			(*texture).width = width;
			(*texture).height = height;
			(*texture).level = level;
			(*texture).colorFormatIndex = colorFormatIndex;
			(*texture).indexFormatIndex = indexFormatIndex;
			(*texture).wrap = -1;
			(*texture).previousTexture = lastTexture;
			lastTexture = texture;

			DWORD internalFormat;
			GLenum type;
			DWORD format;
			switch ((*texture).colorFormatIndex)
			{
				case INDEXED:
					switch ((*texture).indexFormatIndex)
					{
						case 0:
							format = GL_BGR;
							internalFormat = GL_RGB8;
							type = GL_UNSIGNED_BYTE;
							break;

						case 3:
							format = GL_BGR;
							internalFormat = GL_RGB8;
							type = GL_UNSIGNED_BYTE;
							break;

						case 4:
							format = GL_BGRA;
							internalFormat = GL_RGBA8;
							type = GL_UNSIGNED_BYTE;
							break;

						default:
							Main::ShowError("Bad pixel format", __FILE__, __FUNCTION__, __LINE__);
					}

					break;

				case BGR5_A1_16:
					format = GL_BGRA;
					internalFormat = GL_RGB5_A1;
					type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
					break;

				case RGB565_16:
					format = GL_RGB;
					internalFormat = GL_RGB565;
					type = GL_UNSIGNED_SHORT_5_6_5;
					break;

				case BGR_24:
					format = GL_BGR;
					internalFormat = GL_RGB8;
					type = GL_UNSIGNED_BYTE;
					break;

				case BGRA_32:
					format = GL_BGRA;
					internalFormat = GL_RGBA8;
					type = GL_UNSIGNED_BYTE;
					break;

				case BGRA4_16:
					format = GL_BGRA;
					internalFormat = GL_RGBA4;
					type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
					break;

				default: 
					Main::ShowError("Bad pixel format", __FILE__, __FUNCTION__, __LINE__);
			}

			Texture::Bind(texture);

			if (textureFilterEnabled)
			{
				GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				if (textureMipMap == 1)
				{
					GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (*texture).level != 0
						? GL_LINEAR_MIPMAP_NEAREST
						: GL_LINEAR);
				}
				else if (textureMipMap == 2)
				{
					GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (*texture).level != 0
						? GL_LINEAR_MIPMAP_LINEAR
						: GL_LINEAR);
				}
				else
					GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
			else
			{
				GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				if (textureMipMap == 1)
				{
					GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (*texture).level != 0
						? GL_NEAREST_MIPMAP_NEAREST
						: GL_NEAREST);
				}
				else if (textureMipMap == 2)
				{
					GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (*texture).level != 0
						? GL_NEAREST_MIPMAP_LINEAR
						: GL_NEAREST);
				}
				else
					GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}

			if ((*texture).level >= 0)
			{
				level = 0;
				DWORD size = width * height;
				do
				{
					GLTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, GL_NONE, format, type, NULL);
					
					++level;
				} while (level <= (*texture).level);
			}
		}
		else
			Main::ShowError("Out of memory.", __FILE__, __FUNCTION__, __LINE__);

		return texture;
	}

	ThrashTexture* __stdcall Update(ThrashTexture* texture, VOID* memory, BYTE* pallete)
	{
		if (texture == NULL || memory == NULL || (*texture).colorFormatIndex == 0)
			return NULL;

		ThrashBindedTextureId* id = bindedTexturesIds;
		DWORD count = MAX_BINDED_TEXTURES;
		do
		{
			if ((*id).id == (*texture).id && (*id).isActive)
			{
				Buffer::Draw();
				break;
			}

			++id;
		}
		while (--count);

		Texture::Bind(texture);

		DWORD internalFormat;
		GLenum type;
		BOOL isMemoryAllocated = FALSE;
		DWORD format;
		switch ((*texture).colorFormatIndex)
		{
			case INDEXED:
				switch ((*texture).indexFormatIndex)
				{
					case 0:
						memory = Convert_BGR_2_To_BGR_24(texture, memory, pallete);
						isMemoryAllocated = TRUE;
						format = GL_BGR;
						internalFormat = 3;
						type = GL_UNSIGNED_BYTE;
						break;

					case 3:
						memory = Convert_BGR_8_To_BGR_24(texture, memory, pallete);
						isMemoryAllocated = TRUE;
						format = GL_BGR;
						internalFormat = 3;
						type = GL_UNSIGNED_BYTE;
						break;

					case 4:
						memory = Convert_BGRA_8_To_BGRA_32(texture, memory, pallete);
						isMemoryAllocated = TRUE;
						format = GL_BGRA;
						internalFormat = 4;
						type = GL_UNSIGNED_BYTE;
						break;

					default:
						Main::ShowError("Indexed - Bad pixel format", __FILE__, __FUNCTION__, __LINE__);
				}
				
				break;

			case BGR5_A1_16:
				format = GL_BGRA;
				internalFormat = 2;
				type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
				break;

			case RGB565_16:
				format = GL_RGB;
				internalFormat = 2;
				type = GL_UNSIGNED_SHORT_5_6_5;
				break;

			case BGR_24:
				format = GL_BGR;
				internalFormat = 3;
				type = GL_UNSIGNED_BYTE;
				break;

			case BGRA_32:
				format = GL_BGRA;
				internalFormat = 4;
				type = GL_UNSIGNED_BYTE;
				break;

			case BGRA4_16:
				format = GL_BGRA;
				internalFormat = 2;
				type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
				break;

			default:
				Main::ShowError("Indexed - Bad pixel format", __FILE__, __FUNCTION__, __LINE__);
		}

		if ((*texture).level >= 0)
		{
			DWORD width = (*texture).width;
			DWORD height = (*texture).height;
			DWORD level = 0;
			BYTE* pixelsData = (BYTE*)memory;

			do
			{
				GLTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height, format, type, pixelsData);
				
				pixelsData += internalFormat * width * height;
				width >>= 1;
				height >>= 1;
				++level;
			} while (level <= (*texture).level);
		}

		if (isMemoryAllocated)
			Memory::Free(memory);

		return texture;
	}

	BOOL __stdcall Set(ThrashTexture* texture)
	{
		return State::Set(SetTexture, (DWORD)texture);
	}

	BOOL __stdcall Free(ThrashTexture* texture)
	{
		if (lastTexture)
		{
			ThrashBindedTextureId* id = bindedTexturesIds;
			DWORD count = MAX_BINDED_TEXTURES;
			do
			{
				if ((*id).id == (*texture).id && (*id).isActive)
				{
					Buffer::Draw();
					(*id).isActive = (*id).isUsed = FALSE;
					break;
				}

				++id;
			}
			while (--count);

			if (bindedTexture == texture)
				State::Set(SetTexture, NULL);

			if (lastTexture == texture)
			{
				lastTexture = (*texture).previousTexture;
				GLDeleteTextures(1, (GLuint*)texture);
				Memory::Free(texture);
				return TRUE;
			}
			else
			{
				ThrashTexture* currTexture = lastTexture;
				do
				{
					if ((*currTexture).previousTexture == texture)
					{
						(*currTexture).previousTexture = (*texture).previousTexture;
						GLDeleteTextures(1, (GLuint*)texture);
						Memory::Free(texture);
						return TRUE;
					}

					currTexture = (*currTexture).previousTexture;
				} while (currTexture);
			}
		}

		return FALSE;
	}

	BOOL __stdcall Reset()
	{
		if (lastTexture)
		{
			Buffer::Draw();
			Texture::ResetActive();

			DWORD count = 0;
			ThrashTexture* texture = lastTexture;
			do
			{
				texture = (*texture).previousTexture;
				++count;
			} while (texture);

			if (count)
			{
				VOID* idList = Memory::Allocate(count * sizeof(DWORD));
				if (idList)
				{
					texture = lastTexture;
					DWORD index = count;
					DWORD* id = (DWORD*)idList;
					do
					{
						*id = (*texture).id;
						ThrashTexture* temp = (*texture).previousTexture;
						Memory::Free(texture);
						lastTexture = texture = temp;
						++id;
					} while (--index);

					GLDeleteTextures(count, (GLuint*)idList);
					Memory::Free(idList);
				}
				else
					Main::ShowError("Out of memory.", __FILE__, __FUNCTION__, __LINE__);
			}
		}

		State::Set(Unknown_55, State::Get(Unknown_55) + 1);
		State::Set(SetTexture, NULL);

		return TRUE;
	}
}