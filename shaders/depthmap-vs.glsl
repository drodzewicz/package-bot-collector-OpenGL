#version 330 core
layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec2 inUV;
layout( location = 3 ) in mat4 localMatrix;

uniform mat4 lightProj;
uniform mat4 lightView;
uniform mat4 model;
uniform int numberOfInstances;

out vec2 inoutUV;

void main()
{
	mat4 matModel = numberOfInstances > 1 ? localMatrix : model;
    gl_Position = lightProj * lightView * matModel * inPosition;
    inoutUV = inUV;
}
