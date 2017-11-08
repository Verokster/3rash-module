#pragma once

namespace Window
{
	VOID THRASHAPI Clear();
	VOID THRASHAPI Flush();
	BOOL THRASHAPI Window(DWORD bufferIndex);
	LPTHRASHWINDOW THRASHAPI Lock();
	BOOL THRASHAPI Unlock(ThrashWindow* window);
}