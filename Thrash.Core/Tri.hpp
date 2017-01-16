#ifndef _THRASH_TRI_HPP_
#define _THRASH_TRI_HPP_

namespace Tri
{
	VOID __stdcall Draw(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3);
	VOID __stdcall DrawStrip(DWORD count, ThrashVertex vertexArray[]);
	VOID __stdcall DrawStrip(DWORD count, ThrashVertex vertexPtrArray[], DWORD indexes[]);
	VOID __stdcall DrawFan(DWORD count, ThrashVertex vertexArray[]);
	VOID __stdcall DrawFan(DWORD count, ThrashVertex vertexPtrArray[], DWORD indexes[]);
	VOID __stdcall DrawMesh(DWORD count, ThrashVertex vertexPtrArray[], DWORD indexes[]);
}

#endif