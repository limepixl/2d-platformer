#include "sprite.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

Batch PreallocateBatch()
{
    // Set up VAO for all batched data
    Batch batch;
	glGenVertexArrays(1, &batch.VAO);
	glBindVertexArray(batch.VAO);

	// Allocate 1000 * 2 * sizeof(float) bytes for positions and uvs separately
	glGenBuffers(2, batch.VBOs);
	glBindBuffer(GL_ARRAY_BUFFER, batch.VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(glm::vec2), nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, batch.VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(glm::vec2), nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glGenBuffers(1, &batch.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1000 * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);

    return batch;
}

void BatchSpriteData(std::vector<Sprite>& level, Batch& batch, const glm::mat4& PV)
{
    int spriteWidth = 64;
    float fSpriteWidth = (float)spriteWidth;

    float min = 0.0f;
    float max = 4.0f * spriteWidth;

    unsigned int index = 0;
    int numSprites = (int)level.size();
    for(int i = 0; i < numSprites; i++)
    {
        Sprite& sprite = level[i];
        if(sprite.type == AIR)
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
            startX, endY,
        };

        glm::vec2& pos = sprite.position;
        std::vector<float> vertices
        {
            pos.x * fSpriteWidth      , pos.y * fSpriteWidth,
            fSpriteWidth * (1 + pos.x), pos.y * fSpriteWidth,
            fSpriteWidth * (1 + pos.x), fSpriteWidth * (1 + pos.y),
            pos.x * fSpriteWidth      , fSpriteWidth * (1 + pos.y),
        };

        std::vector<unsigned int> indices
        {
            0 + 4*index, 1 + 4*index, 2 + 4*index,
            2 + 4*index, 3 + 4*index, 0 + 4*index
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
        batch.indices.insert(batch.indices.end(), indices.begin(), indices.end());
        index++;
    }

    glBindBuffer(GL_ARRAY_BUFFER, batch.VBOs[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, batch.vertices.size() * sizeof(float), batch.vertices.data());

	glBindBuffer(GL_ARRAY_BUFFER, batch.VBOs[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, batch.uvs.size() * sizeof(float), batch.uvs.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, batch.indices.size() * sizeof(unsigned int), batch.indices.data());
}

inline bool AABB(const glm::vec2& p1, const glm::vec2& size1, const glm::vec2& p2, const glm::vec2& size2)
{
    return (p1.x < p2.x + size2.x && p1.x + size1.x > p2.x) && (p1.y < p2.y + size2.y && p1.y + size1.y > p2.y);
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

    // Check if inside coin
    glm::vec2 playerCenter = player.sprite.position + glm::vec2(0.5f, 0.5f);
    Sprite& centerSprite = level[int(playerCenter.x) + int(playerCenter.y) * 50];
    if(centerSprite.type == COIN && centerSprite != player.sprite)
    {
        centerSprite = {-1, -1, AIR, {0.0f, 0.0f}, false};
        player.score++;
    }

    if(player.velocity.x <= 0.0f)
    {
        Sprite& s1 = level[int(newPlayerPos.x) + oldPlayerPosY * 50];
        Sprite& s2 = level[int(newPlayerPos.x) + oldPlayerPosYOffset * 50];
        if((s1.type != AIR && s1.type != COIN && s1 != player.sprite) || (s2.type != AIR && s2.type != COIN && s2 != player.sprite))
        {
            player.sprite.position.x = newPlayerPos.x = (float)((int)newPlayerPos.x) + 1.0f;
            player.velocity.x = 0.0f;

            if(player.velocity.y < 0.0f)
                player.velocity.y *= 0.5f;
        }
    }
    else
    {
        Sprite& s1 = level[int(newPlayerPos.x) + 1 + oldPlayerPosY * 50];
        Sprite& s2 = level[int(newPlayerPos.x) + 1 + oldPlayerPosYOffset * 50];
        if((s1.type != AIR && s1.type != COIN && s1 != player.sprite) || (s2.type != AIR && s2.type != COIN && s2 != player.sprite))
        {
            player.sprite.position.x = newPlayerPos.x = (float)((int)newPlayerPos.x);
            player.velocity.x = 0.0f;
            
            if(player.velocity.y < 0.0f)
                player.velocity.y *= 0.5f;
        }
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
    }
}
