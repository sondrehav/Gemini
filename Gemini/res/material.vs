#version 330 core

layout ( location = 0 ) in vec3 in_vertex;
layout ( location = 1 ) in vec3 in_normal;
layout ( location = 2 ) in vec3 in_color;
layout ( location = 3 ) in vec2 in_texcoord;

out vec2 pass_texcoord;
out vec3 pass_normal;
out vec3 pass_light;
out vec3 pass_viewDir;

out vec3 pass_diffuseColor;
out vec3 pass_ambientColor;
out vec3 pass_specularColor;

out float pass_light_spread;

out float pass_transparency;

out vec3 dif_test;

uniform mat4 pr_matrix = mat4(1.0);
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 md_matrix = mat4(1.0);

uniform vec3 light_pos;
uniform vec3 view_direction = vec3(1.0,1.0,1.0);

uniform vec3 diffuseColor;
uniform vec3 ambientColor;
uniform vec3 specularColor;

uniform float transparency;
uniform float light_spread = 100000.0;

void main()
{
	pass_light_spread = light_spread;
	dif_test = in_vertex - light_pos;
	gl_Position = pr_matrix * vw_matrix * md_matrix * vec4(in_vertex, 1.0);
	pass_texcoord = in_texcoord;
	pass_normal = in_normal;
	pass_light = light_pos;
	pass_diffuseColor = diffuseColor;
	pass_ambientColor = ambientColor;
	pass_specularColor = specularColor;
	pass_transparency = transparency;
	pass_viewDir = view_direction;
}
