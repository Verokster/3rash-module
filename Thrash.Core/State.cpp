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