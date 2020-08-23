#include "sprite.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

void BatchSpriteData(std::vector<Sprite>& level, Batch& batch, const glm::mat4& PV)
{
    int spriteWidth = 64;
    float fSpriteWidth = (float)spriteWidth;

    float min = 0.0f;
    float max = 4.0f * spriteWidth;

    int numSprites = (int)level.size();
    for(int i = 0; i < numSprites; i++)
    {
        Sprite& sprite = level[i];
        if(sprite.xIndex == -1 || sprite.yIndex == -1)
            continue;

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

inline void CheckCoinCollision(Sprite& s1, Sprite& s2, Player& player)
{
    if(s1.type == COIN && s1 != player.sprite)
    {
        s1 = {-1, -1, AIR, {0.0f, 0.0f}, false};
        player.score++;
    }
    if(s2.type == COIN && s2 != player.sprite)
    {
        s2 = {-1, -1, AIR, {0.0f, 0.0f}, false};
        player.score++;
    }
}

void ProcessCollisions(Player& player, std::vector<Sprite>& level)
{
    glm::vec2 oldPlayerPos = player.sprite.position;
    glm::vec2 newPlayerPos = player.sprite.position + player.velocity;

    int oldPlayerPosY = int(oldPlayerPos.y);
    int oldPlayerPosYOffset = int(oldPlayerPos.y + 0.99f);

    if(int(newPlayerPos.x) - 1 < 0)
    {
        player.velocity.x = 0.0f;
        player.sprite.position.x = 1.0f;
    }
    else if(int(newPlayerPos.x) > 49)
    {
        player.velocity.x = 0.0f;
        player.sprite.position.x = 49.0f;
    }

    if(int(newPlayerPos.y) < 0)
    {
        player.velocity.y = 0.0f;
        player.sprite.position.y = 3.0f;
    }
    else if(int(newPlayerPos.y) > 49)
    {
        player.velocity.y = 0.0f;
        player.sprite.position.y = 2.0f;
    }

    if(player.velocity.x <= 0.0f)
    {
        Sprite& s1 = level[int(newPlayerPos.x) + oldPlayerPosY * 50];
        Sprite& s2 = level[int(newPlayerPos.x) + oldPlayerPosYOffset * 50];
        if((s1.type != AIR && s1.type != COIN && s1 != player.sprite) || (s2.type != AIR && s2.type != COIN && s2 != player.sprite))
        {
            player.sprite.position.x = newPlayerPos.x = (float)((int)newPlayerPos.x) + 1.0f;
            player.velocity.x = 0.0f;
        }
        else
            CheckCoinCollision(s1, s2, player);
    }
    else
    {
        Sprite& s1 = level[int(newPlayerPos.x) + 1 + oldPlayerPosY * 50];
        Sprite& s2 = level[int(newPlayerPos.x) + 1 + oldPlayerPosYOffset * 50];
        if((s1.type != AIR && s1.type != COIN && s1 != player.sprite) || (s2.type != AIR && s2.type != COIN && s2 != player.sprite))
        {
            player.sprite.position.x = newPlayerPos.x = (float)((int)newPlayerPos.x);
            player.velocity.x = 0.0f;
        }
        else
            CheckCoinCollision(s1, s2, player);
    }

    // X collision is solved so now test for Y
    if(player.velocity.y <= 0.0f)   // Moving down
    {
        Sprite& s1 = level[(int)newPlayerPos.x + int(newPlayerPos.y) * 50];
        Sprite& s2 = level[(int)(newPlayerPos.x + 0.99f) + int(newPlayerPos.y) * 50];
        if((s1.type != AIR && s1.type != COIN && s1 != player.sprite) || (s2.type != AIR && s2.type != COIN && s2 != player.sprite))
        {
            player.sprite.position.y = newPlayerPos.y = (float)((int)(newPlayerPos.y + 1));
            player.velocity.y = 0.0f;
            player.acceleration.y = 0.0f;
        }
        else
            CheckCoinCollision(s1, s2, player);
        
        Sprite& s3 = level[(int)newPlayerPos.x + int(newPlayerPos.y - 0.01f) * 50];
        Sprite& s4 = level[(int)(newPlayerPos.x + 0.99f) + int(newPlayerPos.y - 0.01f) * 50];

        bool abovePassableBlock = (s3.type == AIR || s3.type == COIN) && (s4.type == AIR || s4.type == COIN);

        // Above air block
        if(abovePassableBlock)
            player.onGround = false;

        // Above solid block
        else if(s3 != player.sprite && s4 != player.sprite)
        {
            player.acceleration.y = 0.0f;
            player.onGround = true;
            player.jumpTime = 0;
        }
    }
    else // Moving up
    {
        Sprite& s1 = level[(int)newPlayerPos.x + (int(newPlayerPos.y) + 1) * 50];
        Sprite& s2 = level[(int)(newPlayerPos.x + 0.99f) + (int(newPlayerPos.y) + 1) * 50];
        if((s1.type != AIR && s1.type != COIN && s1 != player.sprite) || (s2.type != AIR && s2.type != COIN && s2 != player.sprite))
        {
            player.sprite.position.y = (float)((int)(newPlayerPos.y));
            player.velocity.y = 0.0f;
            player.acceleration.y = 0.0f;
            player.jumpTime = 1000;
        }
        else
            CheckCoinCollision(s1, s2, player);
    }
}
