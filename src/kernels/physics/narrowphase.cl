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
