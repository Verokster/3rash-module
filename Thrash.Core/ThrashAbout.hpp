#pragma once

enum ThrashAboutSubType
{
	DeviceGeneric = 0,
	DeviceVoodooRush = 1,
	DeviceVoodoo2 = 2,
	DeviceVoodooBanshee = 3,
	DeviceVoodoo3 = 4,
};

typedef struct ThrashAbout
{
	CHAR signature[4];							// 0
	DWORD size;									// 4
	DWORD version;								// 8
	DWORD flags;								// 12
	DWORD textureWidthMin;						// 16
	DWORD textureWidthMax;						// 20
	DWORD textureWidthMultiple;					// 24
	DWORD textureHeightMin;						// 28
	DWORD textureHeightMax;						// 32
	DWORD textureHeightMultiple;				// 36

	DWORD clipAlign;							// 40
	DWORD texFormatsCount;						// 44
	ThrashTexColorFormats* texFormats;			// 48
	DWORD texIndexFormatsCount;					// 52
	ThrashTexColorIndexFormats* texIndexFormats;// 56
	DWORD resolutionsCount;						// 60
	ThrashResolution* resolutionsList;			// 64
	DWORD numStages;							// 68	- Multitexturing
	DWORD unknown;								// 72
	CHAR driverName[32];						// 76

	// WORD										// 88

	ThrashAboutSubType subType;					// 108

	DWORD textureRamSize;						// 112
	DWORD textureRamType;						// 116

	CHAR* author;								// 120
	DWORD dxVeriosn;							// 124
	CHAR deviceName[32];						// 128
} *LPTHRASHABOUT;