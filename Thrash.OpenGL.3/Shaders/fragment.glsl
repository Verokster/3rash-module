#version 130

precision highp float;

uniform sampler2D tex01;
uniform sampler2D tex02;
uniform sampler2D tex03;
uniform sampler2D tex04;

uniform bool alphaEnabled;
uniform float alphaFunc;
uniform bool shadeModel;
uniform uint fogMode;
uniform vec4 fogColor;
uniform float gamma;

in vec4 fColorSmooth;
flat in vec4 fColorFlat;
in vec2 fTexCoord;
flat in uint fTexUnit;
in float fogFactor;
out vec4 fragColor;

void main(void)
{
	fragColor = shadeModel ? fColorSmooth : fColorFlat;

	switch (fTexUnit)
	{
		case 1u:
			fragColor = fragColor * texture(tex01, fTexCoord);
			break;
		case 2u:
			fragColor = fragColor * texture(tex02, fTexCoord);
			break;
		case 3u:
			fragColor = fragColor * texture(tex03, fTexCoord);
			break;
		case 4u:
			fragColor = fragColor * texture(tex04, fTexCoord);
			break;
		default:
			break;
	}

	if (alphaEnabled && fragColor.a < alphaFunc)
		discard;

	switch (fogMode)
	{
		case 2u:
		case 4u:
		case 8u:
			fragColor = vec4(mix(fragColor.xyz, fogColor.rgb, fogFactor), fragColor.a);
			break;

		default:
			break;
	}

	fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / gamma));
}
