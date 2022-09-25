#include <metal_stdlib>
using namespace metal;
struct v2f
{
  float4 position [[position]];
  float2 texCoord;
};
struct VertexData
{
  float3 position;
  float2 texCoord;
};

v2f vertex vertexMain(uint vertexId [[vertex_id]],
                      device const VertexData *vertexData [[buffer(0)]])
{
  const device VertexData &vd = vertexData[vertexId];
  v2f o;
  o.position = float4(vd.position, 1.0);
  o.texCoord = vd.texCoord;
  return o;
}

half4 fragment fragmentMain(v2f in [[stage_in]], texture2d<half, access::sample> tex [[texture(0)]])
{
  constexpr sampler s(address::repeat, filter::linear);

  return tex.sample(s, in.texCoord);
}