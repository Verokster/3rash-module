#include "stdafx.h"
#include "Thrash.hpp"

namespace Main
{
	VOID __fastcall ShowError(CHAR* message, CHAR* file, CHAR* function, DWORD line)
	{
		if (funcErrorMessageBox != NULL)
		{
			CHAR dest[400];
			sprintf(dest, "%s:\n%s\n\n\nFILE %s\nLINE %d", function, message, file, line);
			funcErrorMessageBox(0, dest);
		}
	}

	DWORD __fastcall GetEnvironmentValue(DWORD def, CHAR* prefix, CHAR* name)
	{
		char* v3;
		char dest[80];

		sprintf(dest, "%s_%s", prefix, name);
		v3 = getenv(dest);
		if (v3 || (sprintf(dest, "THRASH_%s", name), (v3 = getenv(dest)) != 0))
			return atoi(v3);
		else
			return def;
	}

	VOID __fastcall LoadEnvironmentValues(ThrashAbout* about, CHAR* prefix)
	{
		(*about).signature = GetEnvironmentValue((*about).signature, prefix, "signature");
		(*about).version = GetEnvironmentValue((*about).version, prefix, "version");

		(*about).flags = ((*about).flags & 0xFE) | GetEnvironmentValue((*about).flags & 1, prefix, "linewidth");
		(*about).flags = ((*about).flags & 0xFD) | (GetEnvironmentValue((*about).flags >> 1 & 1, prefix, "texturesquare") << 1);
		(*about).flags = ((*about).flags & 0xFB) | (GetEnvironmentValue((*about).flags >> 2 & 1, prefix, "texturewidthpowerof2") << 2);
		(*about).flags = ((*about).flags & 0xF7) | (GetEnvironmentValue((*about).flags >> 3 & 1, prefix, "textureheightpowerof2") << 3);
		(*about).flags = ((*about).flags & 0xEF) | (GetEnvironmentValue((*about).flags >> 4 & 1, prefix, "software") << 4);
		(*about).flags = ((*about).flags & 0xDF) | (GetEnvironmentValue((*about).flags >> 5 & 1, prefix, "windowed") << 5);
		(*about).flags = ((*about).flags & 0xBF) | (GetEnvironmentValue((*about).flags >> 6 & 1, prefix, "globalclut") << 6);
		(*about).flags = ((*about).flags & 0x7F) | (GetEnvironmentValue((*about).flags >> 7 & 1, prefix, "trilinear2pass") << 7);

		(*about).textureWidthMin = GetEnvironmentValue((*about).textureWidthMin, prefix, "texturewidthmin");
		(*about).textureWidthMax = GetEnvironmentValue((*about).textureWidthMax, prefix, "texturewidthmax");
		(*about).textureWidthMultiple = GetEnvironmentValue((*about).textureWidthMultiple, prefix, "texturewidthmultiple");
		(*about).textureHeightMin = GetEnvironmentValue((*about).textureHeightMin, prefix, "textureheightmin");
		(*about).textureHeightMax = GetEnvironmentValue((*about).textureHeightMax, prefix, "textureheightmax");
		(*about).textureHeightMultiple = GetEnvironmentValue((*about).textureHeightMultiple, prefix, "textureheightmultiple");
		(*about).clipAlign = GetEnvironmentValue((*about).clipAlign, prefix, "clipalign");
		(*about).numStages = GetEnvironmentValue((*about).numStages, prefix, "numstages");
		(*about).subType = GetEnvironmentValue((*about).subType, prefix, "subtype");
		(*about).textureRamSize = GetEnvironmentValue((*about).textureRamSize, prefix, "textureramsize");
		(*about).textureRamType = GetEnvironmentValue((*about).textureRamType, prefix, "textureramtype");
	}
}