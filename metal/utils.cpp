#include "utils.h"
#include "CGPlus.h"
#include <stdio.h>

MTL::Library *loadLibrary(MTL::Device *device, const char *shaderPath)
{
  using NS::StringEncoding::UTF8StringEncoding;

  const char *shaderSource = (const char *)CGPlus::readFileSync(shaderPath, nullptr);

  NS::Error *error = nullptr;
  MTL::Library *library = device->newLibrary(NS::String::string(shaderSource, UTF8StringEncoding), nullptr, &error);
  if (!library)
  {
    printf("%s", error->localizedDescription()->utf8String());
  }
  return library;
}