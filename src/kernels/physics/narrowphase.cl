void setIntersection(
  __global Intersection* intersection,
  ushort type,
  ushort otherIndex,
  float intersectionLength,
  float speed,
  float3 normal,
  float3 relativePosition
) {
  intersection->type = type;
  intersection->otherIndex = otherIndex;
  intersection->length = intersectionLength;
  intersection->speed = speed;
  intersection->normal = normal;
  intersection->relativePosition = relativePosition;
  intersection->distance = length(relativePosition);
}

__kernel void collectIntersections(
  __global ActorState* actorStates,
  __global GridAndActorRelation* relations,
  __global uint* gridStartIndices,
  __global uint* gridEndIndices,
  __global Constants* constants
) {
  size_t actorIndex = get_global_id(0);
  __global ActorState* actorState = &actorStates[actorIndex];
  __global Actor* actor = &(actorStates[actorIndex].constants);
  if (!actor->isAlive) {
    return;
  }
  float sphericalShellRadius = constants->sphericalShellRadius;
  float deltaTime = constants->deltaTime;
  float gravityAcceleration = constants->gravityAcceleration;
  __global Grid* grid = &constants->grid;
  float edgeLength = (float)grid->edgeLength;
  float3 position = actor->position;
  float* positionPtr = (float*)&position;
  float radius = actorState->radius;
  float mass = actorState->mass;
  float smallValue = 0.0001f;
  uchar maxIntersection = 32;
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
	  ushort otherActorIndex = relations[i].actorIndex;
	  if (otherActorIndex == actorIndex) {
	    continue;
	  }
	  __global ActorState* otherActorState = &actorStates[otherActorIndex];
	  __global Actor* otherActor = &(otherActorState->constants);
	  float3 w = otherActor->position - position;
	  float r = radius + otherActorState->radius;
	  float rr = r * r;
	  float ww = dot(w, w);
	  if (ww > 0.0f && ww <= rr + smallValue) {
	    setIntersection(&actorState->intersections[intersectionCount], otherActor->type, otherActorIndex, r - length(w), 0.0f, normalize(w), w);
	    intersectionCount++;
	    isFullOfIntersection = intersectionCount >= maxIntersection;
	  }
	}
      }
    }
  }

  float3 corner = grid->origin + (float3)(radius + smallValue);
  float* cornerPtr = (float*)&corner;
  for (uint i = 0; i < 3 && !isFullOfIntersection; i++) {
    if (positionPtr[i] <= cornerPtr[i]) {
      setIntersection(&actorState->intersections[intersectionCount], ACTOR_TYPE_FACE, i, cornerPtr[i] - positionPtr[i], 0.0f, -grid->normals[i], grid->normals[i] * radius);
      intersectionCount++;
      isFullOfIntersection = intersectionCount >= maxIntersection;
      if (i == 1) {
	isFloating = false;
      }
    }
  }

  for (uint i = 3; i < 6 && !isFullOfIntersection; i++) {
    uint pi = i - 3;
    if (positionPtr[pi] >= -cornerPtr[pi]) {
      setIntersection(&actorState->intersections[intersectionCount], ACTOR_TYPE_FACE, i, positionPtr[pi] + cornerPtr[pi], 0.0f, -grid->normals[i], grid->normals[i] * radius);
      intersectionCount++;
      isFullOfIntersection = intersectionCount >= maxIntersection;
    }
  }

  float shellIntersectionLength = length(position) + radius - sphericalShellRadius;
  if (!isFullOfIntersection && shellIntersectionLength + smallValue > 0.0f) {
    setIntersection(&actorState->intersections[intersectionCount], ACTOR_TYPE_SPHERICAL_SHELL, 0, shellIntersectionLength, 0.0f, normalize(position), -normalize(position) * radius);
    intersectionCount++;
    isFullOfIntersection = intersectionCount >= maxIntersection;
  }


  actorState->intersectionCount = intersectionCount;
  actorState->collisionCount = 0;
  actorState->isFloating = isFloating ? 1 : 0;
}
