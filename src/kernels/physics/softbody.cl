__kernel void calcSpringImpulses(
  __global const Spring* springs,
  __global SpringVar* springVars,
  __global RigidBodyState* currentStates
) {
  size_t i = get_global_id(0);
  Spring* spring = &spring[i];
  SpringVar* springVar = &springVars[i];
  float3 p0 = rotateByQuat(spring.nodePositionsModelSpace[0], currentStates[spring.cellIndices[0]]);
  float3 p1 = rotateByQuat(spring.nodePositionsModelSpace[1], currentStates[spring.cellIndices[1]]);
  springVar.linearImpulses[0] = spring->k * (p1 - p0);
  springVar.linearImpulses[1] = -springVar.linearImpulses[0];
  springVar.angularImpulses[0] = cross(p0, springVar.linearImpulses[0]);
  springVar.angularImpulses[1] = cross(p1, springVar.linearImpulses[1]);
}
