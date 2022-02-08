#ifndef COFF_REDNER_HPP
#define COFF_REDNER_HPP

#include "../libs.hpp"
#include "CProgram.hpp"
#include "CMesh.hpp"
#include "CCam.hpp"

class COffRedner
{
public:
    GLuint FrameBufferID;
    GLuint DepthBufferID;
    GLuint TextureBuffer;
    
    COffRedner() {}

    void InitOffRendering()
    {
        // 1. Stworzenie tekstury do ktorej odbedzie sie rendering

        glGenTextures(1, &TextureBuffer);
        glBindTexture(GL_TEXTURE_2D, TextureBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GLUT_SCREEN_WIDTH, GLUT_SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // --------------------------------------------------
        // 2. Stworzenie obiektu bufora ramki, ktory zawiera bufor koloru

        glGenFramebuffers(1, &FrameBufferID);
        glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);

        // Polaczenie tekstury do skladowej koloru bufora ramki
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureBuffer, 0);

        // --------------------------------------------------
        // 3. Stworzenie obiektu render-buffer dla testu glebokosci

        glGenRenderbuffers(1, &DepthBufferID);
        glBindRenderbuffer(GL_RENDERBUFFER, DepthBufferID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, GLUT_SCREEN_WIDTH, GLUT_SCREEN_HEIGHT);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Polaczenie bufora glebokosci z aktualnym obiektem bufora ramki
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthBufferID);

        // 4. Sprawdzenie czy pomyslnie zostal utworzony obiekt bufora ramki
        // i powrot do domyslnego bufora ramki

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Error: Framebuffer is not complete!\n");
            exit(1);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void DrawToTexture(CProgram *program, CCamera * cam, std::vector<CMesh *> objectList)
    {
        glViewport(0, 0, GLUT_SCREEN_WIDTH, GLUT_SCREEN_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program->UseProgram();
        cam->SendPVE(program);
        // DRAW OBJECTS -------------------------
        for (CMesh *obj : objectList)
        {
            obj->Draw(program);
        }
    }
};

#endif