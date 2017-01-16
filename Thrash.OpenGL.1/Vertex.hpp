#ifndef _THRASH_VERTEX_HPP_
#define _THRASH_VERTEX_HPP_

#include "GL.hpp"
#include "Thrash.hpp"

namespace Vertex
{
	VOID __fastcall Draw(ThrashVertex* vertex);
	VOID __fastcall DrawArray(GLenum mode, DWORD step, DWORD count, ThrashVertex vertexArray[]);
	VOID __fastcall DrawIndexedArray(GLenum mode, DWORD step, DWORD count, ThrashVertex vertexArray[], DWORD indexes[]);
}

#endif