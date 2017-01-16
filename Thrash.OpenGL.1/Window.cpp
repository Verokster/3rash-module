#include "stdafx.h"
#include "Global.hpp"

namespace Window
{
	ThrashWindow currentWindow;

	VOID __stdcall Clear()
	{
		GLClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	VOID __stdcall Flush()
	{
		GLFlush();
	}

	BOOL __stdcall Window(DWORD bufferIndex)
	{
		State::Set(BufferMode, bufferIndex);

		GLenum buf;
		switch (bufferIndex)
		{
			case 1:
				buf = GL_FRONT;
				break;

			case 2:
				buf = GL_BACK;
				break;

			default:
				buf = GL_NONE;
				break;
		}
		GLDrawBuffer(buf);

		return TRUE;
	}

	ThrashWindow* __stdcall Lock()
	{
		Window::Unlock(NULL);
		isWindowLocked = TRUE;

		DWORD bytesPerRow = ((*selectedResolution).colorDepth == 32 ? 4 : 2) * (*selectedResolution).width;
		VOID* data = (DWORD*)Memory::Allocate(bytesPerRow * (*selectedResolution).height);
		if (data != NULL)
		{
			currentWindow.data = data;
			currentWindow.bytesPerRow = bytesPerRow;
			currentWindow.colorFormatIndex = (*selectedResolution).colorDepth == 32 ? BGRA_32 : RGB565_16;
			currentWindow.width = (*selectedResolution).width;
			currentWindow.height = (*selectedResolution).height;

			State::Set(CurrentWindow, (DWORD)&currentWindow);
		}
		else
			Main::ShowError("Out of memory.", __FILE__, __FUNCTION__, __LINE__);

		return &currentWindow;
	}

	BOOL __stdcall Unlock(ThrashWindow* window)
	{
		if (isWindowLocked)
		{
			if (window != NULL && (*window).data != NULL)
			{
				ThrashRectangle rectangle;
				rectangle.x = rectangle.y = 0;
				rectangle.width = (*window).width;
				rectangle.height = (*window).height;
				Rect::Write(rectangle, (*window).data);

				Memory::Free((*window).data);
			}

			isWindowLocked = FALSE;
		}

		return true;
	}
}