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

DWORD stateValueArray[3][STATES_SIZE];

WORD stateKeyArray[6][2] = {{0,  70},
							{100, 110},
							{200, 215},
							{300, 315},
							{400, 415},
							{700, 715}};

namespace State
{
	INT __fastcall GetKeyIndex(DWORD key)
	{
		DWORD total = 0;
		for (WORD i = 0; i < sizeof(stateKeyArray) / sizeof(WORD); ++i)
		{
			if (key >= stateKeyArray[i][0] && key <= stateKeyArray[i][1])
				return total + (key - stateKeyArray[i][0]);

			total += stateKeyArray[i][1] - stateKeyArray[i][0];
		}

		return -1;
	}

	DWORD __fastcall Set(ThrashState key, DWORD value)
	{
		return Set(key, 0, value);
	}

	DWORD __stdcall Set(DWORD key, DWORD value)
	{
		return Set((ThrashState)LOWORD(key), HIWORD(key), value);
	}

	DWORD __fastcall Get(ThrashState key)
	{
		return Get(key, 0);
	}

	DWORD __stdcall Get(DWORD key)
	{
		return Get((ThrashState)LOWORD(key), HIWORD(key));
	}

	DWORD __fastcall Get(ThrashState key, DWORD tmu)
	{
		INT index = GetKeyIndex(key);
		if (index >= 0)
			return stateValueArray[tmu][index];

		return index;
	}
}