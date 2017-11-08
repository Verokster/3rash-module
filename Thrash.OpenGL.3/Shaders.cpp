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

		GLuint id = 0;
		GLUniform1i(GLGetUniformLocation(shProgram, "tex01"), id++);
		GLUniform1i(GLGetUniformLocation(shProgram, "tex02"), id++);
		GLUniform1i(GLGetUniformLocation(shProgram, "tex03"), id++);
		GLUniform1i(GLGetUniformLocation(shProgram, "tex04"), id++);

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