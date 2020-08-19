#pragma once
#include <glm/mat4x4.hpp>
#include <vector>

struct Sprite
{
	int xIndex = -1, yIndex = -1;
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
};

void BatchSpriteData(std::vector<Sprite>& level, Batch& batch, const glm::mat4& PV);
void ProcessCollisions(Player& player, std::vector<Sprite>& level);
std::vector<Sprite> LoadLevelFromFile(const char* path, int& playerIndex);
