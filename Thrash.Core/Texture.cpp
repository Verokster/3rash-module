#include "stdafx.h"
#include "Thrash.hpp"

namespace Texture
{
	DWORD __fastcall GetPixelsCount(ThrashTexture* texture)
	{
		DWORD size = (*texture).width * (*texture).height;
		DWORD count = (*texture).level + 1;
		DWORD total = 0;

		do
		{
			total += size;
			size >>= 2;
			--count;
		}
		while (count);

		return total;
	}

	DWORD colorPalate16[] = {
		0xFF000000,		// 0 — black
		0xFF800000,		// 1 — maroon
		0xFF008000,		// 2 — green
		0xFF808000,		// 3 — olive
		0xFF000080,		// 4 — navy
		0xFF800080,		// 5 — purple
		0xFF008080,		// 6 — teal
		0xFFC0C0C0,		// 7 — silver
		0xFF808080,		// 8 — gray
		0xFFFF0000,		// 9 — red
		0xFF00FF00,		// 10 — lime
		0xFFFFFF00,		// 11 — yellow
		0xFF0000FF,		// 12 — blue
		0xFFFF00FF,		// 13 — fuchsia
		0xFF00FFFF,		// 14 — aqua
		0xFFFFFFFF		// 15 — white
	};

	VOID* __fastcall Convert_BGR_2_To_BGR_24(ThrashTexture* texture, VOID* memory, BYTE pallete[])
	{
		DWORD total = GetPixelsCount(texture);

		VOID* ptr = Memory::Allocate(total * 4);
		if (ptr)
		{
			BYTE* dest = (BYTE*)ptr;
			BYTE* source = (BYTE*)memory;

			if (pallete == NULL)
				pallete = (BYTE*)colorPalate16;

			BOOL lowPart = TRUE;
			do
			{
				DWORD index = lowPart ? (*source & 0x0F) : (*source++ >> 4);
				BYTE* color = &pallete[index];
				
				*dest++ = *color++;
				*dest++ = *color++;
				*dest++ = *color;

				lowPart = !lowPart;
				--total;
			}
			while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, __FUNCTION__, __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert_BGR_8_To_BGR_24(ThrashTexture* texture, VOID* memory, BYTE pallete[])
	{
		DWORD total = GetPixelsCount(texture);

		VOID* ptr = Memory::Allocate(total * 3);
		if (ptr)
		{
			BYTE* dest = (BYTE*)ptr;
			BYTE* source = (BYTE*)memory;
			do
			{
				BYTE* color = &pallete[*source * 3];

				*dest++ = *color++;
				*dest++ = *color++;
				*dest++ = *color;

				++source;
				--total;
			}
			while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, __FUNCTION__, __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert_BGRA_8_To_BGRA_32(ThrashTexture* texture, VOID* memory, BYTE pallete[])
	{
		DWORD total = GetPixelsCount(texture);

		VOID* ptr = Memory::Allocate(total * 4);
		if (ptr)
		{
			BYTE* dest = (BYTE*)ptr;
			BYTE* source = (BYTE*)memory;
			do
			{
				BYTE* color = &pallete[*source << 2];

				*dest++ = *color++;
				*dest++ = *color++;
				*dest++ = *color++;
				*dest++ = *color;

				++source;
				--total;
			}
			while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, __FUNCTION__, __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert_BGR5_A1_To_BGRA_32(ThrashTexture* texture, VOID* memory)
	{
		DWORD total = GetPixelsCount(texture);

		VOID* ptr = Memory::Allocate(total * 4);
		if (ptr)
		{
			BYTE* dest = (BYTE*)ptr;
			WORD* source = (WORD*)memory;
			do
			{
				*dest++ = (*source >> 7) & 0xF8;
				*dest++ = (*source >> 2) & 0xF8;
				*dest++ = *source << 3;
				*dest++ = HIBYTE(*source) & 0x80 ? 0xFF : 0x00;

				++source;
				--total;
			}
			while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, __FUNCTION__, __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert_RGB565_To_RGB(ThrashTexture* texture, VOID* memory)
	{
		DWORD total = GetPixelsCount(texture);

		VOID* ptr = Memory::Allocate(total * 3);
		if (ptr)
		{
			BYTE* dest = (BYTE*)ptr;
			WORD* source = (WORD*)memory;
			do
			{
				*dest++ = (*source >> 8) & 0xF8;
				*dest++ = (*source >> 3) & 0xF0;
				*dest++ = *source << 3;

				++source;
				--total;
			}
			while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, __FUNCTION__, __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert_BGR_24_To_RGB_24(ThrashTexture* texture, VOID* memory)
	{
		DWORD total = GetPixelsCount(texture);

		VOID* ptr = Memory::Allocate(total * 3);
		if (ptr)
		{
			BYTE* dest = (BYTE*)ptr;
			BYTE* source = (BYTE*)memory;
			do
			{
				*dest++ = *(source + 2);
				*dest++ = *(source + 1);
				*dest++ = *source;

				source += 3;
				--total;
			}
			while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, __FUNCTION__, __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert_BGRA_32_To_RGBA_32(ThrashTexture* texture, VOID* memory)
	{
		DWORD total = GetPixelsCount(texture);

		VOID* ptr = Memory::Allocate(total * 4);
		if (ptr)
			memcpy(ptr, memory, total * 4);
		else
			Main::ShowError("Out of memory.", __FILE__, __FUNCTION__, __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert_BGRA4_To_RGBA_32(ThrashTexture* texture, VOID* memory)
	{
		DWORD total = GetPixelsCount(texture);

		VOID* ptr = Memory::Allocate(total * 4);
		if ( ptr )
		{
			BYTE* dest = (BYTE*)ptr;
			BYTE* source = (BYTE*)memory;
			total <<= 1;
			do
			{
				*dest++ = *source << 4;
				*dest++ = *source & 0xF0;

				++source;
				--total;
			}
			while (total);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, __FUNCTION__, __LINE__);

		return ptr;
	}

	/*VOID* __fastcall Convert1(ThrashTexture* texture, VOID* memory, BYTE* pallete)
	{
		DWORD total = GetPixelsCount(texture);

		VOID* ptr = Memory::Allocate(total * 2);
		if (ptr)
		{
			DWORD* dest = (DWORD*)ptr;
			BYTE* source = (BYTE*)memory;
			do
			{
				BYTE* color = &pallete[*source * 3];

				*dest = *color++ >> 3 << 3;
				*dest |= *color++ >> 2 << 5;
				*dest++ |= *color >> 3 << 8;

				++source;
				--total;
			}
			while (total);
		}
		else
			ShowError("Out of memory.", __FILE__, __FUNCTION__, __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert2(ThrashTexture* texture, VOID* memory, BYTE* pallete)
	{
		DWORD total = GetPixelsCount(texture);

		VOID* ptr = Memory::Allocate(total * 2);
		if (ptr)
		{
			BYTE* dest = (BYTE*)ptr;
			BYTE* source = (BYTE*)memory;
			do
			{
				BYTE* color = &pallete[*source << 2];
				
				*dest = *color++ >> 4;
				*dest++ |= (*color++ >> 4 << 4);
				*dest = *color++ >> 4;
				*dest++ |= (*color >> 4 << 4);

				++source;
				--total;
			}
			while (total);
		}
		else
			ShowError("Out of memory.", __FILE__, __FUNCTION__, __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert5(ThrashTexture* texture, VOID* memory)
	{
		DWORD total = GetPixelsCount(texture);

		VOID* ptr = Memory::Allocate(total * 2);
		if (ptr)
		{
			DWORD* dest = (DWORD*)ptr;
			BYTE* source = (BYTE*)memory;
			do
			{
				*dest = *source++ >> 3 << 3;
				*dest |= *source++ >> 2 << 5;
				*dest++ |= *source++ >> 3 << 8;

				--total;
			}
			while (total);
		}
		else
			ShowError("Out of memory.", __FILE__, __FUNCTION__, __LINE__);

		return ptr;
	}

	VOID* __fastcall Convert6(ThrashTexture* texture, VOID* memory)
	{
		DWORD total = GetPixelsCount(texture);

		VOID* ptr = Memory::Allocate(total * 2);
		if (ptr)
		{
			BYTE* dest = (BYTE*)ptr;
			BYTE* source = (BYTE*)memory;
			do
			{
				*dest = *source++ >> 4;
				*dest++ |= (*source++ >> 4 << 4);
				*dest = *source++ >> 4;
				*dest++ |= (*source++ >> 4 << 4);

				--total;
			}
			while (total);
		}
		else
			ShowError("Out of memory.", __FILE__, __FUNCTION__, __LINE__);

		return ptr;
	}*/
}