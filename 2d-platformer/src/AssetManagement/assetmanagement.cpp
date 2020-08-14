#include "assetmanagement.hpp"
#include <glad/glad.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

Shader LoadShadersFromFiles(const char* vShaderPath, const char* fShaderPath)
{
    // Load files into memory
    FILE* vsRaw;
    if(fopen_s(&vsRaw, vShaderPath, "rb") != 0)
    {
        printf("Failed to open file at path: %s\n", vShaderPath);
        exit(-1);
    }

    fseek(vsRaw, 0, SEEK_END);
    long size = ftell(vsRaw);
    rewind(vsRaw);

    char* vsBuffer = new char[size + 1];
    size_t readSize = fread(vsBuffer, 1, size, vsRaw);
    vsBuffer[size] = '\0';
    rewind(vsRaw);

    // Create vertex shader object
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vsBuffer, 0);
    glCompileShader(vertex);

    // Check compilation errors
    GLint compiled;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &compiled);
    if(compiled != GL_TRUE)
    {
        GLsizei length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(vertex, 1024, &length, message);
        printf("Vertex Shader Compilation Errors:\n%s\n", message);
    }

    FILE* fsRaw;
    if(fopen_s(&fsRaw, fShaderPath, "rb") != 0)
    {
        printf("Failed to open file at path: %s\n", fShaderPath);
        exit(-1);
    }

    fseek(fsRaw, 0, SEEK_END);
    size = ftell(fsRaw);
    rewind(fsRaw);

    char* fsBuffer = new char[size + 1];
    readSize = fread(fsBuffer, 1, size, fsRaw);
    fsBuffer[size] = '\0';
    rewind(fsRaw);

    // Create fragment shader object
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fsBuffer, 0);
    glCompileShader(fragment);

    // Check compilation errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &compiled);
    if(compiled != GL_TRUE)
    {
        GLsizei length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(fragment, 1024, &length, message);
        printf("Fragment Shader Compilation Errors:\n%s\n", message);
    }

    GLuint ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // Get uniform names and locations from program 
    std::unordered_map<std::string, int> uniforms;

    int uniformCount = 0;
    glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &uniformCount);

    if(uniformCount)
    {
        int maxNameLength = 0;
        GLsizei length = 0, count = 0;
        GLenum type = GL_NONE;
        glGetProgramiv(ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

        char* buffer = new char[maxNameLength];
        for(int i = 0; i < uniformCount; i++)
        {
            glGetActiveUniform(ID, i, maxNameLength, &length, &count, &type, buffer);

            uniforms[std::string(buffer, length)] = glGetUniformLocation(ID, buffer);
        }

        delete[] buffer;
    }

    // Clean up
    glDetachShader(ID, vertex);
    glDetachShader(ID, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    fclose(vsRaw);
    fclose(fsRaw);
    delete[] vsBuffer;
    delete[] fsBuffer;

    return { ID, uniforms };
}
