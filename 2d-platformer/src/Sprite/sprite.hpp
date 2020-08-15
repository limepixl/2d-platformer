#pragma once
#include <inttypes.h>
#include <glm/vec2.hpp>
#include <vector>

struct Sprite
{
	int xIndex, yIndex;
	glm::vec2 position;
};

void BatchSpriteData(Sprite& sprite, std::vector<float>& batchedVertices, std::vector<float>& batchedUVs);