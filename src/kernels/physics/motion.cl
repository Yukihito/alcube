__kernel void moveFluid(
  __global Fluid* fluids,
  __global ActorState* actorStates,
  __global Constants* constants
) {
  __global Grid* grid = &constants->grid;
  ushort actorIndex = fluids[get_global_id(0)].actorIndex;
  __global ActorState* actorState = &actorStates[actorIndex];
  __global Actor* actor = &actorState->constants;
  if (!actor->isAlive) {
    return;
  }
  actorState->linearVelocity += (actorState->fluidForce * constants->deltaTime) / constants->fluidSettings.particleMass;
  actor->position += constants->deltaTime * actorState->linearVelocity;
}

__kernel void calcSpringImpulses(
  __global Constants* constants,
  __global SpringState* springStates,
  __global ActorState* actorStates
) {
  size_t i = get_global_id(0);
  __global SpringState* springState = &springStates[i];
  __global Spring* spring = &springState->constants;
  __global Actor* actor0 = &actorStates[spring->actorIndices[0]].constants;
  __global Actor* actor1 = &actorStates[spring->actorIndices[1]].constants;
  int enabled = actor0->isAlive && actor1->isAlive;
  float3 pm0 = spring->nodePositionsModelSpace[0];
  float4 rot0 = actorStates[spring->actorIndices[0]].constants.rotation;
  float3 pm1 = spring->nodePositionsModelSpace[1];
  float4 rot1 = actorStates[spring->actorIndices[1]].constants.rotation;
  float3 p0 = rotateByQuat(pm0, rot0);
  float3 p1 = rotateByQuat(pm1, rot1);
  float3 impulse = ((p1 + actor1->position) - (p0 + actor0->position));
  float3 direction = normalize(impulse);
  float len = length(impulse);
  springState->linearImpulses[0] = enabled ? log2(1.0f + len) * constants->splitDeltaTime * spring->k * direction : (float3)(0.0f);
  springState->linearImpulses[1] = enabled ? -springState->linearImpulses[0] : (float3)(0.0f);
  springState->angularImpulses[0] = enabled ? cross(p0, springState->linearImpulses[0]) : (float3)(0.0f);
  springState->angularImpulses[1] = enabled ? cross(p1, springState->linearImpulses[1]) : (float3)(0.0f);
}

__kernel void updateBySpringImpulse(
  __global Constants* constants,
  __global SoftBody* softBodies,
  __global ActorState* actorStates,
  __global SpringState* springStates
) {
  size_t softBodyIndex = get_global_id(0);
  __global SoftBody* softBody = &softBodies[softBodyIndex];
  size_t actorIndex = softBody->actorIndex;
  __global ActorState* actorState = &actorStates[actorIndex];
  __global Actor* actor = &actorState->constants;
  if (!actor->isAlive) {
    return;
  }
  uchar count = softBody->springCount;
  float3 linearImpulse = (float3)(0.0f);
  float3 angularImpulse = (float3)(0.0f);

  for (uchar i = 0; i < count; i++) {
    linearImpulse += springStates[softBody->springIndices[i]].linearImpulses[softBody->springNodeIndices[i]];
    angularImpulse += springStates[softBody->springIndices[i]].angularImpulses[softBody->springNodeIndices[i]];;
  }

  actorState->linearVelocity += linearImpulse / actorState->mass;
  actorState->angularVelocity += angularImpulse / actorState->momentOfInertia;
  actor->position += actorState->linearVelocity * constants->splitDeltaTime;
  actor->rotation = mulQuat(createQuatFromDisplacement(actorState->angularVelocity * constants->splitDeltaTime), actor->rotation);
}

__kernel void postProcessing(
  __global Constants* constants,
  __global ActorState* actorStates
) {
  size_t actorIndex = get_global_id(0);
  __global Actor* actor = &actorStates[actorIndex].constants;
  if (!actor->isAlive) {
    return;
  }
  float3 maxLinearMomentum = (float3)((2.0f * (1.0f / constants->deltaTime)) * actorStates[actorIndex].mass);
  actor->angularMomentum = actorStates[actorIndex].angularVelocity * actorStates[actorIndex].momentOfInertia * 0.999f;
  actor->linearMomentum = clamp(actorStates[actorIndex].linearVelocity * actorStates[actorIndex].mass * 0.999f, -maxLinearMomentum, maxLinearMomentum);

  if (length(actor->linearMomentum) < 0.01f) {
    actor->linearMomentum = (float3)(0.0f);
  }

  if (length(actor->angularMomentum) < 0.01f) {
    actor->angularMomentum = (float3)(0.0f);
  }

  float3 corner = constants->grid.origin + (float3)(0.0001f);

  actor->position = clamp(actor->position, corner, -corner);
}
