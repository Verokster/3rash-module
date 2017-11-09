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
#include "Thrash.hpp"

namespace Texture
{
	VOID* __fastcall Convert_BGR_4_To_RGB_24(ThrashTexture* texture, VOID* memory, BYTE pallete[])
	{
		VOID* ptr = Memory::Allocate(texture->pixels * texture->size);
		if (ptr)
		{
			BYTE* dest = (BYTE*)ptr;
			BYTE* source = (BYTE*)memory;

			BOOL lowPart = TRUE;
			DWORD total = texture->pixels;
			do
			{
				DWORD index = (lowPart ? (*source & 0x0F) : (*source++ >> 4));
				lowPart = !lowPart;

				BYTE* color = &pallete[*source * 3];

				*dest++ = *(color + 2);
				*dest++ = *(color + 1);
				*dest++ = *color;

				--total;
			} while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, "Convert_BGR_4_To_RGB_24", __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert_BGRA_4_To_RGBA_32(ThrashTexture* texture, VOID* memory, BYTE pallete[])
	{
		VOID* ptr = Memory::Allocate(texture->pixels * texture->size);
		if (ptr)
		{
			DWORD* dest = (DWORD*)ptr;
			BYTE* source = (BYTE*)memory;

			BOOL lowPart = TRUE;
			DWORD total = texture->pixels;
			do
			{
				DWORD index = (lowPart ? (*source & 0x0F) : (*source++ >> 4));
				lowPart = !lowPart;

				*dest++ = Color::Swap(*(DWORD*)&pallete[index << 2]);
				--total;
			} while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, "Convert_BGRA_4_To_RGBA_32", __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert_BGR_8_To_RGB_24(ThrashTexture* texture, VOID* memory, BYTE pallete[])
	{
		VOID* ptr = Memory::Allocate(texture->pixels * texture->size);
		if (ptr)
		{
			BYTE* dest = (BYTE*)ptr;
			BYTE* source = (BYTE*)memory;

			DWORD total = texture->pixels;
			do
			{
				BYTE* color = &pallete[*source++ * 3];

				*dest++ = *(color + 2);
				*dest++ = *(color + 1);
				*dest++ = *color;

				--total;
			} while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, "Convert_BGR_8_To_RGB_24", __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert_BGRA_8_To_RGBA_32(ThrashTexture* texture, VOID* memory, BYTE pallete[])
	{
		VOID* ptr = Memory::Allocate(texture->pixels * texture->size);
		if (ptr)
		{
			DWORD* dest = (DWORD*)ptr;
			BYTE* source = (BYTE*)memory;

			DWORD total = texture->pixels;
			do
			{
				*dest++ = Color::Swap(*(DWORD*)&pallete[*source++ << 2]);
				--total;
			} while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, "Convert_BGRA_8_To_RGBA_32", __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert_BGR5_A1_To_BGRA_32(ThrashTexture* texture, VOID* memory)
	{
		VOID* ptr = Memory::Allocate(texture->pixels * texture->size);
		if (ptr)
		{
			BYTE* dest = (BYTE*)ptr;
			WORD* source = (WORD*)memory;

			DWORD total = texture->pixels;
			do
			{
				*dest++ = (*source >> 7) & 0xF8;
				*dest++ = (*source >> 2) & 0xF8;
				*dest++ = *source << 3;
				*dest++ = HIBYTE(*source) & 0x80 ? 0xFF : 0x00;

				++source;
				--total;
			} while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, "Convert_BGR5_A1_To_BGRA_32", __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert_RGB565_To_RGB(ThrashTexture* texture, VOID* memory)
	{
		VOID* ptr = Memory::Allocate(texture->pixels * texture->size);
		if (ptr)
		{
			BYTE* dest = (BYTE*)ptr;
			WORD* source = (WORD*)memory;

			DWORD total = texture->pixels;
			do
			{
				*dest++ = (*source >> 8) & 0xF8;
				*dest++ = (*source >> 3) & 0xF0;
				*dest++ = *source << 3;

				++source;
				--total;
			} while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, "Convert_RGB565_To_RGB", __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert_BGR_24_To_RGB_24(ThrashTexture* texture, VOID* memory)
	{
		VOID* ptr = Memory::Allocate(texture->pixels * texture->size);
		if (ptr)
		{
			BYTE* dest = (BYTE*)ptr;
			BYTE* source = (BYTE*)memory;

			DWORD total = texture->pixels;
			do
			{
				*dest++ = *(source + 2);
				*dest++ = *(source + 1);
				*dest++ = *source;

				source += 3;
				--total;
			} while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, "Convert_BGR_24_To_RGB_24", __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert_BGRA_32_To_RGBA_32(ThrashTexture* texture, VOID* memory)
	{
		VOID* ptr = Memory::Allocate(texture->pixels * texture->size);
		if (ptr)
		{
			DWORD* dest = (DWORD*)ptr;
			DWORD* source = (DWORD*)memory;

			DWORD total = texture->pixels;
			do
			{
				*dest++ = Color::Swap(*source++);
				--total;
			} while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, "Convert_BGRA_32_To_RGBA_32", __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert_BGRA_16_To_RGBA_32(ThrashTexture* texture, VOID* memory)
	{
		VOID* ptr = Memory::Allocate(texture->pixels * texture->size);
		if (ptr)
		{
			DWORD* dest = (DWORD*)ptr;
			WORD* source = (WORD*)memory;

			DWORD total = texture->pixels;
			do
			{
				*dest++ = (*source & 0x0F00) >> 4 | (*source & 0x00F0) << 8 | (*source & 0x000F) << 20 | (*source & 0xF000) << 16;
				++source;
				--total;
			} while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, "Convert_BGRA_16_To_RGBA_32", __LINE__);

		return ptr;
	}

	/*VOID* __fastcall Convert1(ThrashTexture* texture, VOID* memory, BYTE* pallete)
	{
		VOID* ptr = Memory::Allocate(texture->pixels * texture->size);
		if (ptr)
		{
			DWORD* dest = (DWORD*)ptr;
			BYTE* source = (BYTE*)memory;

			DWORD total = texture->pixels;
			do
			{
				BYTE* color = &pallete[*source * 3];

				*dest = *color++ >> 3 << 3;
				*dest |= *color++ >> 2 << 5;
				*dest++ |= *color >> 3 << 8;

				++source;
				--total;
			} while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, "Convert1", __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert2(ThrashTexture* texture, VOID* memory, BYTE* pallete)
	{
		VOID* ptr = Memory::Allocate(texture->pixels * texture->size);
		if (ptr)
		{
			BYTE* dest = (BYTE*)ptr;
			BYTE* source = (BYTE*)memory;

			DWORD total = texture->pixels;
			do
			{
				BYTE* color = &pallete[*source << 2];

				*dest = *color++ >> 4;
				*dest++ |= (*color++ >> 4 << 4);
				*dest = *color++ >> 4;
				*dest++ |= (*color >> 4 << 4);

				++source;
				--total;
			} while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, "Convert2", __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert5(ThrashTexture* texture, VOID* memory)
	{
		VOID* ptr = Memory::Allocate(texture->pixels * texture->size);
		if (ptr)
		{
			DWORD* dest = (DWORD*)ptr;
			BYTE* source = (BYTE*)memory;

			DWORD total = texture->pixels;
			do
			{
				*dest = *source++ >> 3 << 3;
				*dest |= *source++ >> 2 << 5;
				*dest++ |= *source++ >> 3 << 8;

				--total;
			} while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, "Convert5", __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert6(ThrashTexture* texture, VOID* memory)
	{
		VOID* ptr = Memory::Allocate(texture->pixels * texture->size);
		if (ptr)
		{
			BYTE* dest = (BYTE*)ptr;
			BYTE* source = (BYTE*)memory;

			DWORD total = texture->pixels;
			do
			{
				*dest = *source++ >> 4;
				*dest++ |= (*source++ >> 4 << 4);
				*dest = *source++ >> 4;
				*dest++ |= (*source++ >> 4 << 4);

				--total;
			} while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, "Convert6", __LINE__);

		return ptr;
	}*/
}