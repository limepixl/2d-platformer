#pragma once
#include <inttypes.h>

struct Texture
{
	uint32_t ID, index;
	uint32_t width, height;

	static unsigned int GlobalTextureIndex;
};

