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

enum ThrashColorFormat
{
	COLOR_NA					= 0,
	COLOR_INDEX_4				= 1,
	COLOR_INDEX_8				= 2,
	COLOR_ARGB_1555				= 3,
	COLOR_RGB_565				= 4,
	COLOR_RGB_888				= 5,
	COLOR_ARGB_8888				= 6,
	COLOR_ARGB_4444				= 7,
	COLOR_ALPHAINTENSITY_44		= 8,
	COLOR_ALPHAINDEX_88			= 9,
	COLOR_YUV_8888				= 10,
	COLOR_RGB_555				= 11,
	COLOR_TEX_565				= 12,
	COLOR_TEX_4565				= 13,
	COLOR_MPEG_555				= 14,
	COLOR_MPEG_888				= 15,
	COLOR_MPEG_4888				= 16,
	COLOR_MPEG_8888				= 17,
	COLOR_LAST					= 18
};