#pragma once

enum ThrashColorFormatIndex
{
	COLOR_NONE	= 0,
	INDEXED_4	= 1, //  4 bits
	INDEXED_8	= 2, //  8 bits
	BGR5_A1_16	= 3, // 16 bits
	RGB565_16	= 4, // 16 bits
	BGR_24		= 5, // 24 bits
	BGRA_32		= 6, // 32 bits
	BGRA4_16	= 7, // 16 bits
	L4_A4_8		= 8
};