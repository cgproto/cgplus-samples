#pragma once

#include "Metal/Metal.hpp"

MTL::Library *loadLibrary(MTL::Device* device, const char *shaderPath);
