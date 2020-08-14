#pragma once
#include <inttypes.h>
#include <unordered_map>
#include <string>

struct Shader
{
	uint32_t ID;
	std::unordered_map<std::string, int> uniforms;
};