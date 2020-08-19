#pragma once
#include "shader.h"
#include "texture.h"

Shader LoadShadersFromFiles(const char* vShaderPath, const char* fShaderPath);
Texture LoadTexture2DFromFile(const char* imgPath);