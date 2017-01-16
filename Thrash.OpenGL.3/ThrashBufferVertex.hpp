#ifndef _THRASH_TYPE_BUFFER_VERTEX_HPP_
#define _THRASH_TYPE_BUFFER_VERTEX_HPP_

struct ThrashBufferVertex
{
	ThrashVertCoord vertCoord;
	ThrashColor color;
	ThrashTexCoord texCoord;
	DWORD texUnit;
};

#endif