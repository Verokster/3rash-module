#pragma once

struct ThrashBuffer
{
	GLuint name;
	GLuint vName;
	DWORD type;
	DWORD size;
	ThrashVertexV1* data;
};