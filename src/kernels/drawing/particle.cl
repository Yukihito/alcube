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
