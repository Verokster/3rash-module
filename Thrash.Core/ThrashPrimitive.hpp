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

enum PrimitiveType
{
	PT_POINTLIST = 1,
	PT_LINELIST = 2,
	PT_LINESTRIP = 3,
	PT_TRIANGLELIST = 4,
	PT_TRIANGLESTRIP = 5,
	PT_TRIANGLEFAN = 6
};

struct ThrashPrimitive
{
	PrimitiveType type;
	DWORD  vertexType; // 708 - 0x2C4 - D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2
	ThrashVertex* vertexArray;
	DWORD  vertexCount;
};

struct ThrashPrimitiveList : ThrashPrimitive
{
	DWORD  dwFlags; // 0
};

struct ThrashPrimitiveIndexed : ThrashPrimitive
{
	WORD* indices;
	DWORD  indicesCount;
	DWORD  dwFlags; // 0
};