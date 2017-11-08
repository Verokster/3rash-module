#pragma once

namespace Memory
{
	VOID* __fastcall Allocate(size_t size);
	VOID __fastcall Free(VOID* memory);
}