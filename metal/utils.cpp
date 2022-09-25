#include "utils.h"
#include "CGPlus.h"
#include <stdio.h>
#include "stb_image.h"

MTL::Library *loadLibrary(MTL::Device *device, const char *shaderPath)
{
  using NS::StringEncoding::UTF8StringEncoding;

  const char *shaderSource = (const char *)CGPlus::readFileSync(shaderPath, nullptr);

  NS::Error *error = nullptr;
  MTL::Library *library = device->newLibrary(NS::String::string(shaderSource, UTF8StringEncoding), nullptr, &error);
  free((void *)shaderSource);
  if (!library)
  {
    printf("%s", error->localizedDescription()->utf8String());
  }
  return library;
}

MTL::Texture *loadTexture(MTL::Device *device, const char *imagePath)
{
  int width, height, numChannels;
  unsigned char *data = stbi_load(imagePath, &width, &height, &numChannels, 4);
  if (data)
  {
    MTL::TextureDescriptor *textureDesc = MTL::TextureDescriptor::alloc()->init();
    textureDesc->setWidth(width);
    textureDesc->setHeight(height);
    textureDesc->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    textureDesc->setTextureType(MTL::TextureType2D);
    textureDesc->setStorageMode(MTL::StorageModeShared);
    textureDesc->setUsage(MTL::ResourceUsageSample | MTL::ResourceUsageRead);
    MTL::Texture *texture = device->newTexture(textureDesc);
    texture->replaceRegion(MTL::Region(0, 0, 0, width, height, 1), 0, data, width * 4);
    textureDesc->release();
    stbi_image_free(data);
    return texture;
  }
  return nullptr;
}