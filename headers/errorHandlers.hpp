#ifndef __ERROR_HANDLERS
#define __ERROR_HANDLERS

#include "libs.hpp"

void CheckForErrors_Program(GLuint program, GLenum mode)
{
    GLint status;
    glGetProgramiv(program, mode, &status);
    if (status != GL_TRUE)
    {
        switch (mode)
        {
        case GL_LINK_STATUS:
            printf("ERROR::[CONSOLIDATION_PROBLEM]\n");
            break;
        case GL_VALIDATE_STATUS:
            printf("ERROR::[PROGRAM_VALIDATION_ERROR]\n");
            break;
        default:
            printf("ERROR::[OTHER_PROBLEM]!\n");
        }
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        char *log = new char[logLength];
        glGetProgramInfoLog(program, logLength, NULL, log);
        printf("LOG: %s\n", log);
        delete[] log;
        exit(1);
    }
}

void CheckForErrors_Shader(GLuint shader)
{
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        printf("Blad!\n");
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        char *log = new char[logLength];
        glGetShaderInfoLog(shader, logLength, NULL, log);
        printf("LOG: %s\n", log);
        delete[] log;
        exit(1);
    }
}

void SimpleErrorHandler(bool isError, std::string errorMessage)
{
	if (isError)
	{
		printf("%s \n", errorMessage);
		exit(1);
	}
}

#endif