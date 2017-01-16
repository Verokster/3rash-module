#include "stdafx.h"
#include "Thrash.hpp"

namespace Memory
{
	VOID* __fastcall Allocate(size_t size)
	{
		if (funcMalloc != NULL)
			return (VOID*)funcMalloc(size);
		else
			return malloc(size);
	}

	VOID __fastcall Free(VOID* memory)
	{
		if (funcMalloc != NULL)
			funcFree(memory);
		else
			free(memory);
	}
}