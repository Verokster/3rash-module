#pragma once

enum ThrashAboutFlags
{
	About_LineWidth					= 1,
	About_TextureSquare				= 2,
	About_TextureWidthPowerOf2		= 4,
	About_TextureHeighPpowerOf2		= 8,
	About_Software					= 16,
	About_Windowed					= 32,
	About_GlobalClut				= 64,
	About_Trilinear2Pass			= 128
};