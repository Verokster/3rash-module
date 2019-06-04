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

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include "windows.h"
#include "math.h"
#include "wtypesbase.h"

extern "C" _CRTIMP int __cdecl sprintf(char*, const char*, ...);

#define MemoryAlloc(size) malloc(size)
#define MemoryFree(block) free(block)
#define MemorySet(dst, val, size) memset(dst, val, size)
#define MemoryZero(dst, size) memset(dst, 0, size)
#define MemoryCopy(dst, src, size) memcpy(dst, src, size)
#define MathCeil(x) ceil(x)
#define MathFloor(x) floor(x)
#define MathPow(x, y) pow(x, y)
#define StrPrint(buf, fmt, ...) sprintf(buf, fmt, __VA_ARGS__)
#define StrStr(str, substr) strstr(str, substr)
#define StrCopy(dst, src) strcpy(dst, src)
#define StrCat(dst, src) strcat(dst, src)
#define StrLastChar(str, ch) strrchr(str, ch)
#define StrToInt(str) atoi(str)
#define Exit(code) exit(code)

DOUBLE __fastcall MathRound(DOUBLE);