#ifndef _THRASH_LINE_HPP_
#define _THRASH_LINE_HPP_

namespace Line
{
	VOID __stdcall Draw(ThrashVertex* vertex1, ThrashVertex* vertex2);
	VOID __stdcall DrawStrip(DWORD count, ThrashVertex vertexArray[]);
	VOID __stdcall DrawStrip(DWORD count, ThrashVertex vertexPtrArray[], DWORD indexes[]);
	VOID __stdcall DrawMesh(DWORD count, ThrashVertex vertexPtrArray[], DWORD indexes[]);
}

#endif