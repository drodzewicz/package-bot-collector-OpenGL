#ifndef __SHADOW_H
#define __SHADOW_H

#include "libs.hpp"
#include "classes/CProgram.hpp"
#include "classes/CMesh.hpp"

CProgram programDepthMap;
GLuint DepthMap_FrameBuffer;
GLuint DepthMap_Texture;
const unsigned int DepthMap_Width = 2024, DepthMap_Height = 2024;

// macierze rzutowania dla kamery patrzacej z punktu widzenia oswietlenia
glm::mat4 lightProj = glm::ortho(61.0f, -70.0f, -50.0f, 50.0f, 2.0f, 200.0f);

glm::mat4 lightView = glm::lookAt(Light_Position,
                                  Light_Position+Light_Direction,
                                  glm::vec3( 0.0f, 1.0f,  0.0f));


// -------------------------------------------------------
void ShadowMapInit()
{
	// 1. Stworzenie obiektu tekstury
	glGenTextures(1, &DepthMap_Texture);
	glBindTexture(GL_TEXTURE_2D, DepthMap_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DepthMap_Width, DepthMap_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// 2. Stworzenie obiektu ramki bufora jedynie z buforem glebokosci
	// Skladowa koloru nie jest nam potrzebna
	glGenFramebuffers(1, &DepthMap_FrameBuffer);

	// 3. Dolaczanie tekstury do ramki bufora
	glBindFramebuffer(GL_FRAMEBUFFER, DepthMap_FrameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthMap_Texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 4. Stworzenie oddzielnego programu, ktory bedzie generowal mape cieni
	programDepthMap = CProgram("shaders/depthmap-vs.glsl", "shaders/depthmap-fs.glsl");

}

void DrawShadowMap()
{
    glViewport(0, 0, DepthMap_Width, DepthMap_Height);
    glBindFramebuffer(GL_FRAMEBUFFER, DepthMap_FrameBuffer);
    glClear(GL_DEPTH_BUFFER_BIT);
    // AKTYWUJEMY program
    programDepthMap.UseProgram();
    programDepthMap.SendUniformMatrix4fv("lightProj", lightProj);
    programDepthMap.SendUniformMatrix4fv("lightView", lightView);

     for (CMesh *obj : objectList)
    {
        obj->Draw(&programDepthMap);
    }
	robot.Draw(&programDepthMap);
    skyscraper.Draw(&programDepthMap);


    // WYLACZAMY program
    glUseProgram(0);
}

void SendLightPVD (CProgram * program) {
	program->SendUniformMatrix4fv("lightView", lightView);
    program->SendUniformMatrix4fv("lightProj", lightProj);
    program->SendUniform3fv("Light_Direction", Light_Direction);
}

#endif // __SHADOW_H
