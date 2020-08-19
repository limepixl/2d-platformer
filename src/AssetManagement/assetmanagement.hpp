#pragma once
#include "shader.hpp"
#include "texture.hpp"

Shader LoadShadersFromFiles(const char* vShaderPath, const char* fShaderPath);
Texture LoadTexture2DFromFile(const char* imgPath);