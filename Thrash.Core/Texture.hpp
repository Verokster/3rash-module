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

#pragma once

namespace Texture
{
	VOID* __fastcall Convert_BGR_4_To_RGB_24(ThrashTexture* texture, VOID* memory, BYTE pallete[]);
	VOID* __fastcall Convert_BGR_4_To_RGBA_32(ThrashTexture* texture, VOID* memory, BYTE pallete[]);
	VOID* __fastcall Convert_BGRA_4_To_RGBA_32(ThrashTexture* texture, VOID* memory, BYTE pallete[]);
	VOID* __fastcall Convert_BGR_8_To_RGB_24(ThrashTexture* texture, VOID* memory, BYTE pallete[]);
	VOID* __fastcall Convert_BGR_8_To_RGBA_32(ThrashTexture* texture, VOID* memory, BYTE pallete[]);
	VOID* __fastcall Convert_BGRA_8_To_RGBA_32(ThrashTexture* texture, VOID* memory, BYTE pallete[]);
	VOID* __fastcall Convert_BGR5_A1_To_RGBA_32(ThrashTexture* texture, VOID* memory);
	VOID* __fastcall Convert_RGB565_To_RGB_24(ThrashTexture* texture, VOID* memory);
	VOID* __fastcall Convert_RGB565_To_RGBA_32(ThrashTexture* texture, VOID* memory);
	VOID* __fastcall Convert_BGR_24_To_RGB_24(ThrashTexture* texture, VOID* memory);
	VOID* __fastcall Convert_BGR_24_To_RGBA_32(ThrashTexture* texture, VOID* memory);
	VOID* __fastcall Convert_BGRA_32_To_RGBA_32(ThrashTexture* texture, VOID* memory);
	VOID* __fastcall Convert_BGRA_16_To_RGBA_32(ThrashTexture* texture, VOID* memory);

	LPTHRASHTEXTURE THRASHAPI Allocate(DWORD width, DWORD height, ThrashColorFormat colorFormat, ThrashIndexFormat indexFormat, DWORD level);
	BOOL THRASHAPI Set(ThrashTexture* texture);
	LPTHRASHTEXTURE THRASHAPI Update(ThrashTexture* texture, VOID* memory, BYTE* pallete);
	BOOL THRASHAPI Free(ThrashTexture* texture);
	BOOL THRASHAPI Reset();
}