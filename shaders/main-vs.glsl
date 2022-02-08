#version 330
// BUFFORS
layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec2 inUV;
layout( location = 2 ) in vec3 inNormal;
layout( location = 3 ) in mat4 instanceMatrix ;

// UNIFORMS
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform mat4 lightProj;
uniform mat4 lightView;
uniform int numberOfInstances;

// OUT
out vec2 TexCoords;
out vec4 FragPos;
out vec3 fragNormal;
out vec4 ourPosLight;


void main()
{
	mat4 matModel = numberOfInstances > 1 ? instanceMatrix  : model;
	FragPos = inPosition;
	TexCoords = inUV;
	fragNormal = mat3(transpose(inverse(matModel))) * inNormal;

	ourPosLight = lightProj * lightView * matModel * inPosition;

	gl_Position = projection * view * matModel * inPosition;
}

