#include "sprite.hpp"
#include <cstdio>
#include <cstdlib>
#include <glad/glad.h>

inline float Normalize(int value, float min, float max)
{
    return (value - min) / (max - min);
}

Sprite LoadSpriteFromAtlas(int xIndex, int yIndex)
{
    if(xIndex > 3 || yIndex > 3)
    {
        printf("Invalid sprite index! Rendering last sprite.\n");
        xIndex = 3;
        yIndex = 3;
    }

    int spriteWidth = 64;

    // Generate UVs from sprite index (4x4 matrix of sprites on atlas)
    int x = xIndex * spriteWidth;
    int y = yIndex * spriteWidth;

    float startX = Normalize(x, 0.0f, 4.0f * spriteWidth);
    float startY = Normalize(y, 0.0f, 4.0f * spriteWidth);
    float endX = Normalize(x + 64, 0.0f, 4.0f * spriteWidth);
    float endY = Normalize(y + 64, 0.0f, 4.0f * spriteWidth);

    float uvs[]
    {
        startX, startY,
        endX, startY,
        endX, endY,
        endX, endY,
        startX, endY,
        startX, startY
    };

    float fSpriteWidth = (float)spriteWidth;
    float vertices[]
    {
        0.0f, 0.0f,
        fSpriteWidth, 0.0f,
        fSpriteWidth, fSpriteWidth,
        fSpriteWidth, fSpriteWidth,
        0.0f, fSpriteWidth,
        0.0f, 0.0f
    };

    // Generate VBO data
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint vVBO;
    glGenBuffers(1, &vVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vVBO);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    GLuint tVBO;
    glGenBuffers(1, &tVBO);
    glBindBuffer(GL_ARRAY_BUFFER, tVBO);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), uvs, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    return { VAO };
}
