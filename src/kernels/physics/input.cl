__kernel void inputPhysicsConstants(
  __global Constants* constants,
  __global const Grid* grid,
  const float gravityAcceleration,
  const float deltaTime,
  const float sphericalShellRadius
) {
  constants->grid = grid[0];
  constants->gravityAcceleration = gravityAcceleration;
  constants->deltaTime = deltaTime;
  constants->sphericalShellRadius = sphericalShellRadius;
}

__kernel void inputFluidConstants(
  __global Constants* constants,
  __global const FluidSettings* fluidSettings
) {
  constants->fluidSettings = fluidSettings[0];
}

__kernel void inputSoftbodyConstants(
  __global Constants* constants,
  const float splitDeltaTime
) {
  constants->splitDeltaTime = splitDeltaTime;
}

__kernel void inputActors(
  __global const Actor* hostActors,
  __global ActorState* actorStates,
  unsigned short offset
) {
  size_t i = get_global_id(0) + offset;
  actorStates[i].constants = hostActors[i];
  actorStates[i].radius = hostActors[i].radius;
  actorStates[i].mass = hostActors[i].mass;
}

__kernel void inputSoftBodies(
  __global const SoftBody* hostSoftBodies,
  __global SoftBody* softBodies,
  unsigned short offset
) {
  size_t i = get_global_id(0) + offset;
  softBodies[i] = hostSoftBodies[i];
}

__kernel void inputSprings(
  __global const Spring* springs,
  __global SpringState* springStates,
  unsigned int offset
) {
  size_t i = get_global_id(0) + offset;
  springStates[i].constants = springs[i];
}

__kernel void inputFluids(
  __global Fluid* hostFluids,
  __global Fluid* fluids,
  unsigned short offset
) {
  size_t i = get_global_id(0) + offset;
  fluids[i] = hostFluids[i];
}
