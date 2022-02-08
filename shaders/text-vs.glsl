// https://learnopengl.com/In-Practice/Text-Rendering
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

// OUT
out vec2 TexCoords;

// UNIFORM
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform int is3D;
uniform int isBilboarding;

mat4 setModelForBillboarding (mat4 modelView) {
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
            modelView[i][j] = i == j ? 1.f : 0.f;
		}
	}
    return modelView;
}

void main()
{
    if(is3D == 1) {
        mat4 modelView = view * model;
        if (isBilboarding == 1) modelView = setModelForBillboarding(modelView);
       
        gl_Position = projection * modelView * vec4(vertex.xy, 0.0, 1.0);
    } else {
        gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    }
    TexCoords = vertex.zw;
}