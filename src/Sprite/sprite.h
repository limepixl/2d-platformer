#pragma once
#include <glm/mat4x4.hpp>
#include <vector>

enum BlockType
{
	AIR = -1,
	BRICK,
	GRASS,
	TEMP1,
	TEMP2,
	COIN,
	TEMP3,
	TEMP4,
	TEMP5,
	TEMP6,
	TEMP7,
	TEMP8,
	TEMP9,
	PLAYER,
	TEMP10,
	TEMP11,
	TEMP12,
	NUM_BLOCK_TYPES
};

struct Sprite
{
	int xIndex = -1, yIndex = -1;
	BlockType type;
	glm::vec2 position;
	bool visible;

	bool operator==(const Sprite& rhs)
	{
		return xIndex == rhs.xIndex && yIndex == rhs.yIndex && position == rhs.position && visible == rhs.visible;
	}

	bool operator!=(const Sprite& rhs)
	{
		return !operator==(rhs);
	}
};

struct Batch
{
	std::vector<float> vertices;
	std::vector<float> uvs;
};

struct Player
{
	Sprite& sprite;
	bool onGround;
	int jumpTime = 0;
	int allowedJumpTime;
	glm::vec2 velocity;
	glm::vec2 acceleration;

	int score;
};

void BatchSpriteData(std::vector<Sprite>& level, Batch& batch, const glm::mat4& PV);
void ProcessCollisions(Player& player, std::vector<Sprite>& level);
