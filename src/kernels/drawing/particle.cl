__kernel void inputRenderers(
  __global const Renderer* hostRenderers,
  __global Renderer* renderers,
  __global const float3* hostColors,
  __global float3* colors,
  unsigned int offset
) {
  size_t i = get_global_id(0) + offset;
  renderers[i] = hostRenderers[i];
  colors[i] = hostColors[i];
}

__kernel void updateDrawingBuffer(
  __global float3* positions,
  __global float3* colors,
  __global float4* rotations0,
  __global float4* rotations1,
  __global float4* rotations2,
  __global float4* rotations3,
  __global PhysicalQuantity* physicalQuantities,
  __global Renderer* renderers,
  unsigned int offset
) {
  size_t i = get_global_id(0) + offset;
  unsigned short actorIndex = renderers[i].actorIndex;
  positions[i] = physicalQuantities[actorIndex].position;

  if (renderers[i].instanceColorType == 3) {
    colors[i] = fabs(physicalQuantities[actorIndex].linearMomentum);
  }

  if (renderers[i].refersToRotations) {
    __global float4* r = &physicalQuantities[actorIndex].rotation;
    float x = r->x;
    float y = r->y;
    float z = r->z;
    float w = r->w;
    rotations0[i] = (float4)(1.0f - 2.0f * (y*y + z*z), 2.0f * (x*y + w*z), 2.0f * (x*z - w*y), 0.0f);
    rotations1[i] = (float4)(2.0f * (x*y - w*z), 1.0f - 2.0f * (x*x + z*z), 2.0f * (y*z + w*x), 0.0f);
    rotations2[i] = (float4)(2.0f * (x*z + w*y), 2.0f * (y*z - w*x), 1.0f - 2.0f * (x*x + y*y), 0.0f);
    rotations3[i] = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
  }
}

__kernel void updateDrawingBuffer_SingleColor(
  __global float3* positions,
  __global PhysicalQuantity* physicalQuantities
) {
  size_t i = get_global_id(0);
  positions[i] = physicalQuantities[i].position;
}

__kernel void updateDrawingBuffer_InstanceColor(
  __global float3* positions,
  __global float3* colors,
  __global PhysicalQuantity* physicalQuantities
) {
  size_t i = get_global_id(0);
  positions[i] = physicalQuantities[i].position;
  colors[i] = fabs(physicalQuantities[i].linearMomentum);
}

__kernel void updateDrawingBuffer_Texture_SingleColor(
  __global float3* positions,
  __global float4* rotations0,
  __global float4* rotations1,
  __global float4* rotations2,
  __global float4* rotations3,
  __global PhysicalQuantity* physicalQuantities
) {
  size_t i = get_global_id(0);
  positions[i] = physicalQuantities[i].position;
  __global float4* r = &physicalQuantities[i].rotation;
  float x = r->x;
  float y = r->y;
  float z = r->z;
  float w = r->w;

  rotations0[i] = (float4)(1.0f - 2.0f * (y*y + z*z), 2.0f * (x*y + w*z), 2.0f * (x*z - w*y), 0.0f);
  rotations1[i] = (float4)(2.0f * (x*y - w*z), 1.0f - 2.0f * (x*x + z*z), 2.0f * (y*z + w*x), 0.0f);
  rotations2[i] = (float4)(2.0f * (x*z + w*y), 2.0f * (y*z - w*x), 1.0f - 2.0f * (x*x + y*y), 0.0f);
  rotations3[i] = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
}
