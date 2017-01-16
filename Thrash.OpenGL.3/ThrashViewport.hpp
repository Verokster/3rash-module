#ifndef _THRASH_TYPE_VIEWPORT_HPP_
#define _THRASH_TYPE_VIEWPORT_HPP_

struct ThrashViewport
{
	DWORD width;
	DWORD height;
	ThrashRectangle rectangle;
	POINTFLOAT viewFactor;
	POINTFLOAT clipFactor;
};

#endif