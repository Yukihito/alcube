__kernel void postProcessing(
  __global const Grid* grid,
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global RigidBodyState* nextStates,
  const float deltaTime
) {
  size_t cellIndex = get_global_id(0);
  float3 maxLinearMomentum = (float3)((2.0f * (1.0f / deltaTime)) * cells[cellIndex].mass);
  __global RigidBodyState* state = &nextStates[cellIndex];
  state->angularMomentum = cellVars[cellIndex].angularVelocity * cellVars[cellIndex].momentOfInertia * 0.999f;
  state->linearMomentum = clamp(cellVars[cellIndex].linearVelocity * cells[cellIndex].mass * 0.999f, -maxLinearMomentum, maxLinearMomentum);

  if (length(state->linearMomentum) < 0.01f) {
    state->linearMomentum = (float3)(0.0f);
  }

  if (length(state->angularMomentum) < 0.01f) {
    state->angularMomentum = (float3)(0.0f);
  }

  float3 corner = grid->origin + (float3)(0.0001f);

  state->position = clamp(state->position, corner, -corner);
}
