// Fragment shader
#version 330 core

// Predefiniowane dane wejsciowe
//in  vec4 gl_FragCoord;
//in  int  gl_PrimitiveID;

// Dane wyjsciowe
out vec4 outColor;

uniform sampler2D textureId;


in vec2 TexCoords;

void main()
{

	outColor = texture(textureId, TexCoords);

}