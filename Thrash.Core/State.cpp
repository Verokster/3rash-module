#include "stdafx.h"
#include "Thrash.hpp"

DWORD stateValueArray[3840];

DWORD stateKeyArray[6][2] = {{0,  70},
							{100, 110},
							{200, 215},
							{300, 315},
							{400, 415},
							{700, 715}};

namespace State
{
	DWORD __fastcall GetKeyIndex(DWORD key)
	{
		key &= 0x0000FFFF;

		DWORD total = 0;
		for (DWORD i = 0; i < ARRAYSIZE(stateKeyArray); ++i)
		{
			if (key >= stateKeyArray[i][0] && key <= stateKeyArray[i][1])
				return total + (key - stateKeyArray[i][0]);

			total += stateKeyArray[i][1] - stateKeyArray[i][0];
		}

		return -1;
	}
}