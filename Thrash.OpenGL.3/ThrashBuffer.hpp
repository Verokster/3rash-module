#pragma once

struct ThrashBuffer
{
	GLuint name;

	GLuint vName;
	DWORD vSize;
	
	ThrashVertexV1* vData;

	GLuint iName;
	DWORD iSize;
	WORD* iData;

	BOOL isIndexed;
	DWORD type;
};