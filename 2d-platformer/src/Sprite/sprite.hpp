#pragma once
#include <inttypes.h>
#include <glm/mat4x4.hpp>
#include <vector>

struct Sprite
{
	int xIndex, yIndex;
	glm::vec2 position;
	bool visible;

	bool operator==(const Sprite& rhs)
	{
		return xIndex == rhs.xIndex && yIndex == rhs.yIndex && position == rhs.position && visible == rhs.visible;
	}
};

struct Batch
{
	std::vector<float> vertices;
	std::vector<float> uvs;
};

void BatchSpriteData(std::vector<Sprite>& sprites, Batch& batch, const glm::mat4& PV);

struct Player
{
	Sprite& sprite;
	bool onGround;
};

void ProcessCollisions(Player& player, std::vector<Sprite>& sprites);