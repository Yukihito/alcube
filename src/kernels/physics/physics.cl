typedef struct __attribute__ ((packed)) GridStruct {
  unsigned int xCount;
  unsigned int yCount;
  unsigned int zCount;
  unsigned int edgeLength;
  float3 origin;
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
} Cell;

typedef struct __attribute__ ((packed)) CellVarStruct {
  float3 tmpPosition;
  unsigned char collisionOccurred;
  unsigned char wallCollisionOccurred;
  unsigned short collisionCellIndex;
  float collisionTime;
  unsigned char collisionType;
  unsigned char collisionWallAxis;
  unsigned char neighborCellCount;
  char collisionWallNormalSign;
  unsigned short neighborCellIndices[16];
} CellVar;

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

__kernel void collectCollisionAndIntersections(
  __global const Grid* grid,
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global RigidBodyState* currentStates,
  __global GridAndCellRelation* relations,
  __global uint* gridStartIndices,
  __global uint* gridEndIndices,
  const float deltaTime
) {
  size_t cellIndex = get_global_id(0);
  float edgeLength = (float)grid->edgeLength;

  float3 position = currentStates[cellIndex].position;
  float radius = cells[cellIndex].radius;
  float3 velocity = currentStates[cellIndex].linearMomentum / cells[cellIndex].mass;

  float collisionTime = deltaTime + 1.0f;
  bool collisionOccurred = false;
  bool wallCollisionOccurred = false;

  float3 walls = grid->origin + (float3)(radius);
  float3 wallDist1 = (walls - position);
  float3 wallDist2 = (-walls - position);
  float* wallDistPtr1 = &wallDist1;
  float* wallDistPtr2 = &wallDist2;
  float* velocityPtr = &velocity;
  uchar collisionWallAxis;
  float minWallCollisionTime = deltaTime + 1.0f;
  char collisionWallNormalSign;
  for (uchar i = 0; i < 3; i++) {
    if (velocityPtr[i] == 0.0f) {
      continue;
    }
    float t1 = wallDistPtr1[i] / velocityPtr[i];
    float t2 = wallDistPtr2[i] / velocityPtr[i];
    bool intersects1 = deltaTime >= t1 && t1 >= 0;
    bool intersects2 = deltaTime >= t2 && t2 >= 0;
    float wallCollisionTime = minWallCollisionTime;
    if (intersects1 || intersects2) {
      wallCollisionTime = intersects1 && wallCollisionTime > t1 ? t1 : wallCollisionTime;
      wallCollisionTime = intersects2 && wallCollisionTime > t2 ? t2 : wallCollisionTime;
      bool isNearestIntersection = wallCollisionTime < minWallCollisionTime;
      collisionWallAxis = isNearestIntersection ? i : collisionWallAxis;
      char sign = intersects1 ? 1.0f : -1.0f;
      collisionWallNormalSign = isNearestIntersection ? sign : collisionWallNormalSign;
      minWallCollisionTime = wallCollisionTime;
      wallCollisionOccurred = true;
    }
  }

  float effectiveRadius = radius + 4.0f;
  float3 checkStartPositionGridSpace = position - (float3)(effectiveRadius) - grid->origin;
  float3 checkEndPositionGridSpace = position + (float3)(effectiveRadius) - grid->origin;
  uint3 gridCorner0 = (uint3)(0);
  uint3 gridCorner1 = (uint3)(grid->xCount - 1, grid->yCount - 1, grid->zCount - 1);
  uint3 checkStartGrid = clamp(convert_uint3(checkStartPositionGridSpace / edgeLength), gridCorner0, gridCorner1);
  uint3 checkEndGrid = clamp(convert_uint3(checkEndPositionGridSpace / edgeLength), gridCorner0, gridCorner1);

  ushort collisionCellIndex = 0;
  uchar intersectionCount = 0;
  bool isFullOfIntersection = false;
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

	  float3 otherVelocity = currentStates[otherCellIndex].linearMomentum / cells[otherCellIndex].mass;
	  float3 w = currentStates[otherCellIndex].position - position;
	  float3 v = otherVelocity - velocity;
	  float ww = dot(w, w);
	  float r = radius + cells[otherCellIndex].radius;
	  float rr = r * r;
	  float vv = dot(v, v);
	  if (vv == 0.0f) {
	    // Relative velocity is 0
	    if (ww < rr) {
	      // Intersect
	      cellVars[cellIndex].neighborCellIndices[intersectionCount] = otherCellIndex;
	      intersectionCount++;
	      isFullOfIntersection = intersectionCount >= 16;
	    }
	    continue;
	  }
	  float wv = dot(w, v);
	  float root = wv * wv - vv * (ww - rr);
	  if (root < 0.0f) {
	    // Not intersecting
	    continue;
	  }
	  float sqrted = sqrt(root);

	  float intersectionStartTime = (-wv - sqrted) / vv;
	  float intersectionEndTime = (-wv + sqrted) / vv;

	  bool collisionOccursUntilDeltaTime = deltaTime >= intersectionStartTime && intersectionStartTime > 0.0f;
	  bool collisionHasAlreadyOccurredAndConflictDoesNotResolveByDeltaTime = intersectionEndTime >= deltaTime && 0.0f > intersectionStartTime;
	  if (collisionOccursUntilDeltaTime || collisionHasAlreadyOccurredAndConflictDoesNotResolveByDeltaTime) {
	    // Intersect
	    if (wv < 0 && collisionTime > intersectionStartTime) {
	      // Collision
	      collisionCellIndex = otherCellIndex;
	      collisionTime = intersectionStartTime;
	      collisionOccurred = true;
	    }
	    cellVars[cellIndex].neighborCellIndices[intersectionCount] = otherCellIndex;
	    intersectionCount++;
	    isFullOfIntersection = intersectionCount >= 16;
	    continue;
	  }
	  // Not intersecting
	}
      }
    }
  }

  cellVars[cellIndex].collisionCellIndex = collisionCellIndex;
  cellVars[cellIndex].collisionOccurred = collisionOccurred;
  cellVars[cellIndex].wallCollisionOccurred = wallCollisionOccurred;
  cellVars[cellIndex].collisionTime = collisionTime;
  cellVars[cellIndex].neighborCellCount = intersectionCount;
  cellVars[cellIndex].collisionWallAxis = collisionWallAxis;
  cellVars[cellIndex].collisionWallNormalSign = collisionWallNormalSign;
}

__kernel void updatePhysicalQuantities(
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global RigidBodyState* currentStates,
  __global RigidBodyState* nextStates,
  const float deltaTime
) {
  size_t cellIndex = get_global_id(0);
  float mass = cells[cellIndex].mass;
  float3 currentLinearMomentum = currentStates[cellIndex].linearMomentum;
  float3 currentVelocity = currentLinearMomentum / mass;
  float3 currentPosition = currentStates[cellIndex].position;
  float timeInterval = cellVars[cellIndex].collisionOccurred ? cellVars[cellIndex].collisionTime : deltaTime;
  timeInterval = timeInterval > 0.0f ? timeInterval : 0.0f;
  float3 nextPosition = currentPosition + (currentVelocity * timeInterval);
  float3 nextLinearMomentum = currentLinearMomentum;

  if (cellVars[cellIndex].collisionOccurred) {
    ushort otherCellIndex = cellVars[cellIndex].collisionCellIndex;
    if (cellVars[otherCellIndex].collisionOccurred && cellVars[otherCellIndex].collisionCellIndex == cellIndex) {
      float elasticity = cells[cellIndex].elasticity;
      float otherElasticity = cells[otherCellIndex].elasticity;
      float otherMass = cells[otherCellIndex].mass;
      float3 relativeOtherCellPosition = currentStates[otherCellIndex].position - currentPosition;
      float3 otherCurrentVelocity = currentStates[otherCellIndex].linearMomentum / otherMass;
      float3 relativeVelocity = otherCurrentVelocity - currentVelocity;
      float3 direction = normalize(relativeOtherCellPosition);
      float effect = dot(relativeVelocity, direction);
      float3 effectiveVelocity = direction * effect;
      float3 acceleration = effectiveVelocity * (1.0f + (elasticity * otherElasticity)) / (1.0f + (mass / otherMass));
      nextLinearMomentum = currentLinearMomentum + (acceleration * mass);
    }
  }

  if (cellVars[cellIndex].wallCollisionOccurred) {
    float elasticity = cells[cellIndex].elasticity;
    char collisionWallNormalSign = cellVars[cellIndex].collisionWallNormalSign;
    float* nextLinearMomentumPtr = &nextLinearMomentum;
    if (nextLinearMomentumPtr[cellVars[cellIndex].collisionWallAxis] * collisionWallNormalSign < 0) {
      nextLinearMomentumPtr[cellVars[cellIndex].collisionWallAxis] *= -elasticity;
    }
  }
  nextLinearMomentum.y -= 9.8f * deltaTime;

  float3 maxLinearMomentum = (float3)((2.0f * (1.0f / deltaTime)) * mass);
  nextLinearMomentum = clamp(nextLinearMomentum, -maxLinearMomentum, maxLinearMomentum);

  cellVars[cellIndex].tmpPosition = nextPosition;
  nextStates[cellIndex].linearMomentum = nextLinearMomentum;
  nextStates[cellIndex].rotation = currentStates[cellIndex].rotation;
  nextStates[cellIndex].angularMomentum = currentStates[cellIndex].angularMomentum;
}

__kernel void resolveIntersection(
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global RigidBodyState* nextStates,
  __global const Grid* grid
) {
  size_t cellIndex = get_global_id(0);
  uchar neighborCellCount = cellVars[cellIndex].neighborCellCount;
  float radius = cells[cellIndex].radius;
  float3 position = cellVars[cellIndex].tmpPosition;
  float3 translation = (float3)(0.0f);
  float3 nextPosition = position;
  uchar intersectionCount = 0;

  for (uchar i = 0; i < neighborCellCount; i++) {
    ushort neighborCellIndex = cellVars[cellIndex].neighborCellIndices[i];
    float3 otherPosition = cellVars[neighborCellIndex].tmpPosition;
    float r = radius + cells[neighborCellIndex].radius;
    float3 relativePosition = otherPosition - position;
    float dist = length(relativePosition);
    float intersectionLength = r - dist;
    if (intersectionLength > 0.0f) {
      translation = translation +  (relativePosition / dist) * (-intersectionLength / 2.0f);
      intersectionCount++;
    }
  }

  if (intersectionCount > 0) {
    translation /= intersectionCount;
    nextPosition += translation;
  }

  float3 corner = grid->origin + (float3)(radius);
  nextPosition = clamp(nextPosition, corner, -corner);
  float3 reaction = (nextPosition - position) * cells[cellIndex].mass;
  nextStates[cellIndex].linearMomentum += reaction;
  nextStates[cellIndex].position = nextPosition;
}
