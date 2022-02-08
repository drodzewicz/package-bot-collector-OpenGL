#ifndef CPROGRAM_H
#define CPROGRAM_H

#include <GL/glew.h>

class CProgram
{
public:
    GLuint idProgram;

    CProgram() {}

    CProgram(const char *vertexPath, const char *fragmentPath)
    {
        this->CreateProgram();
        this->attachShader(GL_VERTEX_SHADER, vertexPath);
        this->attachShader(GL_FRAGMENT_SHADER, fragmentPath);
        

        this->LinkAndValidateProgram();
    }

    void CreateProgram()
    {
        this->idProgram = glCreateProgram();
    }

    void UseProgram()
    {
        glUseProgram(this->idProgram);
    }

    void DeleteProgram()
    {
        glDeleteProgram(this->idProgram);
    }

    CProgram &attachShader(GLuint MODE, const char *path)
    {
        // utworzenie obiektu shadera
        GLuint shader = glCreateShader(MODE);

        // Wczytanie kodu shadera z pliku
        GLchar *code = LoadShaderFile(path);

        glShaderSource(shader, 1, &code, NULL);
        glCompileShader(shader);
        CheckForErrors_Shader(shader);

        delete[] code;

        glAttachShader(this->idProgram, shader);
        return *this;
    }

    void LinkAndValidateProgram()
    {
        // Konsolidacja programu
        glLinkProgram(this->idProgram);
        CheckForErrors_Program(this->idProgram, GL_LINK_STATUS);

        // Walidacja programu
        glValidateProgram(this->idProgram);
        CheckForErrors_Program(this->idProgram, GL_VALIDATE_STATUS);
    }

    // UNIFORMS
    void SendUniform1i(const char *name, GLint64 data)
    {
        glUniform1i(glGetUniformLocation(this->idProgram, name), data);
    }
    void SendUniform1f(const char *name, GLfloat data)
    {
        glUniform1f(glGetUniformLocation(this->idProgram, name), data);
    }
    void SendUniformMatrix4fv(const char *name, glm::mat4 matData)
    {
        glUniformMatrix4fv(
            glGetUniformLocation(this->idProgram, name),
            1, GL_FALSE, glm::value_ptr(matData));
    }
    void SendUniform3fv(const char *name, glm::vec3 matData)
    {
        glUniform3fv(glGetUniformLocation(this->idProgram, name), 1, &matData[0]);
    }
};

#endif
