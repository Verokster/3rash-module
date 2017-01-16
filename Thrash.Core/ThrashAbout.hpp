#ifndef _THRASH_TYPE_ABOUT_HPP_
#define _THRASH_TYPE_ABOUT_HPP_

struct ThrashAbout
{
	DWORD signature;
	DWORD size;
	DWORD version;
	DWORD flags;
	DWORD textureWidthMin;
	DWORD textureWidthMax;
	DWORD textureWidthMultiple;
	DWORD textureHeightMin;
	DWORD textureHeightMax;
	DWORD textureHeightMultiple;

	DWORD clipAlign;
	DWORD texFormatsCount;
	ThrashTexColorFormats* texFormats;
	DWORD texIndexFormatsCount;
	ThrashTexColorIndexFormats* texIndexFormats;
	DWORD resolutionsCount;
	ThrashResolution* resolutionsList;
	DWORD numStages;

	CHAR driverName[36];
	DWORD subType;
	DWORD textureRamSize;
	DWORD textureRamType;

	CHAR* author;
};

#endif