#pragma once

namespace Texture
{
	VOID* __fastcall Convert_BGR_4_To_RGB_24(ThrashTexture* texture, VOID* memory, BYTE pallete[]);
	VOID* __fastcall Convert_BGRA_4_To_RGBA_32(ThrashTexture* texture, VOID* memory, BYTE pallete[]);
	VOID* __fastcall Convert_BGR_8_To_RGB_24(ThrashTexture* texture, VOID* memory, BYTE pallete[]);
	VOID* __fastcall Convert_BGRA_8_To_RGBA_32(ThrashTexture* texture, VOID* memory, BYTE pallete[]);
	VOID* __fastcall Convert_BGR5_A1_To_BGRA_32(ThrashTexture* texture, VOID* memory);
	VOID* __fastcall Convert_RGB565_To_RGB(ThrashTexture* texture, VOID* memory);
	VOID* __fastcall Convert_BGR_24_To_RGB_24(ThrashTexture* texture, VOID* memory);
	VOID* __fastcall Convert_BGRA_32_To_RGBA_32(ThrashTexture* texture, VOID* memory);
	VOID* __fastcall Convert_BGRA_16_To_RGBA_32(ThrashTexture* texture, VOID* memory);

	LPTHRASHTEXTURE THRASHAPI Allocate(DWORD width, DWORD height, ThrashColorFormatIndex colorFormatIndex, ThrashIndexFormatIndex indexFormatIndex, DWORD level);
	BOOL THRASHAPI Set(ThrashTexture* texture);
	LPTHRASHTEXTURE THRASHAPI Update(ThrashTexture* texture, VOID* memory, BYTE* pallete);
	BOOL THRASHAPI Free(ThrashTexture* texture);
	BOOL THRASHAPI Reset();
}