#ifndef _THRASH_TYPE_BUFFER_HPP_
#define _THRASH_TYPE_BUFFER_HPP_

struct ThrashBuffer
{
	GLuint name;

	GLuint vName;
	DWORD vSize;
	
	ThrashBufferVertex* vData;

	GLuint iName;
	DWORD iSize;
	WORD* iData;

	BOOL isIndexed;
	DWORD type;
};

#endif