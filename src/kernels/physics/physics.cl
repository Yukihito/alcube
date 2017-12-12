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
  float dynamicFrictionCoefficient;
  float staticFrictionCoefficient;
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
  float3 currentLinearVelocity = (currentLinearMomentum / mass);
  float3 currentPosition = currentStates[cellIndex].position;
  float3 positionDiff = currentLinearVelocity * timeInterval;

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
  float timeIntervalAfterCollision = deltaTime - timeInterval;

  // Constants
  float radius = cells[cellIndex].radius;
  float mass = cells[cellIndex].mass;
  float momentOfInertia = (2.0f / 5.0f) * mass * radius * radius;
  float elasticity = cells[cellIndex].elasticity;
  float staticFrictionCoefficient = cells[cellIndex].staticFrictionCoefficient;
  float dynamicFrictionCoefficient = cells[cellIndex].dynamicFrictionCoefficient;
  float3 corner = grid->origin + (float3)(radius);
  float3 maxLinearMomentum = (float3)((2.0f * (1.0f / deltaTime)) * mass);

  // Current physical quantities.
  float3 currentLinearMomentum = currentStates[cellIndex].linearMomentum;
  float3 currentAngularMomentum = currentStates[cellIndex].angularMomentum;
  float3 currentLinearVelocity = currentLinearMomentum / mass;
  float3 integralVelocity = (float3)(0.0f, (-9.8f * timeInterval) / 2.0f, 0.0f);
  float3 currentAngularVelocity = currentAngularMomentum / momentOfInertia;
  float3 positionAfterMotion = cellVars[cellIndex].positionAfterMotion;
  float4 rotationAfterMotion = cellVars[cellIndex].rotationAfterMotion;

  // Next physical quantities.
  float3 nextPosition = positionAfterMotion;
  float4 nextRotation = rotationAfterMotion;
  float3 translation = (float3)(0.0f);
  float3 angularAcceleration = (float3)(0.0f);
  float3 linearAcceleration = (float3)(0.0f);
  float* linearAccelerationPtr = &linearAcceleration;

  // Other variables.
  uchar neighborCellCount = cellVars[cellIndex].neighborCellCount;

  // Calc acceleration of gravity
  linearAcceleration.y -= gravity * deltaTime;
  // Resolve intersection.
  // Calc acceleration of collision with other cell.
  if (cellVars[cellIndex].collisionOccurred) {
    ushort otherCellIndex = cellVars[cellIndex].collisionCellIndex;
    if (cellVars[otherCellIndex].collisionOccurred && cellVars[otherCellIndex].collisionCellIndex == cellIndex) {
      float otherElasticity = cells[otherCellIndex].elasticity;
      float otherMass = cells[otherCellIndex].mass;
      float3 otherPositionAfterMotion = cellVars[otherCellIndex].positionAfterMotion;
      float3 relativeOtherCellPosition = otherPositionAfterMotion - positionAfterMotion;
      float3 otherCurrentLinearVelocity = currentStates[otherCellIndex].linearMomentum / otherMass;
      float3 relativeVelocity = otherCurrentLinearVelocity - currentLinearVelocity;
      float3 direction = normalize(relativeOtherCellPosition);
      float effect = dot(relativeVelocity, direction);
      float3 effectiveVelocity = direction * effect;
      linearAcceleration += effectiveVelocity * (1.0f + (elasticity * otherElasticity)) / (1.0f + (mass / otherMass));
    }
  }
  float3 linearMomentumAfterCollision = currentLinearMomentum + linearAcceleration * mass;

  float intersectionCount = 0;
  for (uchar i = 0; i < neighborCellCount; i++) {
    ushort neighborCellIndex = cellVars[cellIndex].neighborCellIndices[i];
    float3 otherPositionAfterMotion = cellVars[neighborCellIndex].positionAfterMotion;
    float r = radius + cells[neighborCellIndex].radius;
    float3 relativePosition = otherPositionAfterMotion - positionAfterMotion;
    float dist = length(relativePosition);
    float intersectionLength = r - dist;
    if (intersectionLength >= 0.0f) {
      //linearAcceleration += normalize(relativePosition) * (-intersectionLength / 2.0f) * 2.0f;
      translation += (relativePosition / dist) * (-intersectionLength / 2.0f);
      intersectionCount++;
    }
  }


  if (intersectionCount > 0.0f) {
    nextPosition += translation / intersectionCount;
  }

  // Calc reaction of intersected cells.



  if (length(translation) > 0.0f && length(linearMomentumAfterCollision) > 0.0f && dot(translation, linearMomentumAfterCollision) < 0.0f) {
    float3 reactionDirection = normalize(translation);
    float effect = -dot(reactionDirection, linearMomentumAfterCollision) * 0.5f;
    linearAcceleration += (reactionDirection * effect) / mass;
  }

  linearMomentumAfterCollision = currentLinearMomentum + linearAcceleration * mass;
  float3 angularMomentumAfterCollision = currentAngularMomentum + angularAcceleration * momentOfInertia;
  float3 linearVelocityAfterCollision = linearMomentumAfterCollision / mass;
  float3 angularVelocityAfterCollision = angularMomentumAfterCollision / momentOfInertia;
  float* linearMomentumAfterCollisionPtr = &linearMomentumAfterCollision;

  float3 positionDiff = linearVelocityAfterCollision * timeIntervalAfterCollision;
  float3 rotationDiff = angularVelocityAfterCollision * timeIntervalAfterCollision;
  float4 rotationDiffQuat = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
  if (dot(rotationDiff, rotationDiff) > 0.0f) {
    float3 rotationAxis = normalize(rotationDiff);
    float rotationScalar = length(rotationDiff);
    float halfRotationScalar = rotationScalar / 2.0f;
    rotationDiffQuat.w = cos(halfRotationScalar);
    rotationDiffQuat.xyz = rotationAxis * sin(halfRotationScalar);
  }
  nextPosition = nextPosition + positionDiff;
  nextRotation = mulQuat(&rotationDiffQuat, &nextRotation);

  // Finish to resolve intersection with other cells.

  // Calc effects of collision with walls.
  float3 nextPositionBeforeClamp = nextPosition;
  nextPosition = clamp(nextPosition, corner, -corner);
  float3 translationByWalls = nextPosition - nextPositionBeforeClamp;
  float* translationByWallsPtr = &translationByWalls;

  for (uchar i = 0; i < 3; i++) {
    float translationByWallScalar = translationByWallsPtr[i];
    if (translationByWallScalar * linearMomentumAfterCollisionPtr[i] < 0.0f) {
      float3 wallNormal = (float3)(0.0f);
      float* wallNormalPtr = &wallNormal;
      wallNormalPtr[i] = translationByWallScalar > 0.0f ? 1.0f : -1.0f;

      float normalReaction = -linearMomentumAfterCollisionPtr[i] * 2.0f * elasticity;
      linearAccelerationPtr[i] += normalReaction / mass;
      float3 workingPoint = -radius * wallNormal;

      float3 linearVelocityOnWall = linearVelocityAfterCollision - (wallNormal * dot(wallNormal, linearVelocityAfterCollision));
      float3 frictionalImpulse = -(2.0f / 7.0f) * mass * (linearVelocityOnWall + cross(-workingPoint, angularVelocityAfterCollision));
      float maxStaticFrictionalImpulse = fabs(normalReaction * staticFrictionCoefficient);
      if (maxStaticFrictionalImpulse < length(frictionalImpulse)) {
	frictionalImpulse = normalize(frictionalImpulse) * dynamicFrictionCoefficient * fabs(normalReaction);
      }
      if (dot(frictionalImpulse, frictionalImpulse) > 0.0f) {
	angularAcceleration += cross(workingPoint, frictionalImpulse / momentOfInertia);
	linearAcceleration += frictionalImpulse / mass;
      }
    }
  }

  // Apply acceleration.
  float3 nextLinearMomentum = currentLinearMomentum + linearAcceleration * mass;
  float3 nextAngularMomentum = currentAngularMomentum + angularAcceleration * momentOfInertia;

  nextLinearMomentum = clamp(nextLinearMomentum, -maxLinearMomentum, maxLinearMomentum);
  float yVelocity = nextLinearMomentum.y / mass;
  if (yVelocity > 0.0f && yVelocity <= gravity * deltaTime * 2.0f) {
    nextLinearMomentum.y = 0.0f;
  }

  nextStates[cellIndex].linearMomentum = nextLinearMomentum;
  nextStates[cellIndex].angularMomentum = nextAngularMomentum;
  nextStates[cellIndex].position = nextPosition;
  nextStates[cellIndex].rotation = nextRotation;
}
