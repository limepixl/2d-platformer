#pragma once
#include <inttypes.h>

struct Sprite
{
	uint32_t VAO;
};

Sprite LoadSpriteFromAtlas(int xIndex, int yIndex);