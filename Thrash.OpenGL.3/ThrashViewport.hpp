#pragma once

struct ThrashViewport
{
	DWORD width;
	DWORD height;
	ThrashRectangle rectangle;
	POINTFLOAT viewFactor;
	POINTFLOAT clipFactor;
};