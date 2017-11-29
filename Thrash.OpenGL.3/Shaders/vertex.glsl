#version 130

precision highp float;

uniform mat4 mvp;

uniform uint fogMode;
uniform vec4 fogColor;
uniform float fogStart;
uniform float fogEnd;
uniform float fogDensity;

in vec4 vCoord;
in vec4 vDiffuse;
in vec4 vSpecular;
in vec2 vTexCoord;

out vec4 fDiffuseSmooth;
flat out vec4 fDiffuseFlat;
out vec4 fSpecular;
out vec3 fTexCoord;
out float fogFactor;

void main()
{
	if (vCoord.z <= 1.0)
		gl_Position = mvp * vec4(vCoord.xyz, 1.0);
	else
		gl_Position = mvp * vec4(vCoord.xy, 0.0, 1.0);

	fDiffuseFlat = fDiffuseSmooth = vDiffuse.bgra;
	fSpecular = vSpecular.bgra;
	fTexCoord = vec3(vTexCoord.x * vCoord.w, vTexCoord.y * vCoord.w, vCoord.w);

	switch (fogMode)
	{
		case 2u:
			fogFactor = (1.0 - clamp((fogEnd - (vCoord.w != 0.0 ? 1.0 / vCoord.w : 1.0)) / (fogEnd - fogStart), 0.0, 1.0)) * fogColor.a;
			break;

		case 4u:
			fogFactor = (1.0 - clamp(exp(-fogDensity * (vCoord.w != 0.0 ? 1.0 / vCoord.w : 1.0)), 0.0, 1.0)) * fogColor.a;
			break;

		case 8u:
			const float LOG2 = -1.442695;
			float d = fogDensity * (vCoord.w != 0.0 ? 1.0 / vCoord.w : 1.0);
			fogFactor = (1.0 - clamp(exp2(d * d * LOG2), 0.0, 1.0)) * fogColor.a;
			break;

		default:
			fogFactor = 0.0;
			break;
	}
}
