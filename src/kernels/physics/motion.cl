__kernel void postProcessing(
  __global const Grid* grid,
  __global const Actor* actors,
  __global ActorState* actorStates,
  __global RigidBodyState* nextStates,
  const float deltaTime
) {
  size_t actorIndex = get_global_id(0);
  float3 maxLinearMomentum = (float3)((2.0f * (1.0f / deltaTime)) * actors[actorIndex].mass);
  __global RigidBodyState* state = &nextStates[actorIndex];
  state->angularMomentum = actorStates[actorIndex].angularVelocity * actorStates[actorIndex].momentOfInertia * 0.999f;
  state->linearMomentum = clamp(actorStates[actorIndex].linearVelocity * actors[actorIndex].mass * 0.999f, -maxLinearMomentum, maxLinearMomentum);

  if (length(state->linearMomentum) < 0.01f) {
    state->linearMomentum = (float3)(0.0f);
  }

  if (length(state->angularMomentum) < 0.01f) {
    state->angularMomentum = (float3)(0.0f);
  }

  float3 corner = grid->origin + (float3)(0.0001f);

  state->position = clamp(state->position, corner, -corner);
}
