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
  __global PhysicalQuantity* physicalQuantities
) {
  size_t i = get_global_id(0);
  physicalQuantities[i] = hostPhysicalQuantities[i];
  actorStates[i].constants = actors[i];
  actorStates[i].radius = physicalQuantities[i].radius;
  actorStates[i].mass = physicalQuantities[i].mass;
}

__kernel void inputSoftBodies(
  __global const SoftBody* hostSoftBodies,
  __global SoftBody* softBodies
) {
  size_t i = get_global_id(0);
  softBodies[i] = hostSoftBodies[i];
}

__kernel void inputSprings(
  __global const Spring* springs,
  __global SpringState* springStates
) {
  size_t i = get_global_id(0);
  springStates[i].constants = springs[i];
}

__kernel void inputFluids(
  __global Fluid* hostFluids,
  __global Fluid* fluids
) {
  size_t i = get_global_id(0);
  fluids[i] = hostFluids[i];
}
