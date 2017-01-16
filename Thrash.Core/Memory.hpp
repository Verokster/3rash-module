#ifndef _THRASH_MEMORY_HPP_
#define _THRASH_MEMORY_HPP_

#include "stdafx.h"

namespace Memory
{
	VOID* __fastcall Allocate(size_t size);
	VOID __fastcall Free(VOID* memory);
}

#endif