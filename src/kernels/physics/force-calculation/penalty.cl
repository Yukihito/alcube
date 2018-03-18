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
  const float deltaTime
) {
  size_t actorIndex = get_global_id(0);
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->intersectionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = actorState->intersections;
  float3 impulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    if (actorState->constants.type == ACTOR_TYPE_FLUID && intersections[i].type == ACTOR_TYPE_FLUID) {
      continue;
    }
    accumulatePenaltyImpulse(&intersections[i], deltaTime, 64.0f, &impulse);
  }
  actorState->linearVelocity += impulse / actorState->mass;
}
