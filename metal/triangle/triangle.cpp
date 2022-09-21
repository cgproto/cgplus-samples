#include "Metal/Metal.hpp"
#include <simd/simd.h>
#include "CGPlus.h"
#include "utils.h"

MTL::Device *_device = nullptr;
MTL::CommandQueue *_commandQueue = nullptr;
MTL::RenderPipelineState *_pipelineState = nullptr;
MTL::Buffer *_buffer = nullptr;

void draw(CA::MetalDrawable *currentDrawable, MTL::RenderPassDescriptor *passDescriptor)
{
  NS::AutoreleasePool *pool = NS::AutoreleasePool::alloc()->init();

  MTL::CommandBuffer *commandBuffer = _commandQueue->commandBuffer();
  MTL::RenderCommandEncoder *commandEncoder = commandBuffer->renderCommandEncoder(passDescriptor);

  commandEncoder->setRenderPipelineState(_pipelineState);
  commandEncoder->setVertexBuffer(_buffer, 0, 0);
  commandEncoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));

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

  simd::float3 positions[3] = {
      {-0.8f, -0.8f, 0.0f},
      {+0.8f, -0.8f, 0.0f},
      {0.0f, 0.8f, 0.0f}};

  const size_t positionsDataSize = sizeof(positions);
  _buffer = _device->newBuffer(positionsDataSize, MTL::ResourceStorageModeShared);
  memcpy(_buffer->contents(), positions, positionsDataSize);

  vertexFunction->release();
  fragmentFunction->release();
  pipelineDescriptor->release();
  library->release();
}
void release()
{
  _buffer->release();
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