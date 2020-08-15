#include "sprite.hpp"
#include <cstdio>
#include <cstdlib>
#include <glad/glad.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

inline float Normalize(int value, float min, float max)
{
    return (value - min) / (max - min);
}

void BatchSpriteData(int xIndex, int yIndex, const glm::vec2& pos, std::vector<float>& batchedVertices, std::vector<float>& batchedUVs)
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

    std::vector<float> uvs
    {
        startX, startY,
        endX, startY,
        endX, endY,
        endX, endY,
        startX, endY,
        startX, startY
    };

    float fSpriteWidth = (float)spriteWidth;
    std::vector<float> vertices
    {
        pos.x * fSpriteWidth      , pos.y * fSpriteWidth,
        fSpriteWidth * (1 + pos.x), pos.y * fSpriteWidth,
        fSpriteWidth * (1 + pos.x), fSpriteWidth * (1 + pos.y),
        fSpriteWidth * (1 + pos.x), fSpriteWidth * (1 + pos.y),
        pos.x * fSpriteWidth      , fSpriteWidth * (1 + pos.y),
        pos.x * fSpriteWidth      , pos.y * fSpriteWidth
    };

    batchedVertices.insert(batchedVertices.end(), vertices.begin(), vertices.end());
    batchedUVs.insert(batchedUVs.end(), uvs.begin(), uvs.end());
}
