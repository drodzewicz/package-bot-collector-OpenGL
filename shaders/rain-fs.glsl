#version 150

// OUT
out vec4 outColor;

// IN
in vec4 FragPos;
in vec2 TexCoords;

// UNIFORMS
uniform sampler2D textureId;
uniform vec3 color;
uniform int isTexture;



void main()
{
	vec4 tempColor = isTexture == 1 ? texture(textureId, TexCoords) : vec4(color, 1.0f);
	if(tempColor.a < 0.1) {
		discard;
	}
	tempColor.a = 0.02;
	outColor = tempColor;
}
