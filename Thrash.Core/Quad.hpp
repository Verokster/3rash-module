#ifndef _THRASH_QUAD_HPP_
#define _THRASH_QUAD_HPP_

namespace Quad
{
	VOID __stdcall Draw(ThrashVertex* vertex1, ThrashVertex* vertex2, ThrashVertex* vertex3, ThrashVertex* vertex4);
	VOID __stdcall DrawMesh(DWORD count, ThrashVertex vertexPtrArray[], DWORD indexes[]);
}

#endif