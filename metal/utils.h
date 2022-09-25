#pragma once

#include "Metal/Metal.hpp"

MTL::Texture *loadTexture(MTL::Device *device, const char *imagePath);
MTL::Library *loadLibrary(MTL::Device* device, const char *shaderPath);
