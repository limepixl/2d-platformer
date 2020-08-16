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
        if(sprite.xIndex == -1 || sprite.yIndex == -1)
            continue;

        // Generate UVs from sprite index (4x4 matrix of sprites on atlas)
        int x = sprite.xIndex * spriteWidth;
        int y = sprite.yIndex * spriteWidth;

        float startX = (x + 0.5f - min) / (max - min);
        float startY = (y + 0.5f - min) / (max - min);
        float endX = (x + 63.5f - min) / (max - min);
        float endY = (y + 63.5f - min) / (max - min);

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

inline bool AABB(const glm::vec2& posA, const glm::vec2& posB)
{
    return posA.x < posB.x + 1.0f && posA.x + 1.0f > posB.x && posA.y < posB.y + 1.0f && posA.y + 1.0f > posB.y;
}

inline bool PointInsideRect(const glm::vec2& point, const glm::vec2& rect)
{
    return point.x >= rect.x && point.x <= rect.x + 1.0f && point.y >= rect.y && point.y <= rect.y + 1.0f;
}

void ProcessCollisions(Player& player, std::vector<Sprite>& sprites)
{
    glm::vec2 oldPlayerPos = player.sprite.position;
    glm::vec2 newPlayerPos = player.sprite.position + player.velocity;

    if(player.velocity.x <= 0.0f)
    {
        Sprite& s1 = sprites[int(newPlayerPos.x) + int(oldPlayerPos.y) * 50];
        Sprite& s2 = sprites[int(newPlayerPos.x) + int(oldPlayerPos.y + 0.99f) * 50];
        if((s1.xIndex != -1 && s1 != player.sprite) || (s2.xIndex != -1 && s2 != player.sprite))
        {
            player.sprite.position.x = newPlayerPos.x = (float)((int)newPlayerPos.x + 1.0f);
            player.velocity.x = 0.0f;
        }
    }
    else
    {
        Sprite& s1 = sprites[int(newPlayerPos.x + 1.0f) + int(oldPlayerPos.y) * 50];
        Sprite& s2 = sprites[int(newPlayerPos.x + 1.0f) + int(oldPlayerPos.y + 0.99f) * 50];
        if((s1.xIndex != -1 && s1 != player.sprite) || (s2.xIndex != -1 && s2 != player.sprite))
        {
            player.sprite.position.x = newPlayerPos.x = (float)((int)newPlayerPos.x);
            player.velocity.x = 0.0f;
        }
    }

    // X collision is solved so now test for y
    if(player.velocity.y <= 0.0f)   // Moving down
    {
        Sprite& s1 = sprites[int(newPlayerPos.x) + int(newPlayerPos.y) * 50];
        Sprite& s2 = sprites[int(newPlayerPos.x + 0.99f) + int(newPlayerPos.y) * 50];
        if((s1.xIndex != -1 && s1 != player.sprite) || (s2.xIndex != -1 && s2 != player.sprite))
        {
            player.sprite.position.y = (float)((int)newPlayerPos.y + 1);
            player.velocity.y = 0.0f;
        }
    }
    else    // Moving up
    {
        Sprite& s1 = sprites[int(newPlayerPos.x) + int(newPlayerPos.y + 1.0f) * 50];
        Sprite& s2 = sprites[int(newPlayerPos.x + 0.99f) + int(newPlayerPos.y + 1.0f) * 50];
        if((s1.xIndex != -1 && s1 != player.sprite) || (s2.xIndex != -1 && s2 != player.sprite))
        {
            player.sprite.position.y = (float)((int)newPlayerPos.y);
            player.velocity.y = 0.0f;
            player.onGround = true;
        }
    }
}
