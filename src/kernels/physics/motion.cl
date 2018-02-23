__kernel void postProcessing(
  __global const Grid* grid,
  __global const Actor* actors,
  __global ActorState* actorStates,
  __global PhysicalQuantity* physicalQuantities,
  const float deltaTime
) {
  size_t actorIndex = get_global_id(0);
  float3 maxLinearMomentum = (float3)((2.0f * (1.0f / deltaTime)) * actors[actorIndex].mass);
  __global PhysicalQuantity* physicalQuantity = &physicalQuantities[actorIndex];
  physicalQuantity->angularMomentum = actorStates[actorIndex].angularVelocity * actorStates[actorIndex].momentOfInertia * 0.999f;
  physicalQuantity->linearMomentum = clamp(actorStates[actorIndex].linearVelocity * actors[actorIndex].mass * 0.999f, -maxLinearMomentum, maxLinearMomentum);

  if (length(physicalQuantity->linearMomentum) < 0.01f) {
    physicalQuantity->linearMomentum = (float3)(0.0f);
  }

  if (length(physicalQuantity->angularMomentum) < 0.01f) {
    physicalQuantity->angularMomentum = (float3)(0.0f);
  }

  float3 corner = grid->origin + (float3)(0.0001f);

  physicalQuantity->position = clamp(physicalQuantity->position, corner, -corner);
}
