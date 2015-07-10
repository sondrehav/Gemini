#version 330 core

layout ( location = 0 ) out vec4 color;

in vec3 pass_color;

void main()
{
	color = vec4(pass_color,1.0);
}