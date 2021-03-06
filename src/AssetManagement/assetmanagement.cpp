#include "assetmanagement.h"
#include "../Sprite/sprite.h"
#include <glad/glad.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Shader LoadShadersFromFiles(const char* vShaderPath, const char* fShaderPath)
{
    // Load files into memory
    FILE* vsRaw = fopen(vShaderPath, "rb");
    if(!vsRaw)
    {
        printf("Failed to open file at path: %s\n", vShaderPath);
        exit(-1);
    }

    fseek(vsRaw, 0, SEEK_END);
    size_t size = (size_t)ftell(vsRaw);
    rewind(vsRaw);

    char* vsBuffer = new char[size + 1];
    size_t readSize = fread(vsBuffer, 1, size, vsRaw);
    if(readSize != size)
        printf("Bytes needed to be read: %zu\nBytes successfully read: %zu\n", size, readSize);
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

    FILE* fsRaw = fopen(fShaderPath, "rb");
    if(!fsRaw)
    {
        printf("Failed to open file at path: %s\n", fShaderPath);
        exit(-1);
    }

    fseek(fsRaw, 0, SEEK_END);
    size = ftell(fsRaw);
    rewind(fsRaw);

    char* fsBuffer = new char[size + 1];
    readSize = fread(fsBuffer, 1, size, fsRaw);
    if(readSize != size)
        printf("Bytes needed to be read: %zu\nBytes successfully read: %zu\n", size, readSize);
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

Texture LoadTexture2DFromFile(const char* imgPath)
{
    stbi_set_flip_vertically_on_load(1);
    
    int width, height, numChannels;
    uint8_t* data = stbi_load(imgPath, &width, &height, &numChannels, 0);
    if(data == nullptr)
    {
        printf("Failed to load image at path %s\n", imgPath);
        exit(-1);
    }

    GLuint texture;
    glGenTextures(1, &texture);
    
    glActiveTexture(GL_TEXTURE0 + Texture::GlobalTextureIndex);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if(numChannels == 1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    else if(numChannels == 2)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
    else if(numChannels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if(numChannels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glActiveTexture(GL_TEXTURE0);

    return {texture, Texture::GlobalTextureIndex++, (uint32_t)width, (uint32_t)height};
}

std::vector<Sprite> LoadLevelFromFile(const char* path, int& playerIndex, std::vector<Enemy>& enemies)
{
    FILE* levelRaw = fopen(path, "rb");
    if(levelRaw == nullptr)
    {
        printf("Failed to load level from file at: %s\n", path);
        exit(-1);
    }

    std::vector<Sprite> level;
    level.resize(25 * 50);

    char buffer[51];
    for(int i = 24; i >= 0; i--)
    if(fscanf(levelRaw, "%50s", buffer) != EOF)
    {
        if(buffer[i] == '/')
            continue;

        int length = (int)strlen(buffer);
        if(length != 50)
            printf("Row number %d of level is incomplete! There are %d blocks defined!\n", i, length);

        for(int j = 0; j < length; j++)
        {
            if(buffer[j] == '_')
                level.at(j + i * 50) = {-1, -1, AIR, {0.0f, 0.0f}, false};
            else if(buffer[j] == 'B')
                level.at(j + i * 50) = {0, 0, BRICK, {(float)j, (float)i}, false};
            else if(buffer[j] == 'G')
                level.at(j + i * 50) = {1, 0, GRASS, {(float)j, (float)i}, false};
            else if(buffer[j] == 'C')
                level.at(j + i * 50) = {0, 1, COIN, {(float)j, (float)i}, false};
            else if(buffer[j] == 'E')
            {
                level.at(j + i * 50) = {1, 1, ENEMY, {(float)j, (float)i}, false};
                enemies.push_back( {level.at(j + i * 50), glm::vec2(-1.0, 0.0), true} );
            }
            else if(buffer[j] == 'P')
            {
                level.at(j + i * 50) = {0, 3, PLAYER, {(float)j, (float)i}, false};
                playerIndex = j + i * 50;
            }
        }
    }

    fclose(levelRaw);
    return level;
}
