#include "stdafx.h"
#include "Global.hpp"

namespace Rect
{
	VOID __fastcall SwapRows(ThrashRectangle* rectangle, VOID* data, DWORD bytesPerPixel)
	{
		DWORD height = rectangle->height;
		DWORD bytesPerRow = bytesPerPixel * rectangle->width;
		if (bytesPerRow % 4 == 0)
		{
			DWORD sectPerRow = bytesPerRow >> 2;
			DWORD* start = (DWORD*)data;
			DWORD* end = start + sectPerRow * (height - 1);

			while (end > start)
			{
				DWORD width = sectPerRow;
				while (width)
				{
					DWORD byte = *start;
					*start++ = *end;
					*end++ = byte;
					--width;
				}

				end -= sectPerRow << 1;
			}
		}
		else if (bytesPerRow % 2 == 0)
		{
			DWORD sectPerRow = bytesPerRow >> 1;
			WORD* start = (WORD*)data;
			WORD* end = start + sectPerRow * (height - 1);

			while (end > start)
			{
				DWORD width = sectPerRow;
				while (width)
				{
					WORD byte = *start;
					*start++ = *end;
					*end++ = byte;
					--width;
				}

				end -= sectPerRow << 1;
			}
		}
		else
		{
			DWORD sectPerRow = bytesPerRow;
			BYTE* start = (BYTE*)data;
			BYTE* end = start + sectPerRow * (height - 1);

			while (end > start)
			{
				DWORD width = sectPerRow;
				while (width)
				{
					BYTE byte = *start;
					*start++ = *end;
					*end++ = byte;
					--width;
				}

				end -= sectPerRow << 1;
			}
		}
	}

	BOOL THRASHAPI Read(ThrashRectangle rectangle, VOID* data)
	{
		DWORD bytesPerPixel;
		GLenum format, type;
		if (!forced.movies16Bit)
		{
			bytesPerPixel = selectedResolution->colorDepth >> 3;
			switch (bytesPerPixel)
			{
			case 3:
				format = GL_BGR;
				type = GL_UNSIGNED_BYTE;
				break;

			case 4:
				format = GL_BGRA;
				type = GL_UNSIGNED_BYTE;
				break;

			default:
				format = GL_RGB;
				type = GL_UNSIGNED_SHORT_5_6_5;
				break;
			}
		}
		else
		{
			bytesPerPixel = 2;
			format = GL_RGB;
			type = GL_UNSIGNED_SHORT_5_6_5;
		}

		if (viewport.width != selectedResolution->width || viewport.height != selectedResolution->height)
		{
			DWORD width = Main::Round((FLOAT)rectangle.width * viewport.clipFactor.x);
			DWORD height = Main::Round((FLOAT)rectangle.height * viewport.clipFactor.y);

			DWORD count = width * height;
			VOID* memory = Memory::Allocate(count * bytesPerPixel);

			GLReadPixels(viewport.rectangle.x + Main::Round(rectangle.x * viewport.clipFactor.x), viewport.height - (viewport.rectangle.y + Main::Round((rectangle.height + rectangle.y) * viewport.clipFactor.y)), width, height, format, type, memory);

			if (bytesPerPixel == 4)
			{
				DWORD* read = (DWORD*)memory;
				DWORD* write = (DWORD*)data;
				for (int y = 0; y < rectangle.height; ++y)
				{
					DWORD step = Main::Round((FLOAT)y * viewport.clipFactor.y) * width;
					for (int x = 0; x < rectangle.width; ++x)
					{
						DWORD pos = step + Main::Round((FLOAT)x * viewport.clipFactor.x);
						*write++ = read[pos];
					}
				}
			}
			else
			{
				WORD* read = (WORD*)memory;
				WORD* write = (WORD*)data;
				for (int y = 0; y < rectangle.height; ++y)
				{
					DWORD step = Main::Round((FLOAT)y * viewport.clipFactor.y) * width;
					for (int x = 0; x < rectangle.width; ++x)
					{
						DWORD pos = step + Main::Round((FLOAT)x * viewport.clipFactor.x);
						*write++ = read[pos];
					}
				}
			}

			Memory::Free(memory);
		}
		else
			GLReadPixels(rectangle.x, viewport.height - rectangle.height - rectangle.y, rectangle.width, rectangle.height, format, type, data);

		SwapRows(&rectangle, data, bytesPerPixel);

		return TRUE;
	}

	BOOL THRASHAPI Write(ThrashRectangle rectangle, VOID* data)
	{
		DWORD bytesPerPixel = !forced.movies16Bit ? selectedResolution->colorDepth >> 3 : 2;
		return Write(rectangle, data, rectangle.width * bytesPerPixel);
	}

	BOOL THRASHAPI Write(ThrashRectangle rectangle, VOID* data, DWORD bytesPerRow)
	{
		DWORD bytesPerPixel = bytesPerRow / rectangle.width;

		ThrashColorFormatIndex format;
		switch (bytesPerPixel)
		{
		case 3:
			format = BGR_24;
			break;

		case 4:
			format = BGRA_32;
			break;

		default:
			format = RGB565_16;
			break;
		}

		ThrashTexture* texture = Texture::Allocate(about.textureWidthMax, about.textureHeightMax, format, INDEXED_NONE, 0);
		if (texture)
		{
			DWORD step = about.textureWidthMax * bytesPerPixel;
			DWORD memorySize = step * about.textureHeightMax;
			VOID* memory = Memory::Allocate(memorySize);

			if (memory)
			{
				DWORD stateTexture = State::Get(State::SetTexture);
				DWORD stateShadeModel = State::Get(State::ShadeModel);
				DWORD stateVertexLayout = State::Get(State::VertexLayout);
				DWORD stateCullFace = State::Get(State::CullFace);
				DWORD stateTextureClamp = State::Get(State::TextureClamp);
				DWORD stateTextureMipMap = State::Get(State::TextureMipMap);
				DWORD stateAlphaBlend = State::Get(State::EnableAlphaBlend);
				DWORD stateDepthBuffer = State::Get(State::EnableDepthBuffer);
				DWORD stateTexturesCombine = State::Get(State::TexturesCombine);

				State::Set(State::SetTexture, (DWORD)texture);
				State::Set(State::ShadeModel, 0);
				State::Set(State::VertexLayout, FALSE);
				State::Set(State::CullFace, 0);
				State::Set(State::TextureClamp, 0);
				State::Set(State::TextureMipMap, FALSE);
				State::Set(State::EnableAlphaBlend, FALSE);
				State::Set(State::EnableDepthBuffer, FALSE);

				GLTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

				ThrashVertexV1 vertex1;
				*(DWORD*)(&vertex1.diffuseColor) = 0;
				*(DWORD*)(&vertex1.specularColor) = 0;
				vertex1.vertCoord.z = 0.0;
				vertex1.vertCoord.rhw = 1.0;
				vertex1.texCoord0.u = 0.0;
				vertex1.texCoord0.v = 0.0;

				ThrashVertexV1 vertex2 = vertex1;
				vertex2.texCoord0.u = 1.0;
				vertex2.texCoord0.v = 0.0;

				ThrashVertexV1 vertex3 = vertex1;
				vertex3.texCoord0.u = 1.0;
				vertex3.texCoord0.v = 1.0;

				ThrashVertexV1 vertex4 = vertex1;
				vertex4.texCoord0.u = 0.0;
				vertex4.texCoord0.v = 1.0;

				for (DWORD y = 0; y < rectangle.height; y += about.textureHeightMax)
				{
					DWORD maxY = rectangle.height - y;
					if (maxY > about.textureHeightMax)
						maxY = about.textureHeightMax;

					for (DWORD x = 0; x < rectangle.width; x += about.textureWidthMax)
					{
						DWORD maxX = rectangle.width - x;
						if (maxX > about.textureWidthMax)
						{
							maxX = about.textureWidthMax;
							if (maxY != about.textureHeightMax)
								memset(memory, 0, memorySize);
						}
						else if (maxX != about.textureWidthMax)
							memset(memory, 0, memorySize);

						BYTE* read = (BYTE*)data + y * bytesPerRow + x * bytesPerPixel;
						BYTE* write = (BYTE*)memory;

						for (DWORD i = 0; i < maxY; ++i, read += bytesPerRow, write += step)
							memcpy(write, read, maxX * bytesPerPixel);

						Texture::Update(texture, memory, NULL);

						vertex1.vertCoord.x = (rectangle.x + x);
						vertex1.vertCoord.y = (rectangle.y + y);

						vertex2.vertCoord.x = (rectangle.x + x + about.textureWidthMax);
						vertex2.vertCoord.y = (rectangle.y + y);

						vertex3.vertCoord.x = (rectangle.x + x + about.textureWidthMax);
						vertex3.vertCoord.y = (rectangle.y + y + about.textureHeightMax);

						vertex4.vertCoord.x = (rectangle.x + x);
						vertex4.vertCoord.y = (rectangle.y + y + about.textureHeightMax);

						Quad::Draw(&vertex1, &vertex2, &vertex3, &vertex4);
					}
				}

				Window::Flush();

				State::Set(State::SetTexture, stateTexture);
				State::Set(State::ShadeModel, stateShadeModel);
				State::Set(State::VertexLayout, stateVertexLayout);
				State::Set(State::CullFace, stateCullFace);
				State::Set(State::TextureClamp, stateTextureClamp);
				State::Set(State::TextureMipMap, stateTextureMipMap);
				State::Set(State::EnableAlphaBlend, stateAlphaBlend);
				State::Set(State::EnableDepthBuffer, stateDepthBuffer);
				State::Set(State::TexturesCombine, 0xFFFF0000, stateTexturesCombine);

				Memory::Free(memory);
				Texture::Free(texture);
			}
			else
			{
				Texture::Free(texture);
				Main::ShowError("Out of memory.", __FILE__, "Write", __LINE__);
			}
		}

		return TRUE;
	}
}