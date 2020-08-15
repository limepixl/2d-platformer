#include "sprite.hpp"
#include <cstdio>
#include <cstdlib>
#include <glad/glad.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

void BatchSpriteData(Sprite& sprite, std::vector<float>& batchedVertices, std::vector<float>& batchedUVs)
{
    if(sprite.xIndex > 3 || sprite.yIndex > 3)
    {
        printf("Invalid sprite index! Rendering last sprite.\n");
        sprite.xIndex = 3;
        sprite.yIndex = 3;
    }

    int spriteWidth = 64;

    // Generate UVs from sprite index (4x4 matrix of sprites on atlas)
    int x = sprite.xIndex * spriteWidth;
    int y = sprite.yIndex * spriteWidth;

    float min = 0.0f;
    float max = 4.0f * spriteWidth;
    float startX = (x - min) / (max - min);
    float startY = (y - min) / (max - min);
    float endX = (x + 64.0f - min) / (max - min);
    float endY = (y + 64.0f - min) / (max - min);

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
    glm::vec2& pos = sprite.position;
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
