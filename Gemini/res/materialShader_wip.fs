#version 330 core

layout ( location = 0 ) out vec4 color;

uniform sampler2D diffuseMap;
uniform sampler2D ambientMap;
uniform sampler2D specularMap;
uniform sampler2D specularHighlightMap;
uniform sampler2D normalMap;
uniform sampler2D alphaMap;

in vec3 pass_light_direction;

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
	float shine = 100.0;

	vec4 c_dif = texture2D(diffuseMap, pass_texcoord);
	vec4 c_amb = texture2D(ambientMap, pass_texcoord);
	vec4 c_spec = texture2D(specularMap, pass_texcoord);
	vec4 c_sHigh = texture2D(specularHighlightMap, pass_texcoord);
	vec4 c_norm = texture2D(normalMap, pass_texcoord);
	vec4 c_alpha = texture2D(alphaMap, pass_texcoord);

	vec3 n_map = normalize(c_norm.rbg * 2.0 - 1.0);

	vec3 n_tangent = cross(pass_normal, normalize(vec3(0.1, 0.2, 0.3)));
	vec3 bi_tangent = cross(pass_normal, n_tangent);

	mat3x3 normal_mat = mat3x3(normalize(pass_normal), normalize(bi_tangent), normalize(n_tangent));
	//normal_mat = transpose(normal_mat);

	vec3 norm = normal_mat * n_map + pass_normal;

	vec3 reflection = normalize(reflect(-pass_light_direction, norm));
	float spec = pow(max(dot(reflection, -pass_viewDir), 0.0), shine);

	float diffuse = clamp(dot(norm, -normalize(pass_light_direction)), 0.0, 1.0);

	if(c_alpha.r < 0.5)
	{
		discard;
	}

	float d = pass_light_spread / (pow(length(pass_light_direction), 2.0) + pass_light_spread);
// + spec * c_spec.xyz + vec3(0.025,0.025,0.05)
	color = vec4(norm * 0.5 + 0.5, 1.0);
	
}