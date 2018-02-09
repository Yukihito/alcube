void accumulatePenaltyImpulse(
  __global Intersection* intersection,
  const float deltaTime,
  float3* acc
);

void accumulateFrictionalImpulse(
  __global const Cell* cell,
  __global CellVar* cellVar,
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global Intersection* intersection,
  float3* linearMomentumAcc,
  float3* angularMomentumAcc
);

void accumulateConstraintImpulse(
  __global const Cell* cell,
  __global CellVar* cellVar,
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global Intersection* intersection,
  float3* acc
);

void accumulatePenaltyImpulse(
  __global Intersection* intersection,
  const float deltaTime,
  float3* acc
) {
  *acc += (-64.0f * log2(intersection->length + 1.0f) * deltaTime) * intersection->normal;
}

__kernel void updateByPenaltyImpulse(
  __global const Cell* cells,
  __global CellVar* cellVars,
  const float deltaTime
) {
  size_t cellIndex = get_global_id(0);
  __global CellVar* cellVar = &cellVars[cellIndex];
  uchar count = cellVar->intersectionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = cellVar->intersections;
  float3 impulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    accumulatePenaltyImpulse(&intersections[i], deltaTime, &impulse);
  }
  cellVar->linearVelocity += impulse / cells[cellIndex].mass;
}

void accumulateFrictionalImpulse(
  __global const Cell* cell,
  __global CellVar* cellVar,
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global Intersection* intersection,
  float3* linearMomentumAcc,
  float3* angularMomentumAcc
) {
  unsigned int intersectionType = intersection->type;
  ushort otherIndex = intersection->otherIndex;
  float mass = cellVar->massForIntersection;
  float s = intersectionType == 0 ? 1.0f / cellVars[otherIndex].massForIntersection : 0;
  float t = (7.0f / 2.0f) * ((1.0f / mass) + s);
  float3 intersectionNormal = intersection->normal;
  float3 otherLinearVelocity = intersectionType == 0 ? cellVars[otherIndex].linearVelocity : (float3)(0.0f);
  float3 relativeVelocity = otherLinearVelocity - cellVar->linearVelocity;
  float3 relativeVelocityOnSurface = relativeVelocity - intersectionNormal * dot(relativeVelocity, intersectionNormal);
  float3 workingPoint = cell->radius * intersectionNormal;
  float3 workingPointVelocity = cross(cellVar->angularVelocity, workingPoint);
  float3 otherWorkingPointVelocity = intersectionType == 0 ? cross(cellVars[otherIndex].angularVelocity, -cells[otherIndex].radius * intersectionNormal) : (float3)(0.0f);
  float3 relativeWorkingPointVelocity = otherWorkingPointVelocity - workingPointVelocity;
  float3 frictionalImpulse = (relativeVelocityOnSurface + relativeWorkingPointVelocity) / t;
  *linearMomentumAcc += frictionalImpulse;
  *angularMomentumAcc += cross(workingPoint, frictionalImpulse);
}

__kernel void updateByFrictionalImpulse(
  __global const Cell* cells,
  __global CellVar* cellVars
) {
  size_t cellIndex = get_global_id(0);
  __global CellVar* cellVar = &cellVars[cellIndex];
  uchar count = cellVar->intersectionCount;
  if (count == 0) {
    return;
  }
  __global const Cell* cell = &cells[cellIndex];
  __global Intersection* intersections = cellVar->intersections;
  float3 impulse = (float3)(0.0f);
  float3 angularImpulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    if (intersections[i].type == 3) {
      continue;
    }
    accumulateFrictionalImpulse(cell, cellVar, cells, cellVars, &intersections[i], &impulse, &angularImpulse);
  }
  cellVar->linearVelocity += impulse / cell->mass;
  cellVar->angularVelocity += angularImpulse / cellVar->momentOfInertia;
}

__kernel void collectCollisions(
  __global const Cell* cells,
  __global CellVar* cellVars
) {
  size_t cellIndex = get_global_id(0);
  __global CellVar* cellVar = &cellVars[cellIndex];
  uchar count = cellVar->intersectionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = cellVar->intersections;
  uchar collisionCount = 0;
  for (uchar i = 0; i < count; i++) {
    float3 relativeSpeed = intersections[i].type == 0 ? cellVars[intersections[i].otherIndex].linearVelocity - cellVar->linearVelocity : - cellVar->linearVelocity;
    intersections[i].speed = dot(intersections[i].normal, relativeSpeed);
    if (intersections[i].type != 3 && intersections[i].speed < 0.0f) {
      cellVar->collisionIndices[collisionCount] = i;
      collisionCount++;
    }
  }
  cellVar->collisionCount = collisionCount;
  cellVar->massForCollision = cells[cellIndex].mass / collisionCount;
}

void accumulateConstraintImpulse(
  __global const Cell* cell,
  __global CellVar* cellVar,
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global Intersection* intersection,
  float3* acc
) {
  if (intersection->speed >= 0.0f) {
    return;
  }
  unsigned int intersectionType = intersection->type;
  ushort otherIndex = intersection->otherIndex;
  float mass = cellVar->massForCollision;
  float elasticity = intersectionType == 0 ? cell->elasticity * cells[otherIndex].elasticity : cell->elasticity;
  float massRatio = intersectionType == 0 ? mass / cellVars[otherIndex].massForCollision : 0.0f;
  float3 intersectionNormal = intersection->normal;
  float speedOnIntersectionNormal = dot(cellVar->linearVelocity, intersectionNormal);
  float affectedSpeed = (intersection->speed * (1.0f + elasticity) / (1.0f + massRatio)) + speedOnIntersectionNormal;
  *acc += (cellVar->linearVelocity + (affectedSpeed - speedOnIntersectionNormal) * intersectionNormal) * mass;
}

__kernel void updateByConstraintImpulse(
  __global const Cell* cells,
  __global CellVar* cellVars
) {
  size_t cellIndex = get_global_id(0);
  __global CellVar* cellVar = &cellVars[cellIndex];
  uchar count = cellVar->collisionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = cellVar->intersections;
  __global const Cell* cell = &cells[cellIndex];

  float3 impulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    if (intersections[cellVar->collisionIndices[i]].type == 3) {
      continue;
    }
    accumulateConstraintImpulse(cell, cellVar, cells, cellVars, &intersections[cellVar->collisionIndices[i]], &impulse);
  }
  cellVar->linearVelocity = impulse / cell->mass;
}
