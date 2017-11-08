#pragma once

namespace Rect
{
	BOOL __fastcall ReadWindow(ThrashWindow* window);
	BOOL __fastcall WriteWindow(ThrashWindow* window);

	BOOL THRASHAPI Read(ThrashRectangle rectangle, VOID* data);
	
	BOOL THRASHAPI Write(ThrashRectangle rectangle, VOID* data);
	BOOL THRASHAPI Write(ThrashRectangle rectangle, VOID* data, DWORD bytesPerRow);
}