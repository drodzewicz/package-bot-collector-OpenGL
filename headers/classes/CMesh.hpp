#ifndef CMESH_H
#define CMESH_H

#include "CProgram.hpp"

class CMesh
{
private:
    GLuint VAO;
    GLuint vBuffer_pos;
    GLuint vBuffer_norm;
    GLuint vBuffer_uv;

    GLuint vBuffer_ins;

    std::vector<glm::vec3> OBJ_vertices;
    std::vector<glm::vec2> OBJ_uvs;
    std::vector<glm::vec3> OBJ_normals;

    GLuint idTexture;
    int isTexture;

    glm::vec3 objColor;

    MaterialParam objectMaterial = {
        glm::vec3(1, 1, 1),
        glm::vec3(1, 1, 1),
        glm::vec3(1, 1, 1),
        64};

    int numberOfInstances = 1;

    void setupMesh()
    {
        // VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Wspolrzedne wierzchokow
        glGenBuffers(1, &vBuffer_pos);
        glBindBuffer(GL_ARRAY_BUFFER, vBuffer_pos);
        glBufferData(GL_ARRAY_BUFFER, OBJ_vertices.size() * sizeof(glm::vec3), &OBJ_vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        // Wspolrzedne textury UV
        glGenBuffers(1, &vBuffer_uv);
        glBindBuffer(GL_ARRAY_BUFFER, vBuffer_uv);
        glBufferData(GL_ARRAY_BUFFER, OBJ_uvs.size() * sizeof(glm::vec2), &OBJ_uvs[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        // Normalki
        glGenBuffers(1, &vBuffer_norm);
        glBindBuffer(GL_ARRAY_BUFFER, vBuffer_norm);
        glBufferData(GL_ARRAY_BUFFER, OBJ_normals.size() * sizeof(glm::vec3), &OBJ_normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
    }



public:
    glm::mat4 matModel;
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 rotation = glm::vec3(0, 0, 0);
    float scale = 1.0f;


    CMesh() {}

    CMesh(std::string objfile)
    {
        this->matModel = glm::mat4(1.0);

        const char *objectPath = objfile.c_str();
        if (!loadOBJ(objectPath, OBJ_vertices, OBJ_uvs, OBJ_normals))
        {
            printf("ERROR::PROBLEM_LOADING_OBJECT[%s]\n", objectPath);
            exit(1);
        }

        setupMesh();

        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);
    }
    CMesh(std::string objfile, int instances, const void *matData)
    {
        this->numberOfInstances = instances;
        this->matModel = glm::mat4(1.0);

        const char *objectPath = objfile.c_str();
        if (!loadOBJ(objectPath, OBJ_vertices, OBJ_uvs, OBJ_normals))
        {
            printf("ERROR::PROBLEM_LOADING_OBJECT[%s]\n", objectPath);
            exit(1);
        }

        this->setupMesh();
        this->setupModelMesh(matData);

        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);
    }

    void addTexture(std::string texfile)
    {
        const char *texturePath = texfile.c_str();
        CreateTexture(texturePath);
        isTexture = 1;
    }

    void addColor(glm::vec3 color)
    {
        isTexture = 0;
        this->objColor = color;
    }

    void addMaterial(MaterialParam material)
    {
        this->objectMaterial = material;
    }

    void setTextureID(GLuint textureBuff)
    {
        this->idTexture = textureBuff;
        this->isTexture = 1;
    }

    void Draw(CProgram *program)
    {
        glBindVertexArray(VAO);
        this->updateMatModel();
        __CHECK_FOR_ERRORS
        if (this->isTexture)
        {
            glActiveTexture(GL_TEXTURE0);
            __CHECK_FOR_ERRORS
            glBindTexture(GL_TEXTURE_2D, this->idTexture);
            __CHECK_FOR_ERRORS
            program->SendUniform1i("textureId", 0);
            __CHECK_FOR_ERRORS
        } else {
            program->SendUniform3fv("color", this->objColor);
            __CHECK_FOR_ERRORS
        }
        __CHECK_FOR_ERRORS
        program->SendUniform1i("numberOfInstances", this->numberOfInstances);

        program->SendUniformMatrix4fv("model", this->matModel);
        __CHECK_FOR_ERRORS
        program->SendUniform1i("isTexture", this->isTexture);
        __CHECK_FOR_ERRORS
        // MATERIAL
        program->SendUniform3fv("inMaterial.Ambient", this->objectMaterial.Ambient);
        program->SendUniform3fv("inMaterial.Diffuse", this->objectMaterial.Diffuse);
        program->SendUniform3fv("inMaterial.Specular", this->objectMaterial.Specular);
        program->SendUniform1f("inMaterial.Shininess", this->objectMaterial.Shininess);
        __CHECK_FOR_ERRORS


        // DRAW ARRAYS OR INSTANCES
        if (this->numberOfInstances > 1)
        {
            glDrawArraysInstanced(GL_TRIANGLES, 0, OBJ_vertices.size(), this->numberOfInstances);
            __CHECK_FOR_ERRORS
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, OBJ_vertices.size() * sizeof(glm::vec3));
            __CHECK_FOR_ERRORS
        }

        glBindVertexArray(0);
    }

    void Cleanup()
    {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteVertexArrays(1, &this->vBuffer_pos);
        glDeleteVertexArrays(1, &this->vBuffer_norm);
        glDeleteVertexArrays(1, &this->vBuffer_uv);
    }

    void CreateTexture(const char *path)
    {
        int tex_width, tex_height, n;
        unsigned char *tex_data;
        // stbi_set_flip_vertically_on_load(true); // tylko raz aby ...

        tex_data = stbi_load(path, &tex_width, &tex_height, &n, 0);
        if (tex_data == NULL)
        {
            printf("Image %s can't be loaded!\n", path);
            exit(1);
        }
        glGenTextures(1, &idTexture);
        glBindTexture(GL_TEXTURE_2D, idTexture);
        if (n == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
        else if (n == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
        // Generowanie mipmap automatycznie
        glGenerateMipmap(GL_TEXTURE_2D);

        // Podstawowy level mipmap (default 0)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    }

    // MODEL TRASNFORM
    void updateMatModel () {
        this->matModel = glm::mat4(1.0);
        // position
        this->matModel = glm::translate(this->matModel, this->position);
        // rotation
        this->matModel = glm::rotate(this->matModel, this->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        this->matModel = glm::rotate(this->matModel, this->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        this->matModel = glm::rotate(this->matModel, this->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        // scale
        this->matModel = glm::scale(this->matModel, glm::vec3(this->scale));

    }

    std::vector<glm::vec3> getVertices () {
        return this->OBJ_vertices;
    }

    void setupModelMesh(const void *matData)
    {
        glBindVertexArray(VAO);
        glGenBuffers(1, &vBuffer_ins);
        glBindBuffer(GL_ARRAY_BUFFER, vBuffer_ins);
        glBufferData(GL_ARRAY_BUFFER, this->numberOfInstances * sizeof(glm::mat4x4), matData, GL_STATIC_DRAW);

        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)0);
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)sizeof(glm::vec4));
        glEnableVertexAttribArray(4);

        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);

        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(3 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
    }
};

#endif
