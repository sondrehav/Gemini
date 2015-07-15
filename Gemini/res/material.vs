#version 330 core

layout ( location = 0 ) in vec3 in_vertex;
layout ( location = 1 ) in vec3 in_normal;
layout ( location = 2 ) in vec3 in_tangent;
layout ( location = 3 ) in vec2 in_texcoord;

out DATA
{
	vec3 lightDirection;
	float lightSpread;
	float lightStrength;

	vec3 viewDirection;

	vec3 difColor;
	vec3 ambColor;
	vec3 specColor;
	float transp;
	float shine;

	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec2 uv;
} vs;

uniform mat4 pr_matrix = mat4(1.0);
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 md_matrix = mat4(1.0);
uniform vec3 lightPosition;
uniform float lightSpread;
uniform float lightStrength;
uniform vec3 viewDirection;
uniform vec3 diffuseColor;
uniform vec3 ambientColor;
uniform vec3 specularColor;
uniform float shine;
uniform float transparency;

void main()
{
	vs.lightDirection = lightPosition - (md_matrix * vec4(in_vertex, 1.0)).xyz;
	vs.lightSpread = lightSpread;
	vs.viewDirection = viewDirection;
	vs.lightStrength = lightStrength;
	vs.difColor = diffuseColor;
	vs.ambColor = ambientColor;
	vs.specColor = specularColor;
	vs.transp = transparency;
	vs.shine = shine;
	vs.normal = in_normal;
	vs.tangent = in_tangent;
	vs.binormal = cross(in_normal, in_tangent);
	vs.uv = in_texcoord;
	gl_Position = pr_matrix * vw_matrix * md_matrix * vec4(in_vertex, 1.0);
}
