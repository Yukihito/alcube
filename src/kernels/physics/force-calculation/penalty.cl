void accumulatePenaltyImpulse(
  __global Intersection* intersection,
  const float deltaTime,
  float penaltyFactor,
  float3* acc
) {
  *acc += (-penaltyFactor * log2(intersection->length + 1.0f) * deltaTime) * intersection->normal;
}

__kernel void updateByPenaltyImpulse(
  __global ActorState* actorStates,
  __global SoftBodyState* softBodyStates,
  __global Constants* constants
) {
  size_t subIndex = get_global_id(0);
  ushort actorIndex = softBodyStates[subIndex].actorIndex;
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->intersectionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = actorState->intersections;
  float3 impulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    accumulatePenaltyImpulse(&intersections[i], constants->deltaTime, 64.0f, &impulse);
  }
  actorState->linearVelocity += impulse / actorState->mass;
}
