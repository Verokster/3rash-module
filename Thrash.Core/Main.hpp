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

namespace Main
{
	VOID __fastcall ShowError(const CHAR* message, const CHAR* file, const CHAR* function, DWORD line);
	DWORD __fastcall GetEnvironmentValue(DWORD def, const CHAR* prefix, const CHAR* name);
	VOID __fastcall LoadEnvironmentValues(ThrashAbout* about, const CHAR* prefix);
	// --------------- EXPORT --------------

	LPTHRASHABOUT THRASHAPI About();
	BOOL THRASHAPI Clip(RECT rect);
	VOID THRASHAPI Idle();
	BOOL THRASHAPI Init();
	DWORD THRASHAPI Is();
	VOID THRASHAPI PageFlip();
	BOOL THRASHAPI Restore();
	BOOL THRASHAPI SelectDisplay(DWORD index);
	BOOL THRASHAPI SetVideoMode(DWORD resolutionIndex, DWORD maxPanding, BOOL isDepthBuffer16);
	BOOL THRASHAPI Sync(DWORD value);
}