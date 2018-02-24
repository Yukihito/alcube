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
}

__kernel void inputSoftBodyStates(
  __global const SoftBodyState* hostSoftBodyStates,
  __global SoftBodyState* softBodyStates
) {
  size_t i = get_global_id(0);
  softBodyStates[i] = hostSoftBodyStates[i];
}

__kernel void inputSprings(
  __global const Spring* springs,
  __global SpringState* springStates
) {
  size_t i = get_global_id(0);
  springStates[i].constants = springs[i];
}

__kernel void initGridAndActorRelations(
  __global GridAndActorRelation* relations,
  const uint gridIndex,
  const unsigned short actorIndex
) {
  size_t i = get_global_id(0);
  relations[i].gridIndex = gridIndex;
  relations[i].actorIndex = actorIndex;
}

__kernel void fillGridIndex(
  __global Constants* constants,
  __global PhysicalQuantity* physicalQuantities,
  __global GridAndActorRelation* relations
) {
  __global Grid* grid = &constants->grid;
  size_t actorIndex = get_global_id(0);
  float edgeLength = grid->edgeLength;
  float3 position = physicalQuantities[actorIndex].position;
  float3 positionGridSpace = position - grid->origin;
  uint3 gridCorner0 = (uint3)(0);
  uint3 gridCorner1 = (uint3)(grid->xCount - 1, grid->yCount - 1, grid->zCount - 1);
  uint3 p = clamp(convert_uint3(positionGridSpace / edgeLength), gridCorner0, gridCorner1);
  uint gridIndex = (uint)p.x + (uint)p.y * grid->xCount + (uint)p.z * grid->xCount * grid->yCount;
  relations[actorIndex].actorIndex = actorIndex;
  relations[actorIndex].gridIndex = gridIndex;
}

__kernel void merge(
  __global GridAndActorRelation* relations,
  const uint distance
) {
  uint index = get_global_id(0);
  if (index % (distance << 1) < distance) {
    GridAndActorRelation left = relations[index];
    GridAndActorRelation right = relations[index + distance];
    bool b = left.gridIndex < right.gridIndex;
    relations[index] = b ? left : right;
    relations[index + distance] = b ? right : left;
  }
}

__kernel void bitonic(
  __global GridAndActorRelation* relations,
  const uint distance,
  const uint stageDistance
) {
  uint index = get_global_id(0);
  if (index % (distance << 1) < distance) {
    uint middleDistance = stageDistance << 1; // * 2
    GridAndActorRelation left = relations[index];
    GridAndActorRelation right = relations[index + distance];
    bool b1 = left.gridIndex < right.gridIndex;
    bool b2 = index % (middleDistance << 1) >= middleDistance;
    bool b3 = !(b1 ^ b2);
    relations[index] = b3 ? left : right;
    relations[index + distance] = b3 ? right : left;
  }
}

__kernel void setGridRelationIndexRange(
  __global GridAndActorRelation* relations,
  __global uint* gridStartIndices,
  __global uint* gridEndIndices,
  const uint actorCount
) {
  uint current = get_global_id(0);
  uint next = current + 1;
  uint currentGridIndex = relations[current].gridIndex;
  uint nextGridIndex = relations[next].gridIndex;
  if (current == 0) {
    gridStartIndices[currentGridIndex] = current;
  }
  if (currentGridIndex != nextGridIndex) {
    gridEndIndices[currentGridIndex] = next;
    gridStartIndices[nextGridIndex] = next;
  }
  if (next == actorCount - 1) {
    gridEndIndices[nextGridIndex] = next + 1;
  }
}
