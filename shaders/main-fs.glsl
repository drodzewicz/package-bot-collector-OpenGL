#version 150 core

// OUT
out vec4 outColor;

struct LightParam
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 Att;
	vec3 Position;
	vec3 Direction;
};

struct Material
{
	vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
};

// IN
in vec4 FragPos;
in vec2 TexCoords;
in vec3 fragNormal;
in vec4 ourPosLight;

// UNIFORMS
uniform sampler2D textureId;
uniform sampler2D shadowMap;
uniform vec3 color;
uniform int isTexture;
uniform Material inMaterial;
uniform LightParam sunLight;
uniform vec3 eyePosition;
uniform vec3 Light_Direction;

vec3 CalculatePointLight(LightParam light, vec3 vectorNormal, vec3 fragment, Material material) {
	vec3 ambient  = light.Ambient * material.Ambient;
	float DLV = length( light.Position - fragment );
	float Latt = 1.0/( light.Att.x + light.Att.y * DLV + light.Att.z * DLV * DLV );

	vec3 L = normalize(light.Position - fragment);
	vec3 diffuse = max( dot(L, vectorNormal ), 0 ) * light.Diffuse * material.Diffuse;

	vec3 E = normalize( eyePosition - fragment );
	vec3 R = reflect( -E, vectorNormal );
	vec3 specular = pow(max(dot(R, L), 0), material.Shininess) * light.Specular * material.Specular;

	return ambient + Latt * (diffuse + specular);
}

float CalculateShadow(vec4 fragPosLight, vec3 Light_Direction)
{

	// perform perspective divide
	vec3 projCoords = fragPosLight.xyz / fragPosLight.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;

	if(projCoords.z > 1.0)
	return 0.0;

	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;

	// calculate bias according to the angle between normal vector and light vector
	float bias = max(0.001 * (1.0 - dot(fragNormal, Light_Direction)), 0.001);

	//bias = 0.0f;

	// Smooth filtering
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(float x = -1; x <= 1; x+=1.0)
	{
		for(float y = -1; y <= 1; y+=1.0)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 12.0;

	return shadow;
}

vec3 CalculateDirectionalLight(LightParam light, Material material, vec3 cameraPosition)
{
	// diffuse
	vec3  lightDirection = normalize(Light_Direction);
	float diffuseCoeff = max(dot(fragNormal, -Light_Direction), 0.0);
	vec3  resultDiffuse = diffuseCoeff * vec3(light.Diffuse);
	vec3  diffusePart = resultDiffuse * material.Diffuse;

	// specular
	vec3  viewDir = normalize(cameraPosition - vec3(FragPos));
	vec3  reflectDir = reflect(Light_Direction, fragNormal);
	float specularCoeff = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
	vec3  specularPart = specularCoeff * vec3(light.Specular) * material.Specular;

	// result
	return (diffusePart + specularPart);
}



void main()
{
	// COLOR OR TEXTURE
	vec4 tempColor = isTexture == 1 ? texture(textureId, TexCoords) : vec4(color, 1.0f);

	float shadowPart = 0;
	vec3  lightPart = vec3(0.0, 0.0, 0.0);
	vec3  finalColor = vec3(0.0, 0.0, 0.0);

	if(tempColor.a < 0.05)
		discard;

	lightPart = CalculateDirectionalLight(sunLight, inMaterial, eyePosition);
	shadowPart = CalculateShadow(ourPosLight,Light_Direction);
	finalColor = (inMaterial.Ambient * sunLight.Ambient + lightPart * (1 - shadowPart));
	outColor = vec4(finalColor , 1.0) * tempColor;

}
