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
		if (hResource == nullptr)
			Main::ShowError("FindResource failed", __FILE__, __FUNCTION__, __LINE__);

		HGLOBAL hResourceData = LoadResource(hDllModule, hResource);
		if (hResourceData == nullptr)
			Main::ShowError("LoadResource failed", __FILE__, __FUNCTION__, __LINE__);

		LPVOID pData = LockResource(hResourceData);
		if (pData == nullptr)
			Main::ShowError("LockResource failed", __FILE__, __FUNCTION__, __LINE__);

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
		uniShadeModelLoc = GLGetUniformLocation(shProgram, "shadeModel");
		uniAlphaEnabledLoc = GLGetUniformLocation(shProgram, "alphaEnabled");
		uniAlphaFuncLoc = GLGetUniformLocation(shProgram, "alphaFunc");
		uniFogModeLoc = GLGetUniformLocation(shProgram, "fogMode");
		uniFogColorLoc = GLGetUniformLocation(shProgram, "fogColor");
		uniFogStartLoc = GLGetUniformLocation(shProgram, "fogStart");
		uniFogEndLoc = GLGetUniformLocation(shProgram, "fogEnd");
		uniFogDensityLoc = GLGetUniformLocation(shProgram, "fogDensity");
		uniGammaLoc = GLGetUniformLocation(shProgram, "gamma");

		GLuint id = 0;
		GLUniform1i(GLGetUniformLocation(shProgram, "tex01"), id++);
		GLUniform1i(GLGetUniformLocation(shProgram, "tex02"), id++);
		GLUniform1i(GLGetUniformLocation(shProgram, "tex03"), id++);
		GLUniform1i(GLGetUniformLocation(shProgram, "tex04"), id++);

		attrCoordsLoc = GLGetAttribLocation(shProgram, "vCoord");
		attrColorLoc = GLGetAttribLocation(shProgram, "vColor");
		attrTexCoordsLoc = GLGetAttribLocation(shProgram, "vTexCoord");
		attrTexUnitLoc = GLGetAttribLocation(shProgram, "vTexUnit");
	}

	VOID __fastcall Release()
	{
		GLUseProgram(NULL);
		GLDeleteProgram(shProgram);
	}
}