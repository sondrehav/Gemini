#version 330 core

layout ( location = 0 ) out vec4 color;

uniform sampler2D diffuseMap;
uniform sampler2D ambientMap;
uniform sampler2D specularMap;
uniform sampler2D specularHighlightMap;
uniform sampler2D normalMap;
uniform sampler2D alphaMap;

in DATA
{
	vec3 lightDirection;
	float lightSpread;
	float specStrength;

	vec3 viewDirection;
	vec3 viewPosition;

	vec3 difColor;
	vec3 ambColor;
	vec3 specColor;
	float transp;
	float shine;

	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec2 uv;
} fs;

void main()
{

	vec4 c_dif = texture2D(diffuseMap, fs.uv);
	vec4 c_amb = texture2D(ambientMap, fs.uv);
	vec4 c_spec = texture2D(specularMap, fs.uv);
	vec4 c_sHigh = texture2D(specularHighlightMap, fs.uv);
	vec4 c_norm = texture2D(normalMap, fs.uv) * 2.0 - 1.0;
	c_norm.xy *= -1.0;
	vec4 c_alpha = texture2D(alphaMap, fs.uv);

	if(c_alpha.r < 0.5)
	{
		discard;
	}

	vec3 norm = mat3x3(fs.tangent, fs.binormal, fs.normal) * c_norm.xyz;

	float diffuseStrength = fs.lightSpread / (pow(length(fs.lightDirection), 2.0) + fs.lightSpread);
	float ds = diffuseStrength * max(dot(normalize(fs.lightDirection), norm), 0.0);

	vec3 refl = reflect(fs.lightDirection, norm * vec3(1.0, -1.0, 1.0));
	float spec = pow(max(dot(normalize(-fs.viewDirection), normalize(refl)), 0.0), fs.shine) * fs.specStrength * diffuseStrength;

	color = vec4(c_dif.xyz * ds + c_spec.xyz * spec + vec3(0.025,0.025,0.01), 1.0);
	
}