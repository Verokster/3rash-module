#version 130

precision highp float;

uniform mat4 mvp;

uniform uint fogMode;
uniform vec4 fogColor;
uniform float fogStart;
uniform float fogEnd;
uniform float fogDensity;

in vec4 vCoord;
in vec4 vColor;
in vec2 vTexCoord;
in uint vTexUnit;

out vec4 fColorSmooth;
flat out vec4 fColorFlat;
out vec2 fTexCoord;
flat out uint fTexUnit;
out float fogFactor;

void main()
{
	float w = vCoord.w != 0.0 ? 1.0 / vCoord.w : 1.0;

	if (vCoord.z <= 1.0)
		gl_Position = mvp * vec4(vCoord.xyz * w, w);
	else
		gl_Position = mvp * vec4(vCoord.xy * w, (1.0 - 1.0 / vCoord.z) * w, w);

	fColorFlat = fColorSmooth = vColor.bgra;
	fTexCoord = vTexCoord;
	fTexUnit = vTexUnit;

	switch (fogMode)
	{
		case 2u:
			fogFactor = (1.0 - clamp((fogEnd - w) / (fogEnd - fogStart), 0.0, 1.0)) * fogColor.a;
			break;

		case 4u:
			fogFactor = (1.0 - clamp(exp(-fogDensity * w), 0.0, 1.0)) * fogColor.a;
			break;

		case 8u:
			const float LOG2 = -1.442695;
			float d = fogDensity * w;
			fogFactor = (1.0 - clamp(exp2(d * d * LOG2), 0.0, 1.0)) * fogColor.a;
			break;

		default:
			fogFactor = 0.0;
			break;
	}
}
