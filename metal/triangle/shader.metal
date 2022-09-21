#include <metal_stdlib>
using namespace metal;

float4 vertex vertexMain(uint vertexId [[vertex_id]],
                         device const float3 *positions [[buffer(0)]])
{
  return float4(positions[vertexId], 1.0);
}

half4 fragment fragmentMain(float4 in [[stage_in]])
{
  return half4(1.0);
}