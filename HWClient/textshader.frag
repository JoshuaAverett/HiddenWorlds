#version 430

uniform sampler2D font_map;
uniform float Smoothness;

const float gamma = 2.2;

in vec2 uv;
in vec4 color;

out vec4 out_color;

void main()
{
	// retrieve signed distance
	float sdf = texture2D(font_map, uv).r;
	
	// perform adaptive anti-aliasing of the edges
	//float w = clamp(smoothness * (abs(dFdx(uv.x)) + abs(dFdy(uv.y))), 0.0, 0.5);
	//float a = smoothstep(0.5 - w, 0.5 + w, sdf);
	float a = sdf;
	
	// gamma correction for linear attenuation
	// a = pow(a, 1.0 / gamma);
	
	// final color
	out_color = vec4(color.rgb, color.a * a);
}
