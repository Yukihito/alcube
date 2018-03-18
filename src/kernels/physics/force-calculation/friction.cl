void accumulateFrictionalImpulse(
  __global ActorState* actorState,
  __global ActorState* actorStates,
  __global Intersection* intersection,
  float3* linearMomentumAcc,
  float3* angularMomentumAcc
) {
  unsigned int intersectionType = intersection->type;
  ushort otherIndex = intersection->otherIndex;
  float mass = actorState->massForIntersection;
  float s = intersectionType == 0 ? 1.0f / actorStates[otherIndex].massForIntersection : 0;
  float t = (7.0f / 2.0f) * ((1.0f / mass) + s);
  float3 intersectionNormal = intersection->normal;
  float3 otherLinearVelocity = intersectionType == 0 ? actorStates[otherIndex].linearVelocity : (float3)(0.0f);
  float3 relativeVelocity = otherLinearVelocity - actorState->linearVelocity;
  float3 relativeVelocityOnSurface = relativeVelocity - intersectionNormal * dot(relativeVelocity, intersectionNormal);
  float3 workingPoint = actorState->radius * intersectionNormal;
  float3 workingPointVelocity = cross(actorState->angularVelocity, workingPoint);
  float3 otherWorkingPointVelocity = intersectionType == 0 ? cross(actorStates[otherIndex].angularVelocity, -actorStates[otherIndex].radius * intersectionNormal) : (float3)(0.0f);
  float3 relativeWorkingPointVelocity = otherWorkingPointVelocity - workingPointVelocity;
  float3 frictionalImpulse = (relativeVelocityOnSurface + relativeWorkingPointVelocity) / t;
  *linearMomentumAcc += frictionalImpulse;
  *angularMomentumAcc += cross(workingPoint, frictionalImpulse);
}

__kernel void updateByFrictionalImpulse(
  __global ActorState* actorStates,
  __global SoftBody* softBodys
) {
  size_t subIndex = get_global_id(0);
  ushort actorIndex = softBodys[subIndex].actorIndex;
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->intersectionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = actorState->intersections;
  float3 impulse = (float3)(0.0f);
  float3 angularImpulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    if (intersections[i].type == ACTOR_TYPE_FLUID) {
      continue;
    }
    accumulateFrictionalImpulse(actorState, actorStates, &intersections[i], &impulse, &angularImpulse);
  }
  actorState->linearVelocity += impulse / actorState->mass;
  actorState->angularVelocity += angularImpulse / actorState->momentOfInertia;
}
