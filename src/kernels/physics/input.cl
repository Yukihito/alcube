__kernel void inputConstants(
  __global Constants* constants,
  __global const Grid* grid,
  __global const FluidSettings* fluidSettings,
  const float gravityAcceleration,
  const float deltaTime,
  const float splitDeltaTime,
  const float sphericalShellRadius
) {
  constants->grid = grid[0];
  constants->fluidSettings = fluidSettings[0];
  constants->gravityAcceleration = gravityAcceleration;
  constants->deltaTime = deltaTime;
  constants->splitDeltaTime = splitDeltaTime;
  constants->sphericalShellRadius = sphericalShellRadius;
}

__kernel void inputActors(
  __global const Actor* actors,
  __global ActorState* actorStates,
  __global const PhysicalQuantity* hostPhysicalQuantities,
  __global PhysicalQuantity* physicalQuantities,
  unsigned short offset
) {
  size_t i = get_global_id(0) + offset;
  physicalQuantities[i] = hostPhysicalQuantities[i];
  actorStates[i].constants = actors[i];
  actorStates[i].radius = physicalQuantities[i].radius;
  actorStates[i].mass = physicalQuantities[i].mass;
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
  unsigned short offset
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
