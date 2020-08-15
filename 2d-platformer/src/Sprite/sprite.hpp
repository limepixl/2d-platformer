#pragma once
#include <inttypes.h>
#include <glm/mat4x4.hpp>
#include <vector>

struct Sprite
{
	int xIndex, yIndex;
	glm::vec2 position;
	bool visible;
};

struct Batch
{
	std::vector<float> vertices;
	std::vector<float> uvs;
};

void BatchSpriteData(std::vector<Sprite>& sprites, Batch& batch, const glm::mat4& PV);