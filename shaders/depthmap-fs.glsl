#version 330 core

in vec2 inoutUV;

uniform sampler2D textureId;
uniform vec3 color;
uniform int isTexture;

void main()
{
	vec4 tempColor = isTexture == 1 ? texture(textureId, inoutUV) : vec4(color, 1.0f);

    if(tempColor.a < 0.1){
        discard;
    }
    else{
        gl_FragDepth = gl_FragCoord.z;
    }


}  