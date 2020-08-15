#include "sprite.hpp"
#include <cstdio>
#include <cstdlib>
#include <glad/glad.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

void BatchSpriteData(std::vector<Sprite>& sprites, Batch& batch, const glm::mat4& PV)
{
    int spriteWidth = 64;
    float fSpriteWidth = (float)spriteWidth;

    float min = 0.0f;
    float max = 4.0f * spriteWidth;

    int numSprites = (int)sprites.size();
    for(int i = 0; i < numSprites; i++)
    {
        Sprite& sprite = sprites[i];

        if(sprite.xIndex > 3 || sprite.yIndex > 3)
        {
            printf("Invalid sprite index! Rendering last sprite.\n");
            sprite.xIndex = 3;
            sprite.yIndex = 3;
        }

        // Generate UVs from sprite index (4x4 matrix of sprites on atlas)
        int x = sprite.xIndex * spriteWidth;
        int y = sprite.yIndex * spriteWidth;

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

        sprite.visible = true;
        for(int j = 0; j < 12; j+=2)
        {
            glm::vec4 startPos(vertices[j], vertices[j + 1], 0.0f, 1.0f);
            glm::vec4 endPos(startPos.x + fSpriteWidth, startPos.y + fSpriteWidth, 0.0f, 1.0f);
            startPos = PV * startPos;
            endPos = PV * endPos;
            if(endPos.x < -1.0f || startPos.x > 1.0f || endPos.y < -1.0f || startPos.y > 1.0f)
            {
                sprite.visible = false;
                break;
            }
        }

        if(!sprite.visible)
            continue;

        batch.batchedVertices.insert(batch.batchedVertices.end(), vertices.begin(), vertices.end());
        batch.batchedUVs.insert(batch.batchedUVs.end(), uvs.begin(), uvs.end());
    }
}
