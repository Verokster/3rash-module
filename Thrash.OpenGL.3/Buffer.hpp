#pragma once

namespace Buffer
{
	VOID __fastcall Create();
	VOID __fastcall Release();
	VOID __fastcall Draw();
	VOID __fastcall AddPoint(ThrashVertex* vertex);
	VOID __fastcall AddLine(ThrashVertex* vertex1, ThrashVertex* vertex2);
	VOID __fastcall AddTri(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3);
}