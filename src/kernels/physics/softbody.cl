__kernel void calcSpringImpulses(
  __global ActorState* actorStates,
  __global const Spring* springs,
  __global SpringVar* springVars,
  __global RigidBodyState* nextStates,
  const float deltaTime
) {
  size_t i = get_global_id(0);
  __global const Spring* spring = &springs[i];
  __global SpringVar* springVar = &springVars[i];
  float3 pm0 = spring->nodePositionsModelSpace[0];
  float4 rot0 = nextStates[spring->actorIndices[0]].rotation;
  float3 pm1 = spring->nodePositionsModelSpace[1];
  float4 rot1 = nextStates[spring->actorIndices[1]].rotation;
  float3 p0 = rotateByQuat(pm0, rot0);
  float3 p1 = rotateByQuat(pm1, rot1);
  float3 impulse = ((p1 + nextStates[spring->actorIndices[1]].position) - (p0 + nextStates[spring->actorIndices[0]].position));
  float3 direction = normalize(impulse);
  float len = length(impulse);
  springVar->linearImpulses[0] = log2(1.0f + len) * deltaTime * spring->k * direction;
  springVar->linearImpulses[1] = -springVar->linearImpulses[0];
  springVar->angularImpulses[0] = cross(p0, springVar->linearImpulses[0]);
  springVar->angularImpulses[1] = cross(p1, springVar->linearImpulses[1]);
}

__kernel void updateBySpringImpulse(
  __global const Actor* actors,
  __global ActorState* actorStates,
  __global RigidBodyState* nextStates,
  __global SpringVar* springVars,
  const float deltaTime
) {
  size_t actorIndex = get_global_id(0);
  __global const Actor* actor = &actors[actorIndex];
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actor->springCount;
  float3 linearImpulse = (float3)(0.0f);
  float3 angularImpulse = (float3)(0.0f);

  for (uchar i = 0; i < count; i++) {
    linearImpulse += springVars[actor->springIndices[i]].linearImpulses[actor->springNodeIndices[i]];
    angularImpulse += springVars[actor->springIndices[i]].angularImpulses[actor->springNodeIndices[i]];;
  }

  actorState->linearVelocity += linearImpulse / actor->mass;
  actorState->angularVelocity += angularImpulse / actorState->momentOfInertia;
  nextStates[actorIndex].position += actorState->linearVelocity * deltaTime;
  nextStates[actorIndex].rotation = mulQuat(createQuatFromDisplacement(actorState->angularVelocity * deltaTime), nextStates[actorIndex].rotation);
}
