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
