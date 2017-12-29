typedef struct __attribute__ ((packed)) GridStruct {
  unsigned int xCount;
  unsigned int yCount;
  unsigned int zCount;
  unsigned int edgeLength;
  float3 origin;
  float3 normals[6];
} Grid;

typedef struct __attribute__ ((packed)) GridAndCellRelationStruct {
  unsigned int gridIndex;
  unsigned short cellIndex;
  short padding;
} GridAndCellRelation;

typedef struct __attribute__ ((packed)) RigidBodyStateStruct {
  float3 position;
  float4 rotation;
  float3 linearMomentum;
  float3 angularMomentum;
  unsigned int gridIndex;
} RigidBodyState;

typedef struct __attribute__ ((packed)) CellStruct {
  float radius;
  float mass;
  float elasticity;
  float dynamicFrictionCoefficient;
  float staticFrictionCoefficient;
} Cell;

typedef struct __attribute__ ((packed)) IntersectionStruct {
  unsigned short type;
  unsigned short otherIndex;
  float length;
  float speed;
  float3 normal;
} Intersection;

typedef struct __attribute__ ((packed)) CellVarStruct {
  float3 linearVelocity;
  float3 angularVelocity;
  float momentOfInertia;
  float massForIntersection;
  float massForCollision;
  int isFloating;
  unsigned short intersectionCount;
  unsigned short collisionCount;
  unsigned char collisionIndices[16];
  Intersection intersections[16];
} CellVar;

float4 mulQuat(float4* q, float4* r);
float3 rotateByQuat(float3* v, float4* q);

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

float4 mulQuat(
  float4* q,
  float4* r
) {
  float3 u = (float3)(q->x, q->y, q->z);
  float3 v = (float3)(r->x, r->y, r->z);
  float3 ipart = v * q->w + u * r->w + cross(u, v);
  float tpart = q->w * r->w - dot(u, v);
  return (float4)(ipart.x, ipart.y, ipart.z, tpart);
}

float3 rotateByQuat(
  float3* v,
  float4* q
) {
  float4 p = (float4)(v->x, v->y, v->z, 0.0f);
  float4 r = (float4)(-q->x, -q->y, -q->z, q->w);
  float4 qp = mulQuat(q, &p);
  float4 qpr = mulQuat(&qp, &r);
  return (float3)(qpr.x, qpr.y, qpr.z);
}

__kernel void initGridAndCellRelations(
  __global GridAndCellRelation* relations,
  const uint gridIndex,
  const unsigned short cellIndex
) {
  size_t i = get_global_id(0);
  relations[i].gridIndex = gridIndex;
  relations[i].cellIndex = cellIndex;
}

__kernel void fillGridIndex(
  __global const Grid* grid,
  __global const Cell* cells,
  __global RigidBodyState* currentStates,
  __global GridAndCellRelation* relations
) {
  size_t cellIndex = get_global_id(0);
  float edgeLength = grid->edgeLength;
  float3 position = currentStates[cellIndex].position;
  float3 positionGridSpace = position - grid->origin;
  uint3 gridCorner0 = (uint3)(0);
  uint3 gridCorner1 = (uint3)(grid->xCount - 1, grid->yCount - 1, grid->zCount - 1);
  uint3 p = clamp(convert_uint3(positionGridSpace / edgeLength), gridCorner0, gridCorner1);
  uint gridIndex = (uint)p.x + (uint)p.y * grid->xCount + (uint)p.z * grid->xCount * grid->yCount;
  currentStates[cellIndex].gridIndex = gridIndex;
  relations[cellIndex].cellIndex = cellIndex;
  relations[cellIndex].gridIndex = gridIndex;
}

__kernel void merge(
  __global GridAndCellRelation* relations,
  const uint distance
) {
  uint index = get_global_id(0);
  if (index % (distance << 1) < distance) {
    GridAndCellRelation left = relations[index];
    GridAndCellRelation right = relations[index + distance];
    bool b = left.gridIndex < right.gridIndex;
    relations[index] = b ? left : right;
    relations[index + distance] = b ? right : left;
  }
}

__kernel void bitonic(
  __global GridAndCellRelation* relations,
  const uint distance,
  const uint stageDistance
) {
  uint index = get_global_id(0);
  if (index % (distance << 1) < distance) {
    uint middleDistance = stageDistance << 1; // * 2
    GridAndCellRelation left = relations[index];
    GridAndCellRelation right = relations[index + distance];
    bool b1 = left.gridIndex < right.gridIndex;
    bool b2 = index % (middleDistance << 1) >= middleDistance;
    bool b3 = !(b1 ^ b2);
    relations[index] = b3 ? left : right;
    relations[index + distance] = b3 ? right : left;
  }
}

__kernel void setGridRelationIndexRange(
  __global GridAndCellRelation* relations,
  __global uint* gridStartIndices,
  __global uint* gridEndIndices,
  const uint cellCount
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
  if (next == cellCount - 1) {
    gridEndIndices[nextGridIndex] = next + 1;
  }
}

__kernel void collectIntersections(
  __global const Grid* grid,
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global RigidBodyState* currentStates,
  __global GridAndCellRelation* relations,
  __global uint* gridStartIndices,
  __global uint* gridEndIndices,
  const float deltaTime,
  const float gravityAcceleration
) {
  size_t cellIndex = get_global_id(0);
  float edgeLength = (float)grid->edgeLength;
  __global RigidBodyState* currentState = &currentStates[cellIndex];
  __global CellVar* cellVar = &cellVars[cellIndex];
  __global const Cell* cell = &cells[cellIndex];
  float3 position = currentState->position;
  float* positionPtr = (float*)&position;
  float radius = cell->radius;
  float mass = cell->mass;
  float smallValue = 0.0001f;
  bool isFullOfIntersection = false;
  float effectiveRadius = radius + 3.0f;
  float3 checkStartPositionGridSpace = position - (float3)(effectiveRadius) - grid->origin;
  float3 checkEndPositionGridSpace = position + (float3)(effectiveRadius) - grid->origin;
  uint3 gridCorner0 = (uint3)(0);
  uint3 gridCorner1 = (uint3)(grid->xCount - 1, grid->yCount - 1, grid->zCount - 1);
  bool isFloating = true;
  uint3 checkStartGrid = clamp(convert_uint3(checkStartPositionGridSpace / edgeLength), gridCorner0, gridCorner1);
  uint3 checkEndGrid = clamp(convert_uint3(checkEndPositionGridSpace / edgeLength), gridCorner0, gridCorner1);
  uchar intersectionCount = 0;
  for (uint gridZ = checkStartGrid.z; gridZ <= checkEndGrid.z && !isFullOfIntersection; gridZ++) {
    for (uint gridY = checkStartGrid.y; gridY <= checkEndGrid.y && !isFullOfIntersection; gridY++) {
      for (uint gridX = checkStartGrid.x; gridX <=checkEndGrid.x && !isFullOfIntersection; gridX++) {
	uint gridIndex = gridX + gridY * grid->xCount + gridZ * grid->xCount * grid->yCount;
	uint checkStartIndex = gridStartIndices[gridIndex];
	uint checkEndIndex = gridEndIndices[gridIndex];
	for (uint i = checkStartIndex; i < checkEndIndex && !isFullOfIntersection; i++) {
	  ushort otherCellIndex = relations[i].cellIndex;
	  if (otherCellIndex == cellIndex) {
	    continue;
	  }
	  float3 w = currentStates[otherCellIndex].position - position;
	  float r = radius + cells[otherCellIndex].radius;
	  float rr = r * r;
	  float ww = dot(w, w);
	  if (ww > 0.0f && ww <= rr + smallValue) {
	    __global Intersection* intersection = &cellVar->intersections[intersectionCount];
	    intersection->type = 0;
	    intersection->otherIndex = otherCellIndex;
	    intersection->normal = normalize(w);
	    intersection->length = r - length(w);
	    intersection->speed = 0.0f;
	    intersectionCount++;
	    isFullOfIntersection = intersectionCount >= 16;
	  }
	}
      }
    }
  }

  float3 corner = grid->origin + (float3)(radius + smallValue);
  float* cornerPtr = (float*)&corner;
  for (uint i = 0; i < 3 && !isFullOfIntersection; i++) {
    if (positionPtr[i] <= cornerPtr[i]) {
      __global Intersection* intersection = &cellVar->intersections[intersectionCount];
      intersection->type = 1;
      intersection->otherIndex = i;
      intersection->normal = -grid->normals[i];
      intersection->length = cornerPtr[i] - positionPtr[i];
      intersection->speed = 0.0f;
      intersectionCount++;
      isFullOfIntersection = intersectionCount >= 16;
      if (i == 1) {
	isFloating = false;
      }
    }
  }
  for (uint i = 3; i < 6 && !isFullOfIntersection; i++) {
    if (positionPtr[i - 3] >= -cornerPtr[i - 3]) {
      __global Intersection* intersection = &cellVar->intersections[intersectionCount];
      intersection->type = 1;
      intersection->otherIndex = i;
      intersection->normal = -grid->normals[i];
      intersection->length = positionPtr[i - 3] + cornerPtr[i - 3];
      intersection->speed = 0.0f;
      intersectionCount++;
      isFullOfIntersection = intersectionCount >= 16;
    }
  }

  float momentOfInertia = (2.0f / 5.0f) * mass * radius * radius;
  cellVar->momentOfInertia = momentOfInertia;
  float gravityTranslation = gravityAcceleration * deltaTime;
  float gravity = isFloating ? -gravityTranslation : 0.0f;
  cellVar->linearVelocity = currentState->linearMomentum / mass + (float3)(0.0f, gravity, 0.0f);
  float ySpeed = cellVar->linearVelocity.y;
  if (!isFloating && ySpeed * ySpeed < gravityTranslation * gravityTranslation * 16.0f) {
    cellVar->linearVelocity.y = 0.0f;
  }
  cellVar->angularVelocity = currentState->angularMomentum / momentOfInertia;
  cellVar->isFloating = isFloating ? 1 : 0;

  cellVar->intersectionCount = intersectionCount;
  cellVar->collisionCount = 0;

  cellVar->massForIntersection = mass / intersectionCount;
  cellVar->massForCollision = mass;
}

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
    if (intersections[i].speed < 0.0f) {
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
    accumulateConstraintImpulse(cell, cellVar, cells, cellVars, &intersections[cellVar->collisionIndices[i]], &impulse);
  }
  cellVar->linearVelocity = impulse / cell->mass;
}

__kernel void motion(
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global RigidBodyState* currentStates,
  __global RigidBodyState* nextStates,
  const float deltaTime
) {
  size_t cellIndex = get_global_id(0);
  float mass = cells[cellIndex].mass;
  float momentOfInertia = cellVars[cellIndex].momentOfInertia;
  float3 maxLinearMomentum = (float3)((2.0f * (1.0f / deltaTime)) * mass);

  float3 linearVelocity = cellVars[cellIndex].linearVelocity;
  float3 linearMomentum = clamp(linearVelocity * mass, -maxLinearMomentum, maxLinearMomentum);
  float3 currentPosition = currentStates[cellIndex].position;
  float3 positionDiff = linearVelocity * deltaTime;

  float3 angularVelocity = cellVars[cellIndex].angularVelocity;
  float3 angularMomentum = angularVelocity * momentOfInertia;
  float4 currentRotation = currentStates[cellIndex].rotation;
  float3 rotationDiff = angularVelocity * deltaTime;
  float4 rotationDiffQuat = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
  if (dot(rotationDiff, rotationDiff) > 0.0f) {
    float3 rotationAxis = normalize(rotationDiff);
    float rotationScalar = length(rotationDiff);
    float halfRotationScalar = rotationScalar / 2.0f;
    rotationDiffQuat.w = cos(halfRotationScalar);
    rotationDiffQuat.xyz = rotationAxis * sin(halfRotationScalar);
  }

  float3 nextPosition = currentPosition + positionDiff;
  float4 nextRotation = mulQuat(&rotationDiffQuat, &currentRotation);

  nextStates[cellIndex].linearMomentum = linearMomentum;
  nextStates[cellIndex].angularMomentum = angularMomentum;
  nextStates[cellIndex].position = nextPosition;
  nextStates[cellIndex].rotation = nextRotation;
}

__kernel void postProcessing(
  __global const Grid* grid,
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global RigidBodyState* nextStates
) {
  size_t cellIndex = get_global_id(0);
  float3 corner = grid->origin + (float3)(0.0001f);
  float3 nextPosition = nextStates[cellIndex].position;

  nextStates[cellIndex].linearMomentum *= 0.999f;
  nextStates[cellIndex].angularMomentum *= 0.999f;
  if (length(nextStates[cellIndex].linearMomentum) < 0.01f) {
    nextStates[cellIndex].linearMomentum = (float3)(0.0f);
  }

  if (length(nextStates[cellIndex].angularMomentum) < 0.01f) {
    nextStates[cellIndex].angularMomentum = (float3)(0.0f);
  }

  nextStates[cellIndex].position = clamp(nextPosition, corner, -corner);
}
