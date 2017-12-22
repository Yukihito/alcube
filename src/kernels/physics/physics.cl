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

typedef struct __attribute__ ((packed)) CellVarStruct {
  // 32
  float3 linearVelocity; // 4 * 3 = 12
  float3 angularVelocity; // 4 * 3 = 12
  float momentOfInertia; // 4
  float splitMass; // 4

  // 4
  uchar cellIntersectionCount; // 1
  uchar cellCollisionCount; // 1
  uchar planeIntersectionCount; // 1
  uchar planeCollisionCount; // 1

  // 320
  ushort intersectedCellIndices[16]; // 2 * 16 = 32
  ushort collisionCellIndices[16]; // 2 * 16 = 32
  float  cellCollisionSpeeds[16]; // 4 * 16 = 64
  float3 cellIntersectionNormals[16]; // 4 * 3 * 16 = 192

  // 32
  ushort intersectedPlaneIndices[4]; // 2 * 4 = 8
  ushort collisionPlaneIndices[4]; // 2 * 4 = 8
  float  planeCollisionSpeeds[4]; // 4 * 4 = 16

  // 4
  int isFloating; // 4
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

__kernel void collectIntersections(
  __global const Grid* grid,
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global RigidBodyState* currentStates,
  __global GridAndCellRelation* relations,
  __global uint* gridStartIndices,
  __global uint* gridEndIndices
) {
  size_t cellIndex = get_global_id(0);
  float edgeLength = (float)grid->edgeLength;
  float3 position = currentStates[cellIndex].position;
  float* positionPtr = &position;
  float radius = cells[cellIndex].radius;
  float mass = cells[cellIndex].mass;

  uchar cellIntersectionCount = 0;
  bool isFullOfCellIntersection = false;
  float effectiveRadius = radius + 3.0f;
  float3 checkStartPositionGridSpace = position - (float3)(effectiveRadius) - grid->origin;
  float3 checkEndPositionGridSpace = position + (float3)(effectiveRadius) - grid->origin;
  uint3 gridCorner0 = (uint3)(0);
  uint3 gridCorner1 = (uint3)(grid->xCount - 1, grid->yCount - 1, grid->zCount - 1);
  bool isFloating = true;
  uint3 checkStartGrid = clamp(convert_uint3(checkStartPositionGridSpace / edgeLength), gridCorner0, gridCorner1);
  uint3 checkEndGrid = clamp(convert_uint3(checkEndPositionGridSpace / edgeLength), gridCorner0, gridCorner1);
  for (uint gridZ = checkStartGrid.z; gridZ <= checkEndGrid.z && !isFullOfCellIntersection; gridZ++) {
    for (uint gridY = checkStartGrid.y; gridY <= checkEndGrid.y && !isFullOfCellIntersection; gridY++) {
      for (uint gridX = checkStartGrid.x; gridX <=checkEndGrid.x && !isFullOfCellIntersection; gridX++) {
	uint gridIndex = gridX + gridY * grid->xCount + gridZ * grid->xCount * grid->yCount;
	uint checkStartIndex = gridStartIndices[gridIndex];
	uint checkEndIndex = gridEndIndices[gridIndex];
	for (uint i = checkStartIndex; i < checkEndIndex && !isFullOfCellIntersection; i++) {
	  ushort otherCellIndex = relations[i].cellIndex;
	  if (otherCellIndex == cellIndex) {
	    continue;
	  }
	  float3 w = currentStates[otherCellIndex].position - position;
	  float r = radius + cells[otherCellIndex].radius;
	  float rr = r * r;
	  float ww = dot(w, w);
	  if (ww > 0.0f && ww <= rr) {
	    cellVars[cellIndex].intersectedCellIndices[cellIntersectionCount] = otherCellIndex;
	    cellVars[cellIndex].cellIntersectionNormals[cellIntersectionCount] = normalize(w);
	    cellIntersectionCount++;
	    isFullOfCellIntersection = cellIntersectionCount >= 16;
	  }
	}
      }
    }
  }

  float3 corner = grid->origin + (float3)(radius + 0.0001f);
  float* cornerPtr = &corner;
  uchar planeIntersectionCount = 0;
  for (uint i = 0; i < 3; i++) {
    if (positionPtr[i] <= cornerPtr[i]) {
      cellVars[cellIndex].intersectedPlaneIndices[planeIntersectionCount] = i;
      planeIntersectionCount++;
      if (i == 1) {
	isFloating = false;
      }
    }
  }
  for (uint i = 3; i < 6; i++) {
    if (positionPtr[i - 3] >= -cornerPtr[i - 3]) {
      cellVars[cellIndex].intersectedPlaneIndices[planeIntersectionCount] = i;
      planeIntersectionCount++;
    }
  }

  float momentOfInertia = (2.0f / 5.0f) * mass * radius * radius;
  cellVars[cellIndex].cellIntersectionCount = cellIntersectionCount;
  cellVars[cellIndex].planeIntersectionCount = planeIntersectionCount;
  cellVars[cellIndex].momentOfInertia = momentOfInertia;
  float gravityTranslation = 9.8f / 30.0f;
  float gravity = isFloating ? -gravityTranslation : 0.0f;
  cellVars[cellIndex].linearVelocity = currentStates[cellIndex].linearMomentum / mass + (float3)(0.0f, gravity, 0.0f);
  float ySpeed = cellVars[cellIndex].linearVelocity.y;
  if (!isFloating && ySpeed * ySpeed < gravityTranslation * gravityTranslation * 16.0f) {
    cellVars[cellIndex].linearVelocity.y = 0.0f;
  }
  cellVars[cellIndex].angularVelocity = currentStates[cellIndex].angularMomentum / momentOfInertia;
  cellVars[cellIndex].isFloating = isFloating ? 1 : 0;
}

__kernel void applyPenalty(
  __global const Grid* grid,
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global RigidBodyState* currentStates,
  const float deltaTime
) {
  size_t cellIndex = get_global_id(0);
  uchar cellIntersectionCount = cellVars[cellIndex].cellIntersectionCount;
  uchar planeIntersectionCount = cellVars[cellIndex].planeIntersectionCount;
  float mass = cells[cellIndex].mass;
  float radius = cells[cellIndex].radius;
  float3 position = currentStates[cellIndex].position;
  float* positionPtr = &position;
  float3 linearVelocity = cellVars[cellIndex].linearVelocity;
  float3 impulse = (float3)(0.0f);
  float dashPotFactor = 10.0f;
  float springFactor = 256.0f;

  for (uchar i = 0; i < cellIntersectionCount; i++) {
    ushort otherCellIndex = cellVars[cellIndex].intersectedCellIndices[i];
    float3 otherLinearVelocity = cellVars[otherCellIndex].linearVelocity;
    float3 relativeVelocity = otherLinearVelocity - linearVelocity;
    float3 intersectionNormal = cellVars[cellIndex].cellIntersectionNormals[i];
    float collisionSpeed = dot(intersectionNormal, relativeVelocity);
    float dashPotForce = dashPotFactor * collisionSpeed;
    if (dashPotForce > 0.0f) {
      dashPotForce = 0.0f;
    }
    float3 otherPosition = currentStates[otherCellIndex].position;
    float3 relativePosition = otherPosition - position;
    float r = radius + cells[otherCellIndex].radius;

    float dist = length(relativePosition);
    float intersectionLength = r - dist;
    float springForce = springFactor * intersectionLength;
    float impulseScalar = (springForce + dashPotForce) * deltaTime;
    if (impulseScalar > 0.0f) {
      impulse -= intersectionNormal * impulseScalar;
    }
  }

  for (uchar i = 0; i < planeIntersectionCount; i++) {
    ushort planeIndex = cellVars[cellIndex].intersectedPlaneIndices[i];
    ushort elemIndex = planeIndex % 3;
    float3 corner = grid->origin + (float3)(radius);
    float* cornerPtr = &corner;
    float cornerScalar = planeIndex > 2 ? cornerPtr[elemIndex] : -cornerPtr[elemIndex];
    float positionScalar = positionPtr[elemIndex];
    float dist = fabs(cornerScalar - positionScalar);
    float intersectionLength = radius - dist;
    float3 intersectionNormal = -grid->normals[planeIndex];
    float3 relativeVelocity = -linearVelocity;
    float collisionSpeed = dot(intersectionNormal, relativeVelocity);
    float dashPotForce = dashPotFactor * collisionSpeed;
    if (dashPotForce > 0.0f) {
      dashPotForce = 0.0f;
    }
    float springForce = springFactor * intersectionLength;
    float impulseScalar = (springForce + dashPotForce) * deltaTime;
    if (impulseScalar > 0.0f) {
      impulse -= intersectionNormal * impulseScalar;
    }
  }
  cellVars[cellIndex].linearVelocity += impulse / mass;

}

__kernel void collectFrictionalCollisions(
  __global const Grid* grid,
  __global const Cell* cells,
  __global CellVar* cellVars
) {
  size_t cellIndex = get_global_id(0);
  float mass = cells[cellIndex].mass;
  float3 linearVelocity = cellVars[cellIndex].linearVelocity;
  if (!cellVars[cellIndex].isFloating) {
    linearVelocity.y -= 9.8f / 30.0f;
  }
  uchar cellIntersectionCount = cellVars[cellIndex].cellIntersectionCount;
  uchar cellCollisionCount = 0;
  for (uchar i = 0; i < cellIntersectionCount; i++) {
    ushort otherCellIndex = cellVars[cellIndex].intersectedCellIndices[i];
    float3 otherLinearVelocity = cellVars[otherCellIndex].linearVelocity;
    if (!cellVars[otherCellIndex].isFloating) {
      otherLinearVelocity.y -= 9.8f / 30.0f;
    }
    float3 relativeVelocity = otherLinearVelocity - linearVelocity;
    float3 intersectionNormal = cellVars[cellIndex].cellIntersectionNormals[i];
    float collisionSpeed = dot(intersectionNormal, relativeVelocity);
    if (collisionSpeed < 0.0f) {
      cellVars[cellIndex].collisionCellIndices[cellCollisionCount] = i;
      cellVars[cellIndex].cellCollisionSpeeds[cellCollisionCount] = collisionSpeed;
      cellCollisionCount++;
    }
  }

  uchar planeIntersectionCount = cellVars[cellIndex].planeIntersectionCount;
  uchar planeCollisionCount = 0;

  for (uchar i = 0; i < planeIntersectionCount; i++) {
    ushort planeIndex = cellVars[cellIndex].intersectedPlaneIndices[i];
    float3 intersectionNormal = -grid->normals[planeIndex];
    float3 relativeVelocity = -linearVelocity;
    float collisionSpeed = dot(intersectionNormal, relativeVelocity);
    if (collisionSpeed < 0.0f) {
      cellVars[cellIndex].collisionPlaneIndices[planeCollisionCount] = i;
      cellVars[cellIndex].planeCollisionSpeeds[planeCollisionCount] = collisionSpeed;
      planeCollisionCount++;
    }
  }

  uchar allCollisionCount = cellCollisionCount + planeCollisionCount;
  float splitMass = allCollisionCount == 0 ? mass : mass / (float)allCollisionCount;

  cellVars[cellIndex].splitMass = splitMass;
  cellVars[cellIndex].cellCollisionCount = cellCollisionCount;
  cellVars[cellIndex].planeCollisionCount = planeCollisionCount;
}

__kernel void updateVelocityByFriction(
  __global const Grid* grid,
  __global const Cell* cells,
  __global CellVar* cellVars
) {
  size_t cellIndex = get_global_id(0);
  uchar cellCollisionCount = cellVars[cellIndex].cellCollisionCount;
  uchar planeCollisionCount = cellVars[cellIndex].planeCollisionCount;
  if (cellCollisionCount + planeCollisionCount == 0) {
    return;
  }
  float mass = cells[cellIndex].mass;
  float splitMass = cellVars[cellIndex].splitMass;
  float momentOfInertia = cellVars[cellIndex].momentOfInertia;
  float radius = cells[cellIndex].radius;
  float3 linearVelocity = cellVars[cellIndex].linearVelocity;
  float3 angularVelocity = cellVars[cellIndex].angularVelocity;

  if (!cellVars[cellIndex].isFloating) {
    linearVelocity.y -= 9.8f / 30.0f;
  }
  float3 impulseOfFriction = (float3)(0.0f);
  float3 angularImpulseOfFriction = (float3)(0.0f);

  for (uchar i = 0; i < cellCollisionCount; i++) {
    ushort intersectionIndex = cellVars[cellIndex].collisionCellIndices[i];
    ushort otherCellIndex = cellVars[cellIndex].intersectedCellIndices[intersectionIndex];
    float3 otherLinearVelocity = cellVars[otherCellIndex].linearVelocity;
    if (!cellVars[otherCellIndex].isFloating) {
      otherLinearVelocity.y -= 9.8f / 30.0f;
    }
    float3 relativeVelocity = otherLinearVelocity - linearVelocity;
    float3 intersectionNormal = cellVars[cellIndex].cellIntersectionNormals[i];
    float3 relativeVelocityOnSurface = relativeVelocity - intersectionNormal * dot(relativeVelocity, intersectionNormal);

    float otherSplitMass = cellVars[otherCellIndex].splitMass;
    float otherRadius = cells[otherCellIndex].radius;
    float3 otherAngularVelocity = cellVars[otherCellIndex].angularVelocity;
    float3 workingPoint = radius * intersectionNormal;
    float3 otherWorkingPoint = -otherRadius * intersectionNormal;
    float impulseFactor = (2.0f * splitMass * otherSplitMass) / (7.0f * (splitMass + otherSplitMass));
    float3 frictionalImpulse = impulseFactor * (relativeVelocityOnSurface + cross(workingPoint, angularVelocity) - cross(otherWorkingPoint, otherAngularVelocity));
    impulseOfFriction += frictionalImpulse;
    angularImpulseOfFriction += cross(workingPoint, frictionalImpulse);
  }

  float impulseFactor = -splitMass * (2.0f / 7.0f);

  for (uchar i = 0; i < planeCollisionCount; i++) {
    ushort intersectionIndex = cellVars[cellIndex].collisionPlaneIndices[i];
    ushort planeIndex = cellVars[cellIndex].intersectedPlaneIndices[intersectionIndex];
    float3 intersectionNormal = -grid->normals[planeIndex];
    float3 relativeVelocity = -linearVelocity;
    float3 workingPoint = radius * intersectionNormal;
    float3 linearVelocityOnSurface = linearVelocity - (intersectionNormal * dot(intersectionNormal, linearVelocity));
    float3 frictionalImpulse = impulseFactor * (linearVelocityOnSurface + cross(angularVelocity, workingPoint));
    impulseOfFriction += frictionalImpulse;
    angularImpulseOfFriction += cross(workingPoint, frictionalImpulse);
  }

  cellVars[cellIndex].linearVelocity += impulseOfFriction / mass;
  cellVars[cellIndex].angularVelocity += angularImpulseOfFriction / momentOfInertia;
}

__kernel void collectCollisions(
  __global const Grid* grid,
  __global const Cell* cells,
  __global CellVar* cellVars
) {
  size_t cellIndex = get_global_id(0);
  float mass = cells[cellIndex].mass;
  float3 linearVelocity = cellVars[cellIndex].linearVelocity;
  uchar cellIntersectionCount = cellVars[cellIndex].cellIntersectionCount;
  uchar cellCollisionCount = 0;
  for (uchar i = 0; i < cellIntersectionCount; i++) {
    ushort otherCellIndex = cellVars[cellIndex].intersectedCellIndices[i];
    float3 otherLinearVelocity = cellVars[otherCellIndex].linearVelocity;
    float3 relativeVelocity = otherLinearVelocity - linearVelocity;
    float3 intersectionNormal = cellVars[cellIndex].cellIntersectionNormals[i];
    float collisionSpeed = dot(intersectionNormal, relativeVelocity);
    if (collisionSpeed < 0.0f) {
      cellVars[cellIndex].collisionCellIndices[cellCollisionCount] = i;
      cellVars[cellIndex].cellCollisionSpeeds[cellCollisionCount] = collisionSpeed;
      cellCollisionCount++;
    }
  }

  uchar planeIntersectionCount = cellVars[cellIndex].planeIntersectionCount;
  uchar planeCollisionCount = 0;

  for (uchar i = 0; i < planeIntersectionCount; i++) {
    ushort planeIndex = cellVars[cellIndex].intersectedPlaneIndices[i];
    float3 intersectionNormal = -grid->normals[planeIndex];
    float3 relativeVelocity = -linearVelocity;
    float collisionSpeed = dot(intersectionNormal, relativeVelocity);
    if (collisionSpeed < 0.0f) {
      cellVars[cellIndex].collisionPlaneIndices[planeCollisionCount] = i;
      cellVars[cellIndex].planeCollisionSpeeds[planeCollisionCount] = collisionSpeed;
      planeCollisionCount++;
    }
  }

  uchar allCollisionCount = cellCollisionCount + planeCollisionCount;
  float splitMass = allCollisionCount == 0 ? mass : mass / (float)allCollisionCount;

  cellVars[cellIndex].splitMass = splitMass;
  cellVars[cellIndex].cellCollisionCount = cellCollisionCount;
  cellVars[cellIndex].planeCollisionCount = planeCollisionCount;
}

__kernel void updateVelocity(
  __global const Grid* grid,
  __global const Cell* cells,
  __global CellVar* cellVars
) {
  size_t cellIndex = get_global_id(0);
  uchar cellCollisionCount = cellVars[cellIndex].cellCollisionCount;
  uchar planeCollisionCount = cellVars[cellIndex].planeCollisionCount;
  if (cellCollisionCount + planeCollisionCount == 0) {
    return;
  }

  float elasticity = cells[cellIndex].elasticity;
  float mass = cells[cellIndex].mass;
  float splitMass = cellVars[cellIndex].splitMass;
  float3 linearVelocity = cellVars[cellIndex].linearVelocity;
  float3 splitLinearMomentum = linearVelocity * splitMass;
  float3 impulseOfConstraint = (float3)(0.0f);
  for (uchar i = 0; i < cellCollisionCount; i++) {
    ushort intersectionIndex = cellVars[cellIndex].collisionCellIndices[i];
    ushort otherCellIndex = cellVars[cellIndex].intersectedCellIndices[intersectionIndex];

    float otherElasticity = cells[otherCellIndex].elasticity;
    float otherSplitMass = cellVars[otherCellIndex].splitMass;

    float collisionSpeed = cellVars[cellIndex].cellCollisionSpeeds[i];
    float3 intersectionNormal = cellVars[cellIndex].cellIntersectionNormals[intersectionIndex];
    float scalarOfEffectiveMomentum = dot(splitLinearMomentum, intersectionNormal);
    float3 effectiveMomentum = intersectionNormal * scalarOfEffectiveMomentum;
    float3 notEffectiveMomentum = splitLinearMomentum - effectiveMomentum;

    float scalarOfImpulse = ((collisionSpeed * (1.0f + (elasticity * otherElasticity)) / (1.0f + (splitMass / otherSplitMass))) + (scalarOfEffectiveMomentum / splitMass)) * splitMass;
    impulseOfConstraint += notEffectiveMomentum + intersectionNormal * scalarOfImpulse;
  }


  for (uchar i = 0; i < planeCollisionCount; i++) {
    ushort intersectionIndex = cellVars[cellIndex].collisionPlaneIndices[i];
    ushort planeIndex = cellVars[cellIndex].intersectedPlaneIndices[intersectionIndex];
    float3 intersectionNormal = -grid->normals[planeIndex];
    float collisionSpeed = cellVars[cellIndex].planeCollisionSpeeds[i];
    float scalarOfImpulse = collisionSpeed * elasticity * splitMass;
    float3 notEffectiveMomentum = splitLinearMomentum - (intersectionNormal * dot(splitLinearMomentum, intersectionNormal));
    impulseOfConstraint += notEffectiveMomentum + intersectionNormal * scalarOfImpulse;
  }

  cellVars[cellIndex].linearVelocity = impulseOfConstraint / mass;
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
  float radius = cells[cellIndex].radius;

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
  float radius = cells[cellIndex].radius;
  float mass = cells[cellIndex].mass;
  float3 corner = grid->origin + (float3)(radius);
  float3 nextPosition = nextStates[cellIndex].position;

  float cellIntersectionCount = cellVars[cellIndex].cellIntersectionCount;
  float3 translation = (float3)(0.0f);
  for (uchar i = 0; i < cellIntersectionCount; i++) {
    ushort otherCellIndex = cellVars[cellIndex].intersectedCellIndices[i];
    float3 otherNextPosition = nextStates[otherCellIndex].position;
    float r = radius + cells[otherCellIndex].radius;
    float3 relativePosition = otherNextPosition - nextPosition;
    float dist = length(relativePosition);
    float intersectionLength = r - dist;
    if (dist != 0.0f && intersectionLength > 0.0f) {
      translation += (relativePosition / dist) * (-intersectionLength / 2.0f);
      cellIntersectionCount++;
    }
  }

  if (cellIntersectionCount > 0.0f) {
    //nextPosition += translation / cellIntersectionCount;
  }

  float3 nextLinearMomentum = nextStates[cellIndex].linearMomentum;
  /*
  float yVelocity = nextLinearMomentum.y / mass;
  if (yVelocity > 0.0f && yVelocity <= (9.8f / 30.0f) * 2.0f) {
    nextLinearMomentum.y = 0.0f;
    }*/
  nextStates[cellIndex].position = clamp(nextPosition, corner, -corner);
  nextStates[cellIndex].linearMomentum = nextLinearMomentum;
}
