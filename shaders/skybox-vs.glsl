#version 330 core

layout( location = 0 ) in vec4 inPosition;

// OUT
out vec4 fragPos;

// UNIFORM
uniform mat4 matProjView;

void main()
{
	gl_Position = matProjView * inPosition;
	fragPos = inPosition;
}
