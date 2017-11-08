#include "stdafx.h"
#include "Global.hpp"

ThrashWindow* windowObject;

namespace Window
{
	BOOL __inline Write(ThrashWindow* window)
	{
		DWORD bytesPerPixel = window->bytesPerRow / window->width;

		ThrashColorFormatIndex format;
		switch (bytesPerPixel)
		{
		case 3:
			format = BGR_24;
			break;

		case 4:
			format = BGRA_32;
			break;

		default:
			format = RGB565_16;
			break;
		}

		ThrashTexture* texture = Texture::Allocate(about.textureWidthMax, about.textureHeightMax, format, INDEXED_NONE, 0);
		if (texture)
		{
			DWORD step = about.textureWidthMax * bytesPerPixel;
			DWORD memorySize = step * about.textureHeightMax;
			VOID* memory = Memory::Allocate(memorySize);

			if (memory)
			{
				DWORD stateTexture = State::Get(State::SetTexture);
				DWORD stateShadeModel = State::Get(State::ShadeModel);
				DWORD stateVertexLayout = State::Get(State::VertexLayout);
				DWORD stateCullFace = State::Get(State::CullFace);
				DWORD stateTextureClamp = State::Get(State::TextureClamp);
				DWORD stateTextureMipMap = State::Get(State::TextureMipMap);
				DWORD stateAlphaBlend = State::Get(State::EnableAlphaBlend);
				DWORD stateDepthBuffer = State::Get(State::EnableDepthBuffer);
				//DWORD stateTexturesCombine = State::Get(State::TexturesCombine);

				State::Set(State::SetTexture, (DWORD)texture);
				State::Set(State::ShadeModel, 0);
				State::Set(State::VertexLayout, FALSE);
				State::Set(State::CullFace, 0);
				State::Set(State::TextureClamp, 0);
				State::Set(State::TextureMipMap, FALSE);
				State::Set(State::EnableAlphaBlend, FALSE);
				State::Set(State::EnableDepthBuffer, FALSE);

				// TODO: combine

				ThrashVertexV1 vertex1;
				*(DWORD*)(&vertex1.diffuseColor) = 0xFFFFFFFF;
				vertex1.vertCoord.z = 0.0;
				vertex1.vertCoord.rhw = 1.0;
				vertex1.texCoord0.u = 0.0;
				vertex1.texCoord0.v = 0.0;

				ThrashVertexV1 vertex2 = vertex1;
				vertex2.texCoord0.u = 1.0;
				vertex2.texCoord0.v = 0.0;

				ThrashVertexV1 vertex3 = vertex1;
				vertex3.texCoord0.u = 1.0;
				vertex3.texCoord0.v = 1.0;

				ThrashVertexV1 vertex4 = vertex1;
				vertex4.texCoord0.u = 0.0;
				vertex4.texCoord0.v = 1.0;

				for (DWORD y = 0; y < window->height; y += about.textureHeightMax)
				{
					DWORD maxY = window->height - y;
					if (maxY > about.textureHeightMax)
						maxY = about.textureHeightMax;

					for (DWORD x = 0; x < window->width; x += about.textureWidthMax)
					{
						DWORD maxX = window->width - x;
						if (maxX > about.textureWidthMax)
						{
							maxX = about.textureWidthMax;
							if (maxY != about.textureHeightMax)
								memset(memory, 0, memorySize);
						}
						else if (maxX != about.textureWidthMax)
							memset(memory, 0, memorySize);

						BYTE* read = (BYTE*)window->data + y * window->bytesPerRow + x * bytesPerPixel;
						BYTE* write = (BYTE*)memory;

						for (DWORD i = 0; i < maxY; ++i, read += window->bytesPerRow, write += step)
							memcpy(write, read, maxX * bytesPerPixel);

						Texture::Update(texture, memory, NULL);

						vertex1.vertCoord.x = x;
						vertex1.vertCoord.y = y;

						vertex2.vertCoord.x = x + about.textureWidthMax;
						vertex2.vertCoord.y = y;

						vertex3.vertCoord.x = x + about.textureWidthMax;
						vertex3.vertCoord.y = y + about.textureHeightMax;

						vertex4.vertCoord.x = x;
						vertex4.vertCoord.y = y + about.textureHeightMax;

						Quad::Draw(&vertex1, &vertex2, &vertex3, &vertex4);
					}
				}

				Window::Flush();

				State::Set(State::SetTexture, stateTexture);
				State::Set(State::ShadeModel, stateShadeModel);
				State::Set(State::VertexLayout, stateVertexLayout);
				State::Set(State::CullFace, stateCullFace);
				State::Set(State::TextureClamp, stateTextureClamp);
				State::Set(State::TextureMipMap, stateTextureMipMap);
				State::Set(State::EnableAlphaBlend, stateAlphaBlend);
				State::Set(State::EnableDepthBuffer, stateDepthBuffer);
				//State::Set(State::TexturesCombine, stateTexturesCombine);

				Memory::Free(memory);
				Texture::Free(texture);
			}
			else
			{
				Texture::Free(texture);
				Main::ShowError("Out of memory.", __FILE__, "Write", __LINE__);
			}
		}
		else
			Main::ShowError("Out of memory.", __FILE__, "Write", __LINE__);

		return TRUE;
	}

	LPTHRASHWINDOW __fastcall Create()
	{
		if (!windowObject)
		{
			windowObject = (ThrashWindow*)Memory::Allocate(sizeof(ThrashWindow));
			if (windowObject)
			{
				windowObject->width = selectedResolution->width;
				windowObject->height = selectedResolution->height;

				DWORD bytesPerPixel = !forced.movies16Bit ? selectedResolution->colorDepth >> 3 : 2;
				windowObject->bytesPerRow = windowObject->width * bytesPerPixel;

				DWORD size = windowObject->height * windowObject->bytesPerRow;
				VOID* data = Memory::Allocate(size);
				if (data)
				{
					memset(data, NULL, size);

					windowObject->data = data;
					windowObject->unknown = 1;

					if (!forced.movies16Bit)
					{
						switch (selectedResolution->colorDepth)
						{
						case 32:
							windowObject->colorFormatIndex = BGRA_32;
							break;
						case 24:
							windowObject->colorFormatIndex = BGR_24;
							break;
						default:
							windowObject->colorFormatIndex = RGB565_16;
							break;
						}
					}
					else
						windowObject->colorFormatIndex = RGB565_16;

					State::Set(State::CurrentWindow, (DWORD)windowObject);
				}
				else
					Main::ShowError("Out of memory.", __FILE__, "Lock", __LINE__);
			}
			else
				Main::ShowError("Out of memory.", __FILE__, "Lock", __LINE__);
		}

		return windowObject;
	}

	BOOL __fastcall Release()
	{
		if (windowObject)
		{
			if (windowObject->data)
				Memory::Free(windowObject->data);

			Memory::Free(windowObject);
			windowObject = NULL;

			//State::Set(State::CurrentWindow, NULL);
		}

		return TRUE;
	}

	VOID THRASHAPI Clear()
	{
		DWORD bufferBit = 0;

		if (depthEnabled)
			bufferBit |= GL_DEPTH_BUFFER_BIT;

		if (stencilEnabled)
			bufferBit |= GL_STENCIL_BUFFER_BIT;

		if (colorMask)
		{
			bufferBit |= GL_COLOR_BUFFER_BIT;
			if (windowObject)
				memset(windowObject->data, NULL, windowObject->height * windowObject->bytesPerRow);
		}

		GLClear(bufferBit);
	}

	VOID THRASHAPI Flush()
	{
		Buffer::Draw();
		GLFlush();
	}

	BOOL THRASHAPI Window(DWORD bufferIndex)
	{
		State::Set(State::BufferMode, bufferIndex);

		GLenum buf;
		switch (bufferIndex)
		{
		case 0:
		case 1:
			buf = GL_FRONT;
			break;

		case 2:
		case 3:
			buf = GL_BACK;
			break;

		default:
			buf = GL_NONE;
			break;
		}

		GLReadBuffer(buf);
		GLDrawBuffer(buf);

		colorMask = bufferIndex != 3;

		return TRUE;
	}

	LPTHRASHWINDOW THRASHAPI Lock()
	{
		if (isWindowLocked)
			Main::ShowError("Lock called while locked", __FILE__, "Lock", __LINE__);

		isWindowLocked = TRUE;
		if (functions.LockStatus)
			functions.LockStatus(isWindowLocked);

		ThrashWindow* window = Create();
		window->buffer = bufferMode;
		return window;
	}

	BOOL THRASHAPI Unlock(ThrashWindow* window)
	{
		if (!isWindowLocked)
			return TRUE;

		Write(window);

		isWindowLocked = FALSE;
		if (functions.LockStatus)
			functions.LockStatus(isWindowLocked);

		return TRUE;
	}
}