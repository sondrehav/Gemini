#version 330 core

layout ( location = 0 ) in vec3 in_vertex;
layout ( location = 1 ) in vec3 in_normal;
layout ( location = 2 ) in vec3 in_color;
layout ( location = 3 ) in vec2 in_texcoord;

out vec2 pass_texcoord;
out vec3 pass_normal;
out vec3 pass_light;
out vec3 pass_viewDir;

uniform mat4 pr_matrix = mat4(1.0);
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 md_matrix = mat4(1.0);

uniform vec3 lightDir = vec3(-0.5, -0.5, -0.5);
uniform vec3 view_direction = vec3(1.0,1.0,1.0);

void main()
{
	gl_Position = pr_matrix * vw_matrix * md_matrix * vec4(in_vertex, 1.0);
	pass_texcoord = in_texcoord;
	pass_normal = in_normal;
	pass_light = lightDir;
}
