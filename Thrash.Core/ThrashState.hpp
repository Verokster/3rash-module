/*
	MIT License

	Copyright (c) 2017 Oleksiy Ryabchun

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#pragma once

namespace State
{
	enum ThrashState
	{
		SetTexture = 1,
		CullFace = 2,
		ClearColor = 3,
		EnableDepthBuffer = 4,
		EnableDither = 5,
		ShadeModel = 6,
		TextureFilter = 7,
		TextureLodBias = 8,
		EnableSmoothPolygon = 9,
		EnableAlphaBlend = 10,
		TextureMipMap = 11,
		ChromaColor = 12,
		TextureClamp = 13,
		FogDensity = 14,
		FogColor = 15,
		LineWidth = 16,
		FlatFans = 17,

		Functions = 19,
		EnableFog = 20,
		FogMode = 21,
		FogStart = 22,
		FogEnd = 23,
		DepthBias = 24,
		WindowHandle = 25,
		MessageBoxFunction = 26,
		EventFuncion = 27,
		LockStatusFunction = 28,
		CurrentWindow = 29,
		VertexSize = 30,
		ThrashApiVersion = 31,
		MallocFunction = 32,
		FreeFunction = 33,

		AlphaMode = 36,

		StateFunction = 39,
		DepthCompare = 40,
		TexturesCombine = 41,
		VertexLayout = 42,
		BackBufferType = 43,
		TextureColorPalette = 44,

		Gamma = 46,
		EnableStencilBuffer = 47,
		StencilFunc = 48,

		StencilFail = 52,
		StencilDepthFail = 53,
		StencilPass = 54,
		Unknown_55 = 55,
		BlendMode = 56,
		EnableDepthWrite = 57,
		DisplayMode = 58,
		BufferMode = 59,
		FlipRate = 60,
		ShamelessPlug = 61,

		AlphaCompare = 64,
		TextureClampS = 65,
		TextureClampT = 66,
		ClearDepth = 67,

		Texture1Filter = 73,
		Texture0Filter = 74,

		Gamma2 = 101,
		DepthBias2 = 102,
		SwapInterval = 103,
		BlendMode2 = 104,
		EnableFog2 = 105,
		EnableDepthWrite2 = 106,
		PassThru = 108,
		MaxPending = 109,
		LibHandle = 110,
		TextureUnits = 111,

		Hint = 304,
		LineDouble = 305,

		// ============================
		DeviceType = 400,
		Unknown_401 = 401,
		Unknown_403 = 403, // DeviceDescr structure (size = 188)
		BlendSourceFactor = 404,
		BlendDestinationFactor = 405,
		TexturesCombine2 = 406,
		Unknown_407 = 407,  // something for texture object, seems like TMU
		Unknown_408 = 408, // bool flag to indicate GetDeviceCaps or GetClientRect
		Antialiasing = 409, // glEnable(GL_MULTISAMPLE) && glSampleCoverage();
		TextureFactor = 410,
		Unknown_412 = 412, // return some callback
		Unknown_413 = 413, // return some callback

		Unknown_420 = 420, // bool flag when device type == 1
		Unknown_421 = 421, // switch 0|1|2|3
		Unknown_422 = 422,
		Unknown_423 = 423,
		Unknown_424 = 424,
		Unknown_425 = 425,
		Unknown_426 = 426,
		Unknown_427 = 427,
		Unknown_428 = 428,
		Unknown_429 = 429,

		Unknown_430 = 430,
		Unknown_431 = 431,
		Unknown_432 = 432,
		Unknown_433 = 433,
		Unknown_434 = 434,
		DrawPrimitives = 435,
		Unknown_436 = 436,
		Unknown_437 = 437,
		Unknown_438 = 438,
		Unknown_439 = 439,

		Unknown_440 = 440,
		Unknown_441 = 441,
		Unknown_442 = 442,
		Unknown_443 = 443,
		Unknown_444 = 444,
		Unknown_445 = 445,
		Unknown_446 = 446,
		Unknown_447 = 447,
		Unknown_449 = 449,

		Unknown_451 = 451, // bool flag for Texture combine

		Software = 703
	};
}