void accumulatePenaltyImpulse(
  __global Intersection* intersection,
  const float deltaTime,
  float3* acc
) {
  *acc += (-64.0f * log2(intersection->length + 1.0f) * deltaTime) * intersection->normal;
}

__kernel void updateByPenaltyImpulse(
  __global const Actor* actors,
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
    accumulatePenaltyImpulse(&intersections[i], deltaTime, &impulse);
  }
  actorState->linearVelocity += impulse / actors[actorIndex].mass;
}

void accumulateFrictionalImpulse(
  __global const Actor* actor,
  __global ActorState* actorState,
  __global const Actor* actors,
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
  float3 workingPoint = actor->radius * intersectionNormal;
  float3 workingPointVelocity = cross(actorState->angularVelocity, workingPoint);
  float3 otherWorkingPointVelocity = intersectionType == 0 ? cross(actorStates[otherIndex].angularVelocity, -actors[otherIndex].radius * intersectionNormal) : (float3)(0.0f);
  float3 relativeWorkingPointVelocity = otherWorkingPointVelocity - workingPointVelocity;
  float3 frictionalImpulse = (relativeVelocityOnSurface + relativeWorkingPointVelocity) / t;
  *linearMomentumAcc += frictionalImpulse;
  *angularMomentumAcc += cross(workingPoint, frictionalImpulse);
}

__kernel void updateByFrictionalImpulse(
  __global const Actor* actors,
  __global ActorState* actorStates
) {
  size_t actorIndex = get_global_id(0);
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->intersectionCount;
  if (count == 0) {
    return;
  }
  __global const Actor* actor = &actors[actorIndex];
  __global Intersection* intersections = actorState->intersections;
  float3 impulse = (float3)(0.0f);
  float3 angularImpulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    if (intersections[i].type == 3) {
      continue;
    }
    accumulateFrictionalImpulse(actor, actorState, actors, actorStates, &intersections[i], &impulse, &angularImpulse);
  }
  actorState->linearVelocity += impulse / actor->mass;
  actorState->angularVelocity += angularImpulse / actorState->momentOfInertia;
}

__kernel void collectCollisions(
  __global const Actor* actors,
  __global ActorState* actorStates
) {
  size_t actorIndex = get_global_id(0);
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->intersectionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = actorState->intersections;
  uchar collisionCount = 0;
  for (uchar i = 0; i < count; i++) {
    float3 relativeSpeed = intersections[i].type == 0 ? actorStates[intersections[i].otherIndex].linearVelocity - actorState->linearVelocity : - actorState->linearVelocity;
    intersections[i].speed = dot(intersections[i].normal, relativeSpeed);
    if (intersections[i].type != 3 && intersections[i].speed < 0.0f) {
      actorState->collisionIndices[collisionCount] = i;
      collisionCount++;
    }
  }
  actorState->collisionCount = collisionCount;
  actorState->massForCollision = actors[actorIndex].mass / collisionCount;
}

void accumulateConstraintImpulse(
  __global const Actor* actor,
  __global ActorState* actorState,
  __global const Actor* actors,
  __global ActorState* actorStates,
  __global Intersection* intersection,
  float3* acc
) {
  if (intersection->speed >= 0.0f) {
    return;
  }
  unsigned int intersectionType = intersection->type;
  ushort otherIndex = intersection->otherIndex;
  float mass = actorState->massForCollision;
  float elasticity = intersectionType == 0 ? actor->elasticity * actors[otherIndex].elasticity : actor->elasticity;
  float massRatio = intersectionType == 0 ? mass / actorStates[otherIndex].massForCollision : 0.0f;
  float3 intersectionNormal = intersection->normal;
  float speedOnIntersectionNormal = dot(actorState->linearVelocity, intersectionNormal);
  float affectedSpeed = (intersection->speed * (1.0f + elasticity) / (1.0f + massRatio)) + speedOnIntersectionNormal;
  *acc += (actorState->linearVelocity + (affectedSpeed - speedOnIntersectionNormal) * intersectionNormal) * mass;
}

__kernel void updateByConstraintImpulse(
  __global const Actor* actors,
  __global ActorState* actorStates
) {
  size_t actorIndex = get_global_id(0);
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->collisionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = actorState->intersections;
  __global const Actor* actor = &actors[actorIndex];

  float3 impulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    if (intersections[actorState->collisionIndices[i]].type == 3) {
      continue;
    }
    accumulateConstraintImpulse(actor, actorState, actors, actorStates, &intersections[actorState->collisionIndices[i]], &impulse);
  }
  actorState->linearVelocity = impulse / actor->mass;
}
