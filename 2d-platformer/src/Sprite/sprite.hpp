#pragma once
#include <inttypes.h>
#include <glm/vec2.hpp>
#include <vector>

void BatchSpriteData(int xIndex, int yIndex, const glm::vec2& pos, std::vector<float>& batchedVertices, std::vector<float>& batchedUVs);