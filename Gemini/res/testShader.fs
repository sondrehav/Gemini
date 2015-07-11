#version 330 core

layout ( location = 0 ) out vec4 color;

uniform sampler2D tex;

in vec3 pass_color;
in vec2 pass_texcoord;

void main()
{
	color = vec4(texture2D(tex, pass_texcoord));
}