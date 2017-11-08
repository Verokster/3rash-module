#pragma once

namespace State
{
	INT __fastcall GetKeyIndex(DWORD key);

	DWORD __fastcall Get(ThrashState key);
	DWORD __fastcall Get(ThrashState key, DWORD tmu);

	DWORD __fastcall Set(ThrashState key, DWORD value);
	DWORD __fastcall Set(ThrashState key, DWORD tmu, DWORD value);

	DWORD THRASHAPI Get(DWORD key);
	DWORD THRASHAPI Set(DWORD key, DWORD value);
}