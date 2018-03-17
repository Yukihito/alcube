__kernel void calcSpringImpulses(
  __global SpringState* springStates,
  __global PhysicalQuantity* physicalQuantities,
  const float deltaTime
) {
  size_t i = get_global_id(0);
  __global SpringState* springState = &springStates[i];
  __global Spring* spring = &springState->constants;
  float3 pm0 = spring->nodePositionsModelSpace[0];
  float4 rot0 = physicalQuantities[spring->actorIndices[0]].rotation;
  float3 pm1 = spring->nodePositionsModelSpace[1];
  float4 rot1 = physicalQuantities[spring->actorIndices[1]].rotation;
  float3 p0 = rotateByQuat(pm0, rot0);
  float3 p1 = rotateByQuat(pm1, rot1);
  float3 impulse = ((p1 + physicalQuantities[spring->actorIndices[1]].position) - (p0 + physicalQuantities[spring->actorIndices[0]].position));
  float3 direction = normalize(impulse);
  float len = length(impulse);
  springState->linearImpulses[0] = log2(1.0f + len) * deltaTime * spring->k * direction;
  springState->linearImpulses[1] = -springState->linearImpulses[0];
  springState->angularImpulses[0] = cross(p0, springState->linearImpulses[0]);
  springState->angularImpulses[1] = cross(p1, springState->linearImpulses[1]);
}

__kernel void updateBySpringImpulse(
  __global SoftBodyState* softBodyStates,
  __global ActorState* actorStates,
  __global PhysicalQuantity* physicalQuantities,
  __global SpringState* springStates,
  const float deltaTime
) {
  size_t softBodyIndex = get_global_id(0);
  __global SoftBodyState* softBodyState = &softBodyStates[softBodyIndex];
  size_t actorIndex = softBodyState->actorIndex;
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = softBodyState->springCount;
  float3 linearImpulse = (float3)(0.0f);
  float3 angularImpulse = (float3)(0.0f);

  for (uchar i = 0; i < count; i++) {
    linearImpulse += springStates[softBodyState->springIndices[i]].linearImpulses[softBodyState->springNodeIndices[i]];
    angularImpulse += springStates[softBodyState->springIndices[i]].angularImpulses[softBodyState->springNodeIndices[i]];;
  }

  actorState->linearVelocity += linearImpulse / actorState->mass;
  actorState->angularVelocity += angularImpulse / actorState->momentOfInertia;
  physicalQuantities[actorIndex].position += actorState->linearVelocity * deltaTime;
  physicalQuantities[actorIndex].rotation = mulQuat(createQuatFromDisplacement(actorState->angularVelocity * deltaTime), physicalQuantities[actorIndex].rotation);
}
