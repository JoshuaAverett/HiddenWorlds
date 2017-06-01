#version 430

in vec2 uv;

out vec4 color;

uniform sampler2D texture;

void main()
{
	color.rgb = texture2D(texture, uv).rgb;
	color.a = 1;
}