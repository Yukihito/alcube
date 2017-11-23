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
  float3 tmpPosition;
  unsigned int springStartIndex;
  unsigned int springEndIndex;
  bool collisionOccurred;
  unsigned short collisionCellIndex;
  float collisionTime;
  unsigned char neighborCellCount;
  unsigned short neighborCellIndices[16];
} Cell;

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
  __global Cell* cells,
  __global RigidBodyState* currentStates,
  __global GridAndCellRelation* relations
) {
  size_t cellIndex = get_global_id(0);
  float edgeLength = grid->edgeLength;
  float3 position = currentStates[cellIndex].position;
  float3 positionGridSpace = position - grid->origin;
  uint3 p = convert_uint3(positionGridSpace / edgeLength);
  uint gridX = clamp(p.x, (uint)0, grid->xCount - 1);
  uint gridY = clamp(p.y, (uint)0, grid->yCount - 1);
  uint gridZ = clamp(p.z, (uint)0, grid->zCount - 1);
  uint gridIndex = (uint)gridX + (uint)gridY * grid->xCount + (uint)gridZ * grid->xCount * grid->yCount;
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
  __global Cell* cells,
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
  float effectiveRadius = radius + 4.0f;

  float3 checkStartPositionGridSpace = position - (float3)(effectiveRadius) - grid->origin;
  uint3 checkStartGrid = convert_uint3(checkStartPositionGridSpace / edgeLength);
  uint gridCheckStartX = clamp(checkStartGrid.x, (uint)0, grid->xCount - 1);
  uint gridCheckStartY = clamp(checkStartGrid.y, (uint)0, grid->yCount - 1);
  uint gridCheckStartZ = clamp(checkStartGrid.z, (uint)0, grid->zCount - 1);

  float3 checkEndPositionGridSpace = position + (float3)(effectiveRadius) - grid->origin;
  uint3 checkEndGrid = convert_uint3(checkEndPositionGridSpace / edgeLength);
  uint gridCheckEndX = clamp(checkEndGrid.x, (uint)0, grid->xCount - 1);
  uint gridCheckEndY = clamp(checkEndGrid.y, (uint)0, grid->yCount - 1);
  uint gridCheckEndZ = clamp(checkEndGrid.z, (uint)0, grid->zCount - 1);

  ushort collisionCellIndex = 0;
  float collisionTime = deltaTime + 1.0f;
  uchar intersectionCount = 0;
  bool collisionOccurred = false;
  bool isFullOfIntersection = false;
  for (uint gridZ = gridCheckStartZ; gridZ <= gridCheckEndZ && !isFullOfIntersection; gridZ++) {
    for (uint gridY = gridCheckStartY; gridY <= gridCheckEndY && !isFullOfIntersection; gridY++) {
      for (uint gridX = gridCheckStartX; gridX <= gridCheckEndX && !isFullOfIntersection; gridX++) {
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
	      cells[cellIndex].neighborCellIndices[intersectionCount] = otherCellIndex;
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
	    cells[cellIndex].neighborCellIndices[intersectionCount] = otherCellIndex;
	    intersectionCount++;
	    isFullOfIntersection = intersectionCount >= 16;
	    continue;
	  }
	  // Not intersecting
	}
      }
    }
  }

  cells[cellIndex].collisionCellIndex = collisionCellIndex;
  cells[cellIndex].collisionOccurred = collisionOccurred;
  cells[cellIndex].collisionTime = collisionTime;
  cells[cellIndex].neighborCellCount = intersectionCount;
}

__kernel void updatePhysicalQuantities(
  __global Cell* cells,
  __global RigidBodyState* currentStates,
  __global RigidBodyState* nextStates,
  const float deltaTime
) {
  size_t cellIndex = get_global_id(0);
  float mass = cells[cellIndex].mass;
  float3 currentLinearMomentum = currentStates[cellIndex].linearMomentum;
  float3 currentVelocity = currentLinearMomentum / mass;
  float3 currentPosition = currentStates[cellIndex].position;
  float timeInterval = cells[cellIndex].collisionOccurred ? cells[cellIndex].collisionTime : deltaTime;
  timeInterval = timeInterval > 0.0f ? timeInterval : 0.0f;
  float3 nextPosition = currentPosition + (currentVelocity * timeInterval);
  float3 nextLinearMomentum = currentLinearMomentum;
  if (cells[cellIndex].collisionOccurred) {
    ushort otherCellIndex = cells[cellIndex].collisionCellIndex;
    float elasticity = cells[cellIndex].elasticity;
    float otherElasticity = cells[otherCellIndex].elasticity;
    float otherMass = cells[otherCellIndex].mass;
    float3 relativeOtherCellPosition = currentStates[otherCellIndex].position - nextPosition;
    float3 otherCurrentVelocity = currentStates[otherCellIndex].linearMomentum / otherMass;
    float3 relativeVelocity = otherCurrentVelocity - currentVelocity;
    float3 direction = normalize(relativeOtherCellPosition);
    float effect = dot(relativeVelocity, direction);
    float3 effectiveVelocity = direction * effect;
    float3 acceleration = effectiveVelocity * (1.0f + (elasticity * otherElasticity)) / (1.0f + (mass / otherMass));
    nextLinearMomentum = currentLinearMomentum + (acceleration / mass);
  }
  cells[cellIndex].tmpPosition = nextPosition;
  nextStates[cellIndex].linearMomentum = nextLinearMomentum;
  nextStates[cellIndex].rotation = currentStates[cellIndex].rotation;
  nextStates[cellIndex].angularMomentum = currentStates[cellIndex].angularMomentum;
}

__kernel void resolveIntersection(
  __global Cell* cells,
  __global RigidBodyState* nextStates
) {
  size_t cellIndex = get_global_id(0);
  uchar neighborCellCount = cells[cellIndex].neighborCellCount;
  float radius = cells[cellIndex].radius;
  float3 position = cells[cellIndex].tmpPosition;
  float3 translation = (float3)(0.0f);
  float3 nextPosition = position;
  uchar intersectionCount = 0;
  for (uchar i = 0; i < neighborCellCount; i++) {
    ushort neighborCellIndex = cells[cellIndex].neighborCellIndices[i];
    float3 otherPosition = cells[neighborCellIndex].tmpPosition;
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
    nextPosition = nextPosition + (translation / intersectionCount);
  }
  nextStates[cellIndex].position = nextPosition;
}
