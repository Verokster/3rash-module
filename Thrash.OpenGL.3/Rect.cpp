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

namespace Rect
{
	VOID __fastcall SwapRows(DWORD width, DWORD height, VOID* data, DWORD bytesPerPixel)
	{
		DWORD bytesPerRow = bytesPerPixel * width;
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
			DWORD width = (DWORD)MathRound((FLOAT)rectangle.width * viewport.clipFactor.x);
			DWORD height = (DWORD)MathRound((FLOAT)rectangle.height * viewport.clipFactor.y);

			DWORD count = width * height;
			VOID* memory = Memory::Allocate(count * bytesPerPixel);

			GLReadPixels(viewport.rectangle.x + (DWORD)MathRound(rectangle.x * viewport.clipFactor.x), viewport.height - (viewport.rectangle.y + (DWORD)MathRound((rectangle.height + rectangle.y) * viewport.clipFactor.y)), width, height, format, type, memory);
			if (bytesPerPixel == 4)
			{
				DWORD* read = (DWORD*)memory;
				DWORD* write = (DWORD*)data;
				for (DWORD y = 0; y < rectangle.height; ++y)
				{
					DWORD step = (DWORD)MathRound((FLOAT)y * viewport.clipFactor.y) * width;
					for (DWORD x = 0; x < rectangle.width; ++x)
					{
						DWORD pos = step + (DWORD)MathRound((FLOAT)x * viewport.clipFactor.x);
						*write++ = read[pos];
					}
				}
			}
			else
			{
				WORD* read = (WORD*)memory;
				WORD* write = (WORD*)data;
				for (DWORD y = 0; y < rectangle.height; ++y)
				{
					DWORD step = (DWORD)MathRound((FLOAT)y * viewport.clipFactor.y) * width;
					for (DWORD x = 0; x < rectangle.width; ++x)
					{
						DWORD pos = step + (DWORD)MathRound((FLOAT)x * viewport.clipFactor.x);
						*write++ = read[pos];
					}
				}
			}

			Memory::Free(memory);
		}
		else
			GLReadPixels(rectangle.x, viewport.height - rectangle.height - rectangle.y, rectangle.width, rectangle.height, format, type, data);

		SwapRows(rectangle.width, rectangle.height, data, bytesPerPixel);

		return TRUE;
	}

	BOOL THRASHAPI Write(ThrashRectangle rectangle, VOID* data)
	{
		DWORD bytesPerPixel = !forced.movies16Bit ? selectedResolution->colorDepth >> 3 : 2;
		DWORD bytesPerRow = bytesPerPixel * rectangle.width;

		ThrashColorFormat format;
		switch (bytesPerPixel)
		{
		case 3:
			format = COLOR_RGB_888;
			break;

		case 4:
			format = COLOR_ARGB_8888;
			break;

		default:
			format = COLOR_RGB_565;
			break;
		}

		ThrashTexture* texture = Texture::Allocate(about.textureWidthMax, about.textureHeightMax, format, INDEX_NA, 0);
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
				//DWORD stateTexturesCombine = State::Get(State::TexturesCombine);

				State::Set(State::SetTexture, (DWORD)texture);
				State::Set(State::ShadeModel, 0);
				State::Set(State::VertexLayout, FALSE);
				State::Set(State::CullFace, 0);
				State::Set(State::TextureClamp, 0);
				State::Set(State::TextureMipMap, FALSE);
				State::Set(State::EnableAlphaBlend, FALSE);
				State::Set(State::EnableDepthBuffer, FALSE);

				// TODO: combine

				ThrashVertexV1 vertex1;
				*(DWORD*)(&vertex1.diffuseColor) = 0xFFFFFFFF;
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
								MemoryZero(memory, memorySize);
						}
						else if (maxX != about.textureWidthMax)
							MemoryZero(memory, memorySize);

						BYTE* read = (BYTE*)data + y * bytesPerRow + x * bytesPerPixel;
						BYTE* write = (BYTE*)memory;

						for (DWORD i = 0; i < maxY; ++i, read += bytesPerRow, write += step)
							MemoryCopy(write, read, maxX * bytesPerPixel);

						Texture::Update(texture, memory, NULL);

						vertex1.vertCoord.x = (FLOAT)(rectangle.x + x);
						vertex1.vertCoord.y = (FLOAT)(rectangle.y + y);

						vertex2.vertCoord.x = (FLOAT)(rectangle.x + x + about.textureWidthMax);
						vertex2.vertCoord.y = (FLOAT)(rectangle.y + y);

						vertex3.vertCoord.x = (FLOAT)(rectangle.x + x + about.textureWidthMax);
						vertex3.vertCoord.y = (FLOAT)(rectangle.y + y + about.textureHeightMax);

						vertex4.vertCoord.x = (FLOAT)(rectangle.x + x);
						vertex4.vertCoord.y = (FLOAT)(rectangle.y + y + about.textureHeightMax);

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
				//State::Set(State::TexturesCombine, stateTexturesCombine);

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