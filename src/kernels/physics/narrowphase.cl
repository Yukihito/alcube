void setIntersection(
  __global Intersection* intersection,
  ushort type,
  ushort otherIndex,
  float length,
  float speed,
  float3 normal
);

void setIntersection(
  __global Intersection* intersection,
  ushort type,
  ushort otherIndex,
  float length,
  float speed,
  float3 normal
) {
  intersection->type = type;
  intersection->otherIndex = otherIndex;
  intersection->length = length;
  intersection->speed = speed;
  intersection->normal = normal;
}

__kernel void collectIntersections(
  __global const Grid* grid,
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global const Spring* springs,
  __global RigidBodyState* nextStates,
  __global GridAndCellRelation* relations,
  __global uint* gridStartIndices,
  __global uint* gridEndIndices,
  const float sphericalShellRadius,
  const float deltaTime,
  const float gravityAcceleration
) {
  size_t cellIndex = get_global_id(0);
  float edgeLength = (float)grid->edgeLength;
  __global RigidBodyState* nextState = &nextStates[cellIndex];
  __global CellVar* cellVar = &cellVars[cellIndex];
  __global const Cell* cell = &cells[cellIndex];
  float3 position = nextState->position;
  float* positionPtr = (float*)&position;
  float radius = cell->radius;
  int alterEgoIndex = cell->alterEgoIndex;
  float radiusForAlterEgo = cell->radiusForAlterEgo;
  float mass = cell->mass;
  float smallValue = 0.0001f;
  bool isFullOfIntersection = false;
  bool isFloating = true;
  float effectiveRadius = radius + 3.0f;
  uint3 gridCorner0 = (uint3)(0);
  uint3 gridCorner1 = (uint3)(grid->xCount - 1, grid->yCount - 1, grid->zCount - 1);
  uint3 checkStartGrid = clamp(convert_uint3((position - (float3)(effectiveRadius) - grid->origin) / edgeLength), gridCorner0, gridCorner1);
  uint3 checkEndGrid   = clamp(convert_uint3((position + (float3)(effectiveRadius) - grid->origin) / edgeLength), gridCorner0, gridCorner1);
  uchar intersectionCount = 0;
  for (uint gridZ = checkStartGrid.z; gridZ <= checkEndGrid.z && !isFullOfIntersection; gridZ++) {
    for (uint gridY = checkStartGrid.y; gridY <= checkEndGrid.y && !isFullOfIntersection; gridY++) {
      for (uint gridX = checkStartGrid.x; gridX <= checkEndGrid.x && !isFullOfIntersection; gridX++) {
	uint gridIndex = gridX + gridY * grid->xCount + gridZ * grid->xCount * grid->yCount;
	uint checkStartIndex = gridStartIndices[gridIndex];
	uint checkEndIndex = gridEndIndices[gridIndex];
	for (uint i = checkStartIndex; i < checkEndIndex && !isFullOfIntersection; i++) {
	  ushort otherCellIndex = relations[i].cellIndex;
	  if (otherCellIndex == cellIndex) {
	    continue;
	  }
	  float3 w = nextStates[otherCellIndex].position - position;
	  float r = alterEgoIndex == -1 || alterEgoIndex != otherCellIndex ? radius + cells[otherCellIndex].radius : radiusForAlterEgo + cells[otherCellIndex].radiusForAlterEgo;
	  float rr = r * r;
	  float ww = dot(w, w);
	  if (ww > 0.0f && ww <= rr + smallValue) {
	    setIntersection(
              &cellVar->intersections[intersectionCount], 0, otherCellIndex, r - length(w), 0.0f, normalize(w));
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
      setIntersection(
        &cellVar->intersections[intersectionCount], 1, i, cornerPtr[i] - positionPtr[i], 0.0f, -grid->normals[i]);
      intersectionCount++;
      isFullOfIntersection = intersectionCount >= 16;
      if (i == 1) {
	isFloating = false;
      }
    }
  }

  for (uint i = 3; i < 6 && !isFullOfIntersection; i++) {
    uint pi = i - 3;
    if (positionPtr[pi] >= -cornerPtr[pi]) {
      setIntersection(
        &cellVar->intersections[intersectionCount], 1, i, positionPtr[pi] + cornerPtr[pi], 0.0f, -grid->normals[i]);
      intersectionCount++;
      isFullOfIntersection = intersectionCount >= 16;
    }
  }

  float shellIntersectionLength = length(position) + radius - sphericalShellRadius;
  if (!isFullOfIntersection && shellIntersectionLength + smallValue > 0.0f) {
    setIntersection(
      &cellVar->intersections[intersectionCount], 2, 0, shellIntersectionLength, 0.0f, normalize(position));
    intersectionCount++;
    isFullOfIntersection = intersectionCount >= 16;
  }

  float momentOfInertia = (2.0f / 5.0f) * mass * radius * radius;
  cellVar->momentOfInertia = momentOfInertia;
  float gravityTranslation = gravityAcceleration * deltaTime;
  float gravity = isFloating ? -gravityTranslation : 0.0f;
  cellVar->linearVelocity = nextState->linearMomentum / mass + (float3)(0.0f, gravity, 0.0f);
  float ySpeed = cellVar->linearVelocity.y;
  if (!isFloating && ySpeed * ySpeed < gravityTranslation * gravityTranslation * 16.0f) {
    cellVar->linearVelocity.y = 0.0f;
  }
  cellVar->angularVelocity = nextState->angularMomentum / momentOfInertia;
  cellVar->isFloating = isFloating ? 1 : 0;

  cellVar->intersectionCount = intersectionCount;
  cellVar->collisionCount = 0;

  cellVar->massForIntersection = mass / intersectionCount;
  cellVar->massForCollision = mass;
}
