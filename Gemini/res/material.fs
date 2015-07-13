#version 330 core

layout ( location = 0 ) out vec4 color;

uniform sampler2D diffuseMap;
uniform sampler2D ambientMap;
uniform sampler2D specularMap;
uniform sampler2D specularHighlightMap;
uniform sampler2D normalMap;
uniform sampler2D alphaMap;

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

	vec3 reflection = reflect(-pass_light, pass_normal);
	float spec = pow(max(dot(pass_viewDir, reflection), 0.0), 10.0);

	if(c_alpha.w < 0.9)
	{
		discard;
	}

	float diffuseAmount = clamp(dot(pass_light, pass_normal), 0.0, 1.0) * 0.8 + 0.2;

	//color = vec4(c_dif.rgb * diffuseAmount + vec3(spec, spec, spec), c_alpha.r);
	color = vec4(c_dif.rgb * diffuseAmount, c_alpha.r);
}