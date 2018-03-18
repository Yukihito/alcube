__kernel void collectCollisions(
  __global ActorState* actorStates,
  __global SoftBodyState* softBodyStates
) {
  size_t subIndex = get_global_id(0);
  ushort actorIndex = softBodyStates[subIndex].actorIndex;
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->intersectionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = actorState->intersections;
  uchar collisionCount = 0;
  for (uchar i = 0; i < count; i++) {
    /*
    if (intersections[actorState->collisionIndices[i]].type == ACTOR_TYPE_FLUID) {
      continue;
      }*/
    float3 relativeSpeed = intersections[i].type == ACTOR_TYPE_RIGID_BODY || intersections[i].type == ACTOR_TYPE_FLUID ? actorStates[intersections[i].otherIndex].linearVelocity - actorState->linearVelocity : - actorState->linearVelocity;
    intersections[i].speed = dot(intersections[i].normal, relativeSpeed);
    if (intersections[i].speed < 0.0f) {
      actorState->collisionIndices[collisionCount] = i;
      collisionCount++;
    }
  }
  actorState->collisionCount = collisionCount;
  actorState->massForCollision = actorState->mass / collisionCount;
}

void accumulateConstraintImpulse(
  __global Actor* actor,
  __global ActorState* actorState,
  __global ActorState* actorStates,
  __global Intersection* intersection,
  __global SoftBodyState* softBodyStates,
  float3* acc
) {
  if (intersection->speed >= 0.0f) {
    return;
  }
  unsigned int intersectionType = intersection->type;
  ushort otherIndex = intersection->otherIndex;
  float mass = actorState->massForCollision;
  float elasticity = intersectionType == ACTOR_TYPE_RIGID_BODY ? softBodyStates[actor->subPhysicalQuantityIndex].elasticity * softBodyStates[actorStates[otherIndex].constants.subPhysicalQuantityIndex].elasticity : softBodyStates[actor->subPhysicalQuantityIndex].elasticity;
  float massRatio = intersectionType == ACTOR_TYPE_RIGID_BODY ? mass / actorStates[otherIndex].massForCollision : 0.0f;
  if (intersectionType == ACTOR_TYPE_FLUID) {
    massRatio = mass / actorStates[otherIndex].mass;
  }
  float3 intersectionNormal = intersection->normal;
  float speedOnIntersectionNormal = dot(actorState->linearVelocity, intersectionNormal);
  float affectedSpeed = (intersection->speed * (1.0f + elasticity) / (1.0f + massRatio)) + speedOnIntersectionNormal;
  *acc += (actorState->linearVelocity + (affectedSpeed - speedOnIntersectionNormal) * intersectionNormal) * mass;
}

__kernel void updateByConstraintImpulse(
  __global ActorState* actorStates,
  __global SoftBodyState* softBodyStates
) {
  size_t subIndex = get_global_id(0);
  ushort actorIndex = softBodyStates[subIndex].actorIndex;
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->collisionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = actorState->intersections;
  __global Actor* actor = &actorStates[actorIndex].constants;

  float3 impulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    /*
    if (intersections[actorState->collisionIndices[i]].type == ACTOR_TYPE_FLUID) {
      continue;
      }*/
    accumulateConstraintImpulse(actor, actorState, actorStates, &intersections[actorState->collisionIndices[i]], softBodyStates, &impulse);
  }
  actorState->linearVelocity = impulse / actorState->mass;
}
