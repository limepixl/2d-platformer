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

        glm::vec4 startPos(vertices[0], vertices[1], 0.0f, 1.0f);
        glm::vec4 endPos(startPos.x + fSpriteWidth, startPos.y + fSpriteWidth, 0.0f, 1.0f);
        startPos = PV * startPos;
        endPos = PV * endPos;
        if(endPos.x < -1.0f || startPos.x > 1.0f || endPos.y < -1.0f || startPos.y > 1.0f)
        {
            sprite.visible = false;
            continue;
        }

        batch.vertices.insert(batch.vertices.end(), vertices.begin(), vertices.end());
        batch.uvs.insert(batch.uvs.end(), uvs.begin(), uvs.end());
    }
}
