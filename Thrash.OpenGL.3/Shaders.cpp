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

#include "stdafx.h"
#include "Global.hpp"

CHAR* VERTEX_SHADER = "[VERTEX SHADER]";
CHAR* FRAGMENT_SHADER = "[FRAGMENT SHADER]";

GLuint shProgram;

GLuint vShader;
GLuint fShader;

namespace Shaders
{
	GLuint __fastcall CompileFromResource(DWORD name, GLenum shaderType)
	{
		HRSRC hResource = FindResource(hDllModule, MAKEINTRESOURCE(name), RT_RCDATA);
		if (!hResource)
			Main::ShowError("FindResource failed", __FILE__, "CompileFromResource", __LINE__);

		HGLOBAL hResourceData = LoadResource(hDllModule, hResource);
		if (!hResourceData)
			Main::ShowError("LoadResource failed", __FILE__, "CompileFromResource", __LINE__);

		LPVOID pData = LockResource(hResourceData);
		if (!pData)
			Main::ShowError("LockResource failed", __FILE__, "CompileFromResource", __LINE__);

		GLuint shader = GLCreateShader(shaderType);

		const GLchar* source[] = {static_cast<const GLchar*>(pData)};
		const GLint lengths[] = {SizeofResource(hDllModule, hResource)};
		GLShaderSource(shader, 1, source, lengths);

		GLint result;
		GLCompileShader(shader);
		GLGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			GLGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);

			if (result == 0)
				Main::ShowError("Compile shader failed", __FILE__, shaderType == GL_VERTEX_SHADER ? VERTEX_SHADER : FRAGMENT_SHADER, __LINE__);
			else
			{
				const DWORD size = 360;
				CHAR data[size];
				GLGetShaderInfoLog(shader, size, &result, data);
				Main::ShowError(data, __FILE__, shaderType == GL_VERTEX_SHADER ? VERTEX_SHADER : FRAGMENT_SHADER, __LINE__);
			}
		}

		return shader;
	}

	VOID __fastcall Prepare()
	{
		vShader = CompileFromResource(IDR_GLSL_VERTEX, GL_VERTEX_SHADER);
		fShader = CompileFromResource(IDR_GLSL_FRAGMENT, GL_FRAGMENT_SHADER);

		shProgram = GLCreateProgram();
		GLAttachShader(shProgram, vShader);
		GLAttachShader(shProgram, fShader);
		GLLinkProgram(shProgram);
		GLUseProgram(shProgram);

		uniMVPLoc = GLGetUniformLocation(shProgram, "mvp");
		uniTexEnabledLoc = GLGetUniformLocation(shProgram, "texEnabled");
		uniShadeModelLoc = GLGetUniformLocation(shProgram, "shadeModel");
		uniAlphaFuncLoc = GLGetUniformLocation(shProgram, "alphaFunc");
		uniAlphaValLoc = GLGetUniformLocation(shProgram, "alphaValue");

		uniFogEnabledLoc = GLGetUniformLocation(shProgram, "fogEnabled");
		uniFogModeLoc = GLGetUniformLocation(shProgram, "fogMode");
		uniFogColorLoc = GLGetUniformLocation(shProgram, "fogColor");
		uniFogStartLoc = GLGetUniformLocation(shProgram, "fogStart");
		uniFogEndLoc = GLGetUniformLocation(shProgram, "fogEnd");
		uniFogDensityLoc = GLGetUniformLocation(shProgram, "fogDensity");
		uniSpecularEnabledLoc = GLGetUniformLocation(shProgram, "specularEnabled");
		uniGammaLoc = GLGetUniformLocation(shProgram, "gamma");

		attrCoordsLoc = GLGetAttribLocation(shProgram, "vCoord");
		attrDiffuseLoc = GLGetAttribLocation(shProgram, "vDiffuse");
		attrSpecularLoc = GLGetAttribLocation(shProgram, "vSpecular");
		attrTexCoordsLoc = GLGetAttribLocation(shProgram, "vTexCoord");
	}

	VOID __fastcall Release()
	{
		GLUseProgram(NULL);
		GLDeleteProgram(shProgram);
	}
}