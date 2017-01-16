#ifndef _THRASH_SPRITE_HPP_
#define _THRASH_SPRITE_HPP_

namespace Sprite
{
	VOID __stdcall Draw(ThrashVertex* vertex1, ThrashVertex* vertex2);
	VOID __stdcall DrawMesh(DWORD count, ThrashVertex vertexPtrArray[], DWORD indexes[]);
}

#endif