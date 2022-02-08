#version 330

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec2 inUV;
layout( location = 3 ) in mat4 instanceMatrix ;

// UNIFORMS
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform int numberOfInstances;

// OUT
out vec2 TexCoords;
out vec4 FragPos;

void main()
{
	mat4 matModel = numberOfInstances > 1 ? instanceMatrix  : model;

	gl_Position = projection * view * matModel * inPosition;
	FragPos = inPosition;
	TexCoords = inUV;
}

