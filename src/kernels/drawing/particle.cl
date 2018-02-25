__kernel void inputModelVertices(
  const __global float3* hostVertices,
  __global float3* vertices
) {
  size_t i = get_global_id(0);
  vertices[i] = hostVertices[i];
}

__kernel void transformModel(
  __global float3* vertices,
  __global float3* modelVertices,
  const uint modelVertexCount,
  __global PhysicalQuantity* physicalQuantities
) {
  size_t actorIndex = get_global_id(0);
  uint headVertexIndex = modelVertexCount * actorIndex;
  float3 position = physicalQuantities[actorIndex].position;
  for (uint i = 0; i < modelVertexCount; i++) {
    vertices[i + headVertexIndex] = modelVertices[i] + position;
  }
}
