#include "stdafx.h"
#include "Global.hpp"

namespace Rect
{
	VOID __inline SwapRows(ThrashRectangle* rectangle, VOID* data, DWORD bytesPerPixel)
	{
		DWORD height = (*rectangle).height;
		DWORD bytesPerRow = bytesPerPixel * (*rectangle).width;
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

	BOOL __stdcall Read(ThrashRectangle rectangle, VOID* data)
	{
		DWORD bytePerPixel;
		GLenum format, type;

		if ((*selectedResolution).colorDepth == 32)
		{
			bytePerPixel = 4;
			format = GL_BGRA;
			type = GL_UNSIGNED_BYTE;
		}
		else
		{
			bytePerPixel = 2;
			format = GL_RGB;
			type = GL_UNSIGNED_SHORT_5_6_5;
		}

		if (viewport.width != (*selectedResolution).width || viewport.height != (*selectedResolution).height)
		{
			DWORD width = Main::Round((FLOAT)rectangle.width * viewport.clipFactor.x);
			DWORD height = Main::Round((FLOAT)rectangle.height * viewport.clipFactor.y);

			DWORD count = width * height;
			VOID* memory = Memory::Allocate(count * bytePerPixel);

			GLReadPixels(viewport.rectangle.x + Main::Round(rectangle.x * viewport.clipFactor.x), viewport.height - (viewport.rectangle.y + Main::Round((rectangle.height + rectangle.y) * viewport.clipFactor.y)), width, height, format, type, memory);

			if (bytePerPixel == 4)
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

		SwapRows(&rectangle, data, bytePerPixel);

		return TRUE;
	}

	BOOL __stdcall Write(ThrashRectangle rectangle, VOID* data)
	{
		return Write(rectangle, data, rectangle.width * ((*selectedResolution).colorDepth == 32 ? 4 : 2));
	}

	BOOL __stdcall Write(ThrashRectangle rectangle, VOID* data, DWORD bytesPerRow)
	{
		DWORD bytesPerPixel = bytesPerRow / rectangle.width;
		
		if (bytesPerPixel == 4)
		{
			DWORD count = rectangle.width * rectangle.height;
			BYTE* read = (BYTE*)data + 3;
			do
			{
				*read = 0xFF;
				read += 4;
				--count;
			} while (count);
		}

		ThrashVertex vertex1;
		*(DWORD*)(&vertex1.diffuseColor) = 0xFFFFFFFF;
		vertex1.vertCoord.z = 1.0;
		vertex1.vertCoord.rhw = 1.0;
		vertex1.texCoord.u = 0.0;
		vertex1.texCoord.v = 0.0;

		ThrashVertex vertex2 = vertex1;
		vertex2.texCoord.u = 1.0;
		vertex2.texCoord.v = 0.0;

		ThrashVertex vertex3 = vertex1;
		vertex3.texCoord.u = 1.0;
		vertex3.texCoord.v = 1.0;

		ThrashVertex vertex4 = vertex1;
		vertex4.texCoord.u = 0.0;
		vertex4.texCoord.v = 1.0;

		DWORD stateCullFace = State::Get(ThrashState::CullFace);
		DWORD stateTextureClamp = State::Get(ThrashState::TextureWrap);
		DWORD stateTextureFilter = State::Get(ThrashState::TextureFilter);
		DWORD stateTextureMipMap = State::Get(ThrashState::TextureMipMap);

		State::Set(ThrashState::CullFace, 0);
		State::Set(ThrashState::TextureWrap, 0);
		State::Set(ThrashState::TextureFilter, 1);
		State::Set(ThrashState::TextureMipMap, FALSE);
		
		const DWORD COUNTER = 256;

		ThrashColorFormatIndex format;
		switch (bytesPerPixel)
		{
			case 2:
				format = RGB565_16;
				break;

			case 3:
				format = BGR_24;
				break;

			case 4:
				format = BGRA_32;
				break;

			default: break;
		}
		ThrashTexture* texture = Texture::Allocate(COUNTER, COUNTER, format, 0, NULL);
		Texture::Set(texture);

		DWORD step = COUNTER * bytesPerPixel;
		DWORD memorySize = step * COUNTER;
		VOID* memory = Memory::Allocate(memorySize);

		for (DWORD y = 0; y < rectangle.height; y += COUNTER)
		{
			DWORD maxY = rectangle.height - y;
			if (maxY > COUNTER)
				maxY = COUNTER;

			for (DWORD x = 0; x < rectangle.width; x += COUNTER)
			{
				DWORD maxX = rectangle.width - x;
				if (maxX > COUNTER)
				{
					maxX = COUNTER;
					if (maxY != COUNTER)
						memset(memory, 0, memorySize);
				}
				else if (maxX != COUNTER)
				{
					memset(memory, 0, memorySize);
				}
				
				BYTE* read = (BYTE*)data + y * bytesPerPixel * rectangle.width + x * bytesPerPixel;
				BYTE* write = (BYTE*)memory;

				for (DWORD i = 0; i < maxY; ++i,
					read += rectangle.width * bytesPerPixel,
					write += step)
					memcpy(write, read, maxX * bytesPerPixel);

				Texture::Update(texture, memory, NULL);

				vertex1.vertCoord.x = (rectangle.x + x);
				vertex1.vertCoord.y = (rectangle.y + y);

				vertex2.vertCoord.x = (rectangle.x + x + COUNTER);
				vertex2.vertCoord.y = (rectangle.y + y);

				vertex3.vertCoord.x = (rectangle.x + x + COUNTER);
				vertex3.vertCoord.y = (rectangle.y + y + COUNTER);

				vertex4.vertCoord.x = (rectangle.x + x);
				vertex4.vertCoord.y = (rectangle.y + y + COUNTER);

				Quad::Draw(&vertex1, &vertex2, &vertex3, &vertex4);
			}
		}

		Memory::Free(memory);
		Texture::Free(texture);

		State::Set(ThrashState::CullFace, stateCullFace);
		State::Set(ThrashState::TextureWrap, stateTextureClamp);
		State::Set(ThrashState::TextureFilter, stateTextureFilter);
		State::Set(ThrashState::TextureMipMap, stateTextureMipMap);

		return TRUE;
	}
}