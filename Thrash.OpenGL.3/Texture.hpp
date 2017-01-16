#ifndef _THRASH_OGL_TEXTURE_HPP_
#define _THRASH_OGL_TEXTURE_HPP_

namespace Texture
{
	VOID __fastcall ResetActive();
	VOID __fastcall ResetUsed();
	VOID __fastcall Bind(ThrashTexture* texture);
	VOID __fastcall CheckWrap();
	VOID* __fastcall Convert_BGR_2_To_BGR_24(ThrashTexture* texture, VOID* memory, BYTE pallete[]);
	VOID* __fastcall Convert_BGR_8_To_BGR_24(ThrashTexture* texture, VOID* memory, BYTE pallete[]);
	VOID* __fastcall Convert_BGRA_8_To_BGRA_32(ThrashTexture* texture, VOID* memory, BYTE pallete[]);
	VOID* __fastcall Convert_BGR5_A1_To_RGBA_32(ThrashTexture* texture, VOID* memory);
	VOID* __fastcall Convert_RGB565_To_RGB(ThrashTexture* texture, VOID* memory);
	VOID* __fastcall Convert_BGR_24_To_RGB_24(ThrashTexture* texture, VOID* memory);
	VOID* __fastcall Convert_BGRA_32_To_RGBA_32(ThrashTexture* texture, VOID* memory);
	VOID* __fastcall Convert_BGRA4_To_RGBA_32(ThrashTexture* texture, VOID* memory);
}

#endif