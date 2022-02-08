#ifndef CLIGHT_H
#define CLIGHT_H

#include "../libs.hpp"
#include "../utils.hpp"
#include "CProgram.hpp"
#include "CMesh.hpp"

class CLight
{
public:
    CProgram programDepthMap;
    GLuint DepthMap_FrameBuffer;
    GLuint DepthMap_Texture;

    const unsigned int DepthMap_Width = 2024, DepthMap_Height = 2024;

    glm::vec3 Light_Direction;
    glm::vec3 Light_Position;

    glm::mat4 lightProj;
    glm::mat4 lightView;

    std::vector<CMesh *> renderObjectList;

    LightParam sunLight = {
        glm::vec3(0.3, 0.3, 0.3),
        glm::vec3(1.2, 1.2, 1.2),
        glm::vec3(0.5, 0.5, 0.5)};

    CLight() {}

    void initLight()
    {
        this->Light_Direction = glm::normalize(glm::vec3(0.81, -0.34, 0.47));
        this->Light_Position = glm::vec3(-67.84, 56.78, -26.93);

        glm::mat4 lightProj = glm::ortho(61.0f, -70.0f, -50.0f, 50.0f, 2.0f, 200.0f);

        glm::mat4 lightView = glm::lookAt(this->Light_Position,
                                          this->Light_Position + this->Light_Direction,
                                          glm::vec3(0.0f, 1.0f, 0.0f));
    }

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
        float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
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

        for (CMesh *obj : renderObjectList)
        {
            obj->Draw(&programDepthMap);
        }

        // WYLACZAMY program
        glUseProgram(0);
    }
    void SendLightPVD(CProgram *program)
    {
        program->SendUniformMatrix4fv("lightView", this->lightView);
        program->SendUniformMatrix4fv("lightProj", this->lightProj);
        program->SendUniform3fv("Light_Direction", this->Light_Direction);
    }
    void SendLight(CProgram *program)
    {
        program->SendUniform3fv("sunLight.Ambient", this->sunLight.Ambient);
        program->SendUniform3fv("sunLight.Diffuse", this->sunLight.Diffuse);
        program->SendUniform3fv("sunLight.Specular", this->sunLight.Specular);
    }
};

#endif