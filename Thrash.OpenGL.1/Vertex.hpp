#pragma once

namespace Vertex
{
	VOID __fastcall Draw(ThrashVertexV1* vertex);
	VOID __fastcall Draw(ThrashVertexV2* vertex);
	VOID __fastcall DrawArray(GLenum mode, DWORD step, DWORD count, ThrashVertex vertexArray[]);
	VOID __fastcall DrawIndexedArray(GLenum mode, DWORD step, DWORD count, ThrashVertex vertexArray[], DWORD indexes[]);
	VOID __fastcall DrawIndexedArray(GLenum mode, DWORD step, DWORD count, ThrashVertex vertexArray[], WORD indexes[]);
}