__kernel void postProcessing(
  __global const Grid* grid,
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global RigidBodyState* currentStates,
  __global RigidBodyState* nextStates,
  const float deltaTime
) {
  size_t cellIndex = get_global_id(0);
  float3 maxLinearMomentum = (float3)((2.0f * (1.0f / deltaTime)) * cells[cellIndex].mass);
  nextStates[cellIndex].angularMomentum = cellVars[cellIndex].angularVelocity * cellVars[cellIndex].momentOfInertia * 0.999f;
  nextStates[cellIndex].linearMomentum = clamp(cellVars[cellIndex].linearVelocity * cells[cellIndex].mass * 0.999f, -maxLinearMomentum, maxLinearMomentum);

  if (length(nextStates[cellIndex].linearMomentum) < 0.01f) {
    nextStates[cellIndex].linearMomentum = (float3)(0.0f);
  }

  if (length(nextStates[cellIndex].angularMomentum) < 0.01f) {
    nextStates[cellIndex].angularMomentum = (float3)(0.0f);
  }

  float3 corner = grid->origin + (float3)(0.0001f);

  nextStates[cellIndex].position = clamp(currentStates[cellIndex].position, corner, -corner);
  nextStates[cellIndex].rotation = currentStates[cellIndex].rotation;
}
