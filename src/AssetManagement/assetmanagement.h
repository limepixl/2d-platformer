#pragma once
#include <inttypes.h>
#include <unordered_map>
#include <string>
#include <vector>

struct Texture
{
	uint32_t ID, index;
	uint32_t width, height;

	static unsigned int GlobalTextureIndex;
};

struct Shader
{
	uint32_t ID;
	std::unordered_map<std::string, int> uniforms;
};

Shader LoadShadersFromFiles(const char* vShaderPath, const char* fShaderPath);
Texture LoadTexture2DFromFile(const char* imgPath);
std::vector<struct Sprite> LoadLevelFromFile(const char* path, int& playerIndex, std::vector<struct Enemy>& enemies);