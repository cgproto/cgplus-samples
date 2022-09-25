#include "Metal/Metal.hpp"
#include <simd/simd.h>
#include "CGPlus.h"
#include "utils.h"

struct VertexData
{
  simd::float3 position;
  simd::float2 texCoord;
};

MTL::Device *_device = nullptr;
MTL::CommandQueue *_commandQueue = nullptr;
MTL::RenderPipelineState *_pipelineState = nullptr;
MTL::Buffer *_vertexBuffer = nullptr;
MTL::Buffer *_indexBuffer = nullptr;
MTL::Texture *_texture = nullptr;

void draw(CA::MetalDrawable *currentDrawable, MTL::RenderPassDescriptor *passDescriptor)
{
  NS::AutoreleasePool *pool = NS::AutoreleasePool::alloc()->init();

  MTL::CommandBuffer *commandBuffer = _commandQueue->commandBuffer();
  MTL::RenderCommandEncoder *commandEncoder = commandBuffer->renderCommandEncoder(passDescriptor);

  commandEncoder->setRenderPipelineState(_pipelineState);
  commandEncoder->setVertexBuffer(_vertexBuffer, 0, 0);
  commandEncoder->setFragmentTexture(_texture, 0);
  commandEncoder->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                        6, MTL::IndexType::IndexTypeUInt16,
                                        _indexBuffer,
                                        0);

  commandEncoder->endEncoding();
  commandBuffer->presentDrawable(currentDrawable);
  commandBuffer->commit();

  pool->release();
}
void init(MTL::Device *device)
{
  using NS::StringEncoding::UTF8StringEncoding;

  _device = device->retain();
  _commandQueue = _device->newCommandQueue();

  MTL::Library *library = loadLibrary(_device, "shader.metal");
  MTL::Function *vertexFunction = library->newFunction(NS::String::string("vertexMain", UTF8StringEncoding));
  MTL::Function *fragmentFunction = library->newFunction(NS::String::string("fragmentMain", UTF8StringEncoding));
  MTL::RenderPipelineDescriptor *pipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
  pipelineDescriptor->setVertexFunction(vertexFunction);
  pipelineDescriptor->setFragmentFunction(fragmentFunction);
  pipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm);
  NS::Error *error = nullptr;
  _pipelineState = _device->newRenderPipelineState(pipelineDescriptor, &error);
  if (!_pipelineState)
  {
    printf("%s", error->localizedDescription()->utf8String());
  }

  _texture = loadTexture(_device, "../../resources/logo.png");

  VertexData verts[4] = {
      {{-0.8f, 0.8f, 0.0f}, {0.0f, 0.0f}},
      {{-0.8f, -0.8f, 0.0f}, {0.0f, 1.0f}},
      {{0.8f, -0.8f, 0.0f}, {1.0f, 1.0f}},
      {{0.8f, 0.8f, 0.0f}, {1.0f, 0.0f}}};
  uint16_t indices[] = {
      0, 1, 3,
      1, 2, 3};

  const size_t vertexDataSize = sizeof(verts);
  const size_t indexDataSize = sizeof(indices);
  _vertexBuffer = _device->newBuffer(vertexDataSize, MTL::ResourceStorageModeShared);
  _indexBuffer = _device->newBuffer(indexDataSize, MTL::ResourceStorageModeShared);
  memcpy(_vertexBuffer->contents(), verts, vertexDataSize);
  memcpy(_indexBuffer->contents(), indices, indexDataSize);

  vertexFunction->release();
  fragmentFunction->release();
  pipelineDescriptor->release();
  library->release();
}
void release()
{
  _vertexBuffer->release();
  _indexBuffer->release();
  _pipelineState->release();
  _commandQueue->release();
  _device->release();
}
void resize(int, int) {}
int main()
{
  CGPlus::MetalAppConfig config;
  config.initCallback = init;
  config.renderCallback = draw;
  config.exitCallback = release;
  config.resizeCallback = resize;
  config.clearColor = MTL::ClearColor::Make(0.0f, 0.0f, 0.0f, 1.0f);
  CGPlus::createApp(&config);
}