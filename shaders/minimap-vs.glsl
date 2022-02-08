// Vertex shader
#version 330 core

// BUFFORS
layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec2 inUV;

out vec2 TexCoords;


void main()
{
	TexCoords = inUV;
	gl_Position = inPosition;

}