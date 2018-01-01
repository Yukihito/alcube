__kernel void calcSpringImpulses(
  __global CellVar* cellVars,
  __global const Spring* springs,
  __global SpringVar* springVars,
  __global RigidBodyState* currentStates,
  const float deltaTime
) {
  size_t i = get_global_id(0);
  __global const Spring* spring = &springs[i];
  __global SpringVar* springVar = &springVars[i];
  float3 pm0 = spring->nodePositionsModelSpace[0];
  float4 rot0 = currentStates[spring->cellIndices[0]].rotation;
  float3 pm1 = spring->nodePositionsModelSpace[1];
  float4 rot1 = currentStates[spring->cellIndices[1]].rotation;
  float3 p0 = rotateByQuat(&pm0, &rot0);
  float3 p1 = rotateByQuat(&pm1, &rot1);
  float3 impulse = ((p1 + currentStates[spring->cellIndices[1]].position) - (p0 + currentStates[spring->cellIndices[0]].position));
  float3 direction = normalize(impulse);
  float len = length(impulse);
  springVar->linearImpulses[0] = log2(1.0f + len) * deltaTime * spring->k * direction;
  springVar->linearImpulses[1] = -springVar->linearImpulses[0];
  springVar->angularImpulses[0] = cross(p0, springVar->linearImpulses[0]);
  springVar->angularImpulses[1] = cross(p1, springVar->linearImpulses[1]);
}

__kernel void updateBySpringImpulse(
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global RigidBodyState* currentStates,
  __global SpringVar* springVars,
  const float deltaTime
) {
  size_t cellIndex = get_global_id(0);
  __global const Cell* cell = &cells[cellIndex];
  __global CellVar* cellVar = &cellVars[cellIndex];
  uchar count = cell->springCount;
  float3 linearImpulse = (float3)(0.0f);
  float3 angularImpulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    linearImpulse += springVars[cell->springIndices[i]].linearImpulses[cell->springNodeIndices[i]];
    angularImpulse += springVars[cell->springIndices[i]].angularImpulses[cell->springNodeIndices[i]];;
  }
  cellVar->linearVelocity += linearImpulse / cell->mass;
  cellVar->angularVelocity += angularImpulse / cellVar->momentOfInertia;
  float4 currentRotation = currentStates[cellIndex].rotation;
  float3 rotationDiff = cellVar->angularVelocity * deltaTime;
  float4 rotationDiffQuat = createQuatFromDisplacement(&rotationDiff);
  currentStates[cellIndex].position += cellVar->linearVelocity * deltaTime;
  currentStates[cellIndex].rotation = mulQuat(&rotationDiffQuat, &currentRotation);
}
