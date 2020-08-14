#pragma once
#include <string>
#include <unordered_map>
#include <inttypes.h>

struct Shader
{
	uint32_t ID;
	std::unordered_map<std::string, int> uniforms;
};