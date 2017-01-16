#ifndef _THRASH_POINT_HPP_
#define _THRASH_POINT_HPP_

namespace Point
{
	VOID __stdcall Draw(ThrashVertex* vertex);
	VOID __stdcall DrawStrip(DWORD count, ThrashVertex vertexArray[]);
	VOID __stdcall DrawMesh(DWORD count, ThrashVertex vertexPtrArray[], DWORD indexes[]);
}

#endif