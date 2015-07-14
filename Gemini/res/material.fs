#version 330 core

layout ( location = 0 ) out vec4 color;

uniform sampler2D diffuseMap;
uniform sampler2D ambientMap;
uniform sampler2D specularMap;
uniform sampler2D specularHighlightMap;
uniform sampler2D normalMap;
uniform sampler2D alphaMap;

in vec3 dif_test;

in vec3 pass_diffuseColor;
in vec3 pass_ambientColor;
in vec3 pass_specularColor;

in float transparency;

in vec2 pass_texcoord;
in vec3 pass_normal;
in vec3 pass_light;
in vec3 pass_viewDir;

in float pass_light_spread;

void main()
{

	vec4 c_dif = texture2D(diffuseMap, pass_texcoord);
	vec4 c_amb = texture2D(ambientMap, pass_texcoord);
	vec4 c_spec = texture2D(specularMap, pass_texcoord);
	vec4 c_sHigh = texture2D(specularHighlightMap, pass_texcoord);
	vec4 c_norm = texture2D(normalMap, pass_texcoord);
	vec4 c_alpha = texture2D(alphaMap, pass_texcoord);

	vec3 n_map = vec3(c_norm.rg * 2.0 - 1.0, c_norm.b - 1.0) * -1.0;
	vec3 norm = pass_normal + n_map;

	vec3 reflection = reflect(-normalize(dif_test), norm);
	float spec = clamp(pow(max(dot(reflection, pass_viewDir), 0.0), 3.0),0.0,1.0) * 0.3;
	float diffuse = clamp(dot(norm, -normalize(dif_test)), 0.0, 1.0) * 0.8 + 0.2;

	if(c_alpha.r < 0.5)
	{
		discard;
	}

	float d = pass_light_spread / (pow(length(dif_test), 2.0) + pass_light_spread);

	color = vec4(diffuse * c_dif.xyz * d + spec * c_spec.xyz + vec3(0.025,0.025,0.05), 1.0);
	
}