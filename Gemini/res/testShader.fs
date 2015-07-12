#version 330 core

layout ( location = 0 ) out vec4 color;

uniform sampler2D tex;

in vec2 pass_texcoord;
in float pass_diffuse;

void main()
{
	vec4 c_Col = texture2D(tex, pass_texcoord);
	if(c_Col.w < 1.0)
	{
		discard;
	}
	color = c_Col * pass_diffuse;
}