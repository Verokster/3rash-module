#pragma once

enum PrimitiveType
{
	PT_POINTLIST = 1,
	PT_LINELIST = 2,
	PT_LINESTRIP = 3,
	PT_TRIANGLELIST = 4,
	PT_TRIANGLESTRIP = 5,
	PT_TRIANGLEFAN = 6
};

struct ThrashPrimitive
{
	PrimitiveType type;
	DWORD  vertexType; // 708 - 0x2C4 - D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2
	ThrashVertex* vertexArray;
	DWORD  vertexCount;
};

struct ThrashPrimitiveList : ThrashPrimitive
{
	DWORD  dwFlags; // 0
};

struct ThrashPrimitiveIndexed : ThrashPrimitive
{
	WORD* indices;
	DWORD  indicesCount;
	DWORD  dwFlags; // 0
};