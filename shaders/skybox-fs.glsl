#version 150 core

// OUT
out vec4 outColor;

// IN
in vec4 fragPos;

// UNIFORM
uniform samplerCube tex_skybox;

void main()
{
	outColor = texture(tex_skybox, fragPos.xyz);
}
