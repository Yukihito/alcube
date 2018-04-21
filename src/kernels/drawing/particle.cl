__kernel void updateDrawingBuffer(
  __global float3* positions,
  __global PhysicalQuantity* physicalQuantities
) {
  size_t i = get_global_id(0);
  positions[i] = physicalQuantities[i].position;
}

__kernel void updateDrawingBuffer_linearMomentumToColor(
  __global float3* positions,
  __global float3* colors,
  __global PhysicalQuantity* physicalQuantities
) {
  size_t i = get_global_id(0);
  positions[i] = physicalQuantities[i].position;
  colors[i] = fabs(physicalQuantities[i].linearMomentum);
}

__kernel void updateDrawingBuffer_rotation(
  __global float3* positions,
  __global float16* rotations,
  __global PhysicalQuantity* physicalQuantities
) {
  size_t i = get_global_id(0);
  positions[i] = physicalQuantities[i].position;
  __global float4* r = &physicalQuantities[i].rotation;
  float x = r->x;
  float y = r->y;
  float z = r->z;
  float w = r->w;
  rotations[i] = (float16)(
    1.0f - 2.0f * (y*y + z*z), 2.0f * (x*y + w*z), 2.0f * (x*z - w*y), 0.0f,
    2.0f * (x*y - w*z), 1.0f - 2.0f * (x*x + z*z), 2.0f * (y*z + w*x), 0.0f,
    2.0f * (x*z + w*y), 2.0f * (y*z - w*x), 1.0f - 2.0f * (x*x + y*y), 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  );
}
