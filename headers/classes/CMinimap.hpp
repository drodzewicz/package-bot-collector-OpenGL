#ifndef CMINIMAP_H
#define CMINIMAP_H

#include "CProgram.hpp"
#include "CCam.hpp"


enum MinimapPlacement
{
    MINIMAP_CENTER,
    MINIMAP_LEFT,
    MINIMAP_RIGHT,
    MINIMAP_TOP,
    MINIMAP_BOTTOM,
    MINIMAP_TOP_RIGHT,
    MINIMAP_TOP_LEFT,
    MINIMAP_BOTTOM_RIGHT,
    MINIMAP_BOTTOM_LEFT
};

class CMinimap
{
private:
    GLuint idVAO;
    GLuint idVBO_coord;
    GLuint vBuffer_uv;

    GLfloat vertices_tex[2 * 6];
    GLfloat minmapCoords[2 * 6] = {
        -1,
        -1,
        1,
        -1,
        -1,
        1,

        1,
        -1,
        -1,
        1,
        1,
        1,
    };

    void initBuffers()
    {
        // 2. Vertex arrays
        glGenVertexArrays(1, &this->idVAO);
        glBindVertexArray(this->idVAO);

        glGenBuffers(1, &this->idVBO_coord);
        glBindBuffer(GL_ARRAY_BUFFER, this->idVBO_coord);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->minmapCoords), this->minmapCoords, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &this->vBuffer_uv);
        glBindBuffer(GL_ARRAY_BUFFER, this->vBuffer_uv);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices_tex), this->vertices_tex, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    void positionMinimap(MinimapPlacement placement)
    {
        GLfloat x = 0;
        GLfloat y = 0;
        GLfloat distance = 1.0f - this->size / 2 - this->margin;

        switch (placement)
        {
        case MINIMAP_LEFT:
            x = -distance;
            break;
        case MINIMAP_RIGHT:
            x = distance;
            break;
        case MINIMAP_TOP:
            y = distance;
            break;
        case MINIMAP_BOTTOM:
            y = -distance;
            break;
        case MINIMAP_TOP_LEFT:
            x = -distance;
            y = distance;
            break;
        case MINIMAP_TOP_RIGHT:
            x = distance;
            y = distance;
            break;
        case MINIMAP_BOTTOM_LEFT:
            x = -distance;
            y = -distance;
            break;
        case MINIMAP_BOTTOM_RIGHT:
            x = distance;
            y = -distance;
            break;

        default:
            break;
        }

        for (int i = 0; i < 2 * 6; i++)
        {
            this->minmapCoords[i] = i % 2 == 0 ? this->minmapCoords[i] + x : this->minmapCoords[i] + y;
        }
    }

public:

    GLboolean isVisible;
    GLfloat size;
    GLfloat margin;

    CMinimap() {}

    CMinimap(MinimapPlacement placement, float size = 0.4f, float margin = 0.05f)
    {
        this->size = size;
        this->margin = margin;
        this->isVisible = GL_FALSE;

        // Generate UV array
        for (int i = 0; i < 2 * 6; i++)
        {
            this->vertices_tex[i] = this->minmapCoords[i] > 0 ? 1 : 0;
        }

        // Generate Coord array
        for (int i = 0; i < 2 * 6; i++)
        {
            this->minmapCoords[i] = this->minmapCoords[i] * (this->size / 2);
        }

        this->positionMinimap(placement);

        // Bufor na wspolrzedne wierzcholkow
        this->initBuffers();
    }

    void toggleMinimapVisibility()
    {
        this->isVisible = this->isVisible == GL_TRUE ? GL_FALSE : GL_TRUE;
    }


    void Draw(CProgram * program, GLuint textureId)
    {
        if (this->isVisible == GL_TRUE)
        {
            glBindVertexArray(this->idVAO);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureId);
            program->SendUniform1i("textureId", 0);

            glDrawArrays(GL_TRIANGLES, 0, 2 * 6);

            glBindVertexArray(0);
        }
    }
};

#endif
