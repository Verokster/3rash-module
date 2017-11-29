#version 130

precision highp float;

uniform sampler2D tex01;

uniform bool texEnabled;
uniform bool alphaEnabled;
uniform uint alphaFunc;
uniform float alphaValue;
uniform bool shadeModel;
uniform uint fogMode;
uniform vec4 fogColor;
uniform bool specularEnabled;
uniform float gamma;

in vec4 fDiffuseSmooth;
flat in vec4 fDiffuseFlat;
in vec4 fSpecular;
in vec3 fTexCoord;
in float fogFactor;
out vec4 fragColor;

void main(void)
{
	fragColor = shadeModel ? fDiffuseSmooth : fDiffuseFlat;
	
	if (texEnabled)
		fragColor *= texture(tex01, fTexCoord.xy / fTexCoord.z);

	if (specularEnabled)
		fragColor += fSpecular;

	switch (alphaFunc)
	{
		case 0u: // GL_NEVER:
			discard;

		case 1u: // GL_LESS:
			if (fragColor.a >= alphaValue)
				discard;
			break;
		case 2u: // GL_EQUAL:
			if (fragColor.a != alphaValue)
				discard;
			break;
		case 3u: // GL_LEQUAL:
			if (fragColor.a > alphaValue)
				discard;
			break;
		case 4u: // GL_GREATER:
			if (fragColor.a <= alphaValue)
				discard;
			break;
		case 5u: // GL_NOTEQUAL:
			if (fragColor.a == alphaValue)
				discard;
			break;
		case 6u: // GL_GEQUAL:
			if (fragColor.a < alphaValue)
				discard;
			break;
		default: // GL_ALWAYS:
			break;
	}

	switch (fogMode)
	{
		case 2u:
		case 4u:
		case 8u:
			fragColor = vec4(mix(fragColor.rgb, fogColor.rgb, fogFactor), fragColor.a);
			break;

		default:
			break;
	}

	if (gamma != 1.0)
		fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / gamma));
}
