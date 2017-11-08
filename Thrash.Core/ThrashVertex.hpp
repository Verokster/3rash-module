#pragma once

struct ThrashVertex {};

struct ThrashVertexV1 : ThrashVertex
{
	ThrashVertCoord vertCoord;
	ThrashColor diffuseColor;
	ThrashColor specularColor;
	ThrashTexCoord texCoord0;
};

struct ThrashVertexV2 : ThrashVertexV1
{
	ThrashTexCoord texCoord1;
};