#include "stdafx.h"
#include "Thrash.hpp"

namespace Memory
{
	VOID* __fastcall Allocate(size_t size)
	{
		if (functions.Malloc != NULL)
			return (VOID*)functions.Malloc(size);
		else
			return malloc(size);
	}

	VOID __fastcall Free(VOID* memory)
	{
		if (functions.Malloc != NULL)
			functions.Free(memory);
		else
			free(memory);
	}
}