#version 330 core

layout ( location = 0 ) out vec4 color;

uniform sampler2D diffuseMap;
uniform sampler2D ambientMap;
uniform sampler2D specularMap;
uniform sampler2D specularHighlightMap;
uniform sampler2D normalMap;
uniform sampler2D alphaMap;

in vec3 pass_diffuseColor;
in vec3 pass_ambientColor;
in vec3 pass_specularColor;

in float transparency;

in vec2 pass_texcoord;
in vec3 pass_normal;
in vec3 pass_light;
in vec3 pass_viewDir;

void main()
{

	vec4 c_dif = texture2D(diffuseMap, pass_texcoord);
	vec4 c_amb = texture2D(ambientMap, pass_texcoord);
	vec4 c_spec = texture2D(specularMap, pass_texcoord);
	vec4 c_sHigh = texture2D(specularHighlightMap, pass_texcoord);
	vec4 c_norm = texture2D(normalMap, pass_texcoord);
	vec4 c_alpha = texture2D(alphaMap, pass_texcoord);

	vec3 n_map = vec3(c_norm.rg * 2.0 - 1.0, c_norm.b - 1.0);
	vec3 norm = pass_normal + n_map;

	vec3 reflection = reflect(-pass_light, norm);
	float spec = pow(max(dot(reflection, pass_viewDir), 0.0), 1.0) * 1.0;
	float diffuse = max(dot(norm, pass_light), 0.0);

	if(c_alpha.r < 0.9)
	{
		discard;
	}

	color = vec4(c_dif.rgb * diffuse + spec * c_spec.rgb, 1.0);
}