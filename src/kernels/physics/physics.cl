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
  float frictionFactor;
} Cell;

typedef struct __attribute__ ((packed)) CellVarStruct {
  float3 positionAfterMotion;
  float4 rotationAfterMotion;
  float collisionTime;
  unsigned short collisionCellIndex;
  unsigned char collisionOccurred;
  unsigned char neighborCellCount;
  unsigned short neighborCellIndices[16];
} CellVar;

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
  cellVars[cellIndex].collisionTime = collisionTime;
  cellVars[cellIndex].neighborCellCount = intersectionCount;
}

__kernel void motion(
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global RigidBodyState* currentStates,
  __global RigidBodyState* nextStates,
  const float deltaTime
) {
  size_t cellIndex = get_global_id(0);
  float timeInterval = cellVars[cellIndex].collisionOccurred ? cellVars[cellIndex].collisionTime : deltaTime;
  timeInterval = timeInterval > 0.0f ? timeInterval : 0.0f;

  float mass = cells[cellIndex].mass;
  float radius = cells[cellIndex].radius;
  float3 currentLinearMomentum = currentStates[cellIndex].linearMomentum;
  float3 currentVelocity = currentLinearMomentum / mass;
  float3 currentPosition = currentStates[cellIndex].position;
  float3 positionDiff = currentVelocity * timeInterval;

  float3 currentAngularMomentum = currentStates[cellIndex].angularMomentum;
  float momentOfInertia = (2.0f / 5.0f) * mass * radius * radius;
  float3 currentAngularVelocity = currentAngularMomentum / momentOfInertia;
  float4 currentRotation = currentStates[cellIndex].rotation;
  float3 rotationDiff = currentAngularVelocity * timeInterval;
  float4 rotationDiffQuat = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
  if (dot(rotationDiff, rotationDiff) > 0.0f) {
    float3 rotationAxis = normalize(rotationDiff);
    float rotationScalar = length(rotationDiff);
    float halfRotationScalar = rotationScalar / 2.0f;
    rotationDiffQuat.w = cos(halfRotationScalar);
    rotationDiffQuat.xyz = rotationAxis * sin(halfRotationScalar);
  }
  cellVars[cellIndex].positionAfterMotion = currentPosition + positionDiff;
  cellVars[cellIndex].rotationAfterMotion = mulQuat(&rotationDiffQuat, &currentRotation);
}

__kernel void resolveIntersection(
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global RigidBodyState* currentStates,
  __global RigidBodyState* nextStates,
  __global const Grid* grid,
  const float deltaTime,
  const float gravity
) {
  size_t cellIndex = get_global_id(0);
  float timeInterval = cellVars[cellIndex].collisionOccurred ? cellVars[cellIndex].collisionTime : deltaTime;
  timeInterval = timeInterval > 0.0f ? timeInterval : 0.0f;
  float3 currentLinearMomentum = currentStates[cellIndex].linearMomentum;
  uchar neighborCellCount = cellVars[cellIndex].neighborCellCount;
  float radius = cells[cellIndex].radius;
  float mass = cells[cellIndex].mass;
  float3 positionAfterMotion = cellVars[cellIndex].positionAfterMotion;
  float4 rotationAfterMotion = cellVars[cellIndex].rotationAfterMotion;
  float4 nextRotation = rotationAfterMotion;

  // Resolve intersection.
  float3 translation = (float3)(0.0f);
  for (uchar i = 0; i < neighborCellCount; i++) {
    ushort neighborCellIndex = cellVars[cellIndex].neighborCellIndices[i];
    float3 otherPositionAfterMotion = cellVars[neighborCellIndex].positionAfterMotion;
    float r = radius + cells[neighborCellIndex].radius;
    float3 relativePosition = otherPositionAfterMotion - positionAfterMotion;
    float dist = length(relativePosition);
    float intersectionLength = r - dist;
    if (intersectionLength > 0.0f) {
      translation += (relativePosition / dist) * (-intersectionLength / 2.0f);
    }
  }
  float3 nextPosition = positionAfterMotion + translation;
  // Finish to resolve intersection.

  // Calc reaction.
  // Calc reaction of collision with walls.
  float3 currentAngularMomentum = currentStates[cellIndex].angularMomentum;
  float momentOfInertia = (2.0f / 5.0f) * mass * radius * radius;
  float3 currentAngularVelocity = currentAngularMomentum / momentOfInertia;
  float3 currentVelocity = currentLinearMomentum / mass;
  float3 angularAcceleration = (float3)(0.0f);
  float3 acceleration = (float3)(0.0f);
  float3 reaction = (float3)(0.0f, 0.0f, 0.0f);
  float3 corner = grid->origin + (float3)(radius);
  float3 nextPositionBeforeClamp = nextPosition;
  nextPosition = clamp(nextPosition, corner, -corner);
  float3 translationByWalls = nextPosition - nextPositionBeforeClamp;
  float* translationByWallsPtr = &translationByWalls;
  float* reactionPtr = &reaction;
  float* linearMomentumPtr = &currentLinearMomentum;
  float elasticity = cells[cellIndex].elasticity;
  for (uchar i = 0; i < 3; i++) {
    float translationByWallScalar = translationByWallsPtr[i];
    if (translationByWallScalar * linearMomentumPtr[i] < 0.0f) {
      reactionPtr[i] -= linearMomentumPtr[i] * 2.0f * elasticity;
      float3 wallNormal = (float3)(0.0f);
      float3* wallNormalPtr = &wallNormal;
      wallNormalPtr[i] = translationByWallsPtr[i] > 0.0f ? 1.0f : -1.0f;
      float3 workingPoint = (float3)(0.0f);
      float* workingPointPtr = &workingPoint;
      workingPointPtr[i] = translationByWallsPtr[i] > 0.0f ? -radius : radius;
      float3 currentVelocityOnWall = currentVelocity - wallNormal * dot(wallNormal, currentVelocity);
      float3 impulse = -(2.0f / 7.0f) * mass * (currentVelocity + cross(-workingPoint, currentAngularVelocity));
      angularAcceleration += cross(workingPoint, impulse / momentOfInertia);
      acceleration += impulse / mass;
    }
  }
  // Calc reaction of intersected cells.
  if (length(translation) > 0.0f && length(currentLinearMomentum) > 0.0f && dot(translation, currentLinearMomentum) < 0.0f) {
    float3 reactionDirection = normalize(translation);
    float effect = sqrt(-dot(translation, currentLinearMomentum));
    reaction += (reactionDirection * effect) * 2.0f;
  }
  // Finish to calc reaction.



  // Calc acceleration.
  // Calc acceleration of collision with other cell.

  if (cellVars[cellIndex].collisionOccurred) {
    ushort otherCellIndex = cellVars[cellIndex].collisionCellIndex;
    if (cellVars[otherCellIndex].collisionOccurred && cellVars[otherCellIndex].collisionCellIndex == cellIndex) {
      float elasticity = cells[cellIndex].elasticity;
      float otherElasticity = cells[otherCellIndex].elasticity;
      float otherMass = cells[otherCellIndex].mass;
      float3 otherPositionAfterMotion = cellVars[otherCellIndex].positionAfterMotion;
      float3 relativeOtherCellPosition = otherPositionAfterMotion - positionAfterMotion;
      float3 otherCurrentVelocity = currentStates[otherCellIndex].linearMomentum / otherMass;
      float3 relativeVelocity = otherCurrentVelocity - currentVelocity;
      float3 direction = normalize(relativeOtherCellPosition);
      float effect = dot(relativeVelocity, direction);
      float3 effectiveVelocity = direction * effect;
      acceleration += effectiveVelocity * (1.0f + (elasticity * otherElasticity)) / (1.0f + (mass / otherMass));
    }
  }

  // Calc acceleration of gravity
  acceleration.y -= gravity * timeInterval;
  // Finish to calc acceleration.

  // Apply acceleration and reaction.
  float3 nextLinearMomentum = currentLinearMomentum + acceleration * mass + reaction;
  float3 maxLinearMomentum = (float3)((2.0f * (1.0f / deltaTime)) * mass);
  nextLinearMomentum = clamp(nextLinearMomentum, -maxLinearMomentum, maxLinearMomentum);

  nextStates[cellIndex].linearMomentum = nextLinearMomentum;
  nextStates[cellIndex].angularMomentum = currentStates[cellIndex].angularMomentum + angularAcceleration * momentOfInertia;
  nextStates[cellIndex].position = nextPosition;
  nextStates[cellIndex].rotation = nextRotation;
}
