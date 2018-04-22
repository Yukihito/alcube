
typedef struct __attribute__ ((packed)) ActorStruct {
  ushort type;
  char _padding0[2];
  ushort subPhysicalQuantityIndex;
  char _padding1[2];
} Actor;

typedef struct __attribute__ ((packed)) FluidStruct {
  ushort actorIndex;
  char _padding0[2];
} Fluid;

typedef struct __attribute__ ((packed)) FluidSettingsStruct {
  float stiffness;
  float density;
  float viscosity;
  float particleMass;
  float effectiveRadius;
  float poly6Constant;
  float spikyGradientConstant;
  float viscosityLaplacianConstant;
} FluidSettings;

typedef struct __attribute__ ((packed)) GridStruct {
  uint xCount;
  uint yCount;
  uint zCount;
  uint edgeLength;
  float3 origin;
  float3 normals[6];
} Grid;

typedef struct __attribute__ ((packed)) GridAndActorRelationStruct {
  uint gridIndex;
  ushort actorIndex;
  char _padding0[2];
} GridAndActorRelation;

typedef struct __attribute__ ((packed)) IntersectionStruct {
  ushort type;
  char _padding0[2];
  ushort otherIndex;
  char _padding1[2];
  float length;
  float speed;
  float3 normal;
  float3 relativePosition;
  float distance;
} Intersection;

typedef struct __attribute__ ((packed)) PhysicalQuantityStruct {
  float radius;
  float mass;
  float3 position;
  float4 rotation;
  float3 linearMomentum;
  float3 angularMomentum;
} PhysicalQuantity;

typedef struct __attribute__ ((packed)) RendererStruct {
  int refersToRotations;
  uint instanceColorType;
  float3 instanceColor;
} Renderer;

typedef struct __attribute__ ((packed)) SoftBodyStruct {
  float elasticity;
  uint springIndices[16];
  uchar springNodeIndices[16];
  uint springCount;
  ushort actorIndex;
  char _padding0[2];
} SoftBody;

typedef struct __attribute__ ((packed)) SpringStruct {
  float k;
  float3 nodePositionsModelSpace[2];
  ushort actorIndices[2];
} Spring;

typedef struct __attribute__ ((packed)) SpringStateStruct {
  Spring constants;
  float3 linearImpulses[2];
  float3 angularImpulses[2];
} SpringState;

typedef struct __attribute__ ((packed)) ActorStateStruct {
  Actor constants;
  float radius;
  float mass;
  float3 linearVelocity;
  float3 angularVelocity;
  float momentOfInertia;
  float massForIntersection;
  float massForCollision;
  ushort intersectionCount;
  char _padding0[2];
  ushort collisionCount;
  char _padding1[2];
  int isFloating;
  uchar collisionIndices[32];
  Intersection intersections[32];
  float pressure;
  float density;
  float3 fluidForce;
} ActorState;

typedef struct __attribute__ ((packed)) ConstantsStruct {
  Grid grid;
  FluidSettings fluidSettings;
  float gravityAcceleration;
  float deltaTime;
  float splitDeltaTime;
  float sphericalShellRadius;
} Constants;

float4 mulQuat(float4 q, float4 r);

float3 rotateByQuat(float3 v, float4 q);

float4 createQuatFromDisplacement(float3 angularDisplacement);

void setIntersection(__global Intersection* intersection, ushort type, ushort otherIndex, float intersectionLength, float speed, float3 normal, float3 relativePosition);

void accumulateConstraintImpulse(__global Actor* actor, __global ActorState* actorState, __global ActorState* actorStates, __global Intersection* intersection, __global SoftBody* softBodys, float3* acc);

void accumulateFrictionalImpulse(__global ActorState* actorState, __global ActorState* actorStates, __global Intersection* intersection, float3* linearMomentumAcc, float3* angularMomentumAcc);

void accumulatePenaltyImpulse(__global Intersection* intersection, const float deltaTime, float penaltyFactor, float3* acc);

#define ACTOR_TYPE_SOFT_BODY       0
#define ACTOR_TYPE_FACE            1
#define ACTOR_TYPE_SPHERICAL_SHELL 2
#define ACTOR_TYPE_FLUID           3

float4 mulQuat(float4 q, float4 r) {
  float4 result;
  result.xyz = r.xyz * q.w + q.xyz * r.w + cross(q.xyz, r.xyz);
  result.w = q.w * r.w - dot(q.xyz, r.xyz);
  return result;
}

float3 rotateByQuat(float3 v, float4 q) {
  return mulQuat(mulQuat(q, (float4)(v.x, v.y, v.z, 0.0f)), (float4)(-q.x, -q.y, -q.z, q.w)).xyz;
}

float4 createQuatFromDisplacement(float3 angularDisplacement) {
  float halfRotationScalar = length(angularDisplacement) / 2.0f;
  float4 q;
  q.w = cos(halfRotationScalar);
  q.xyz = normalize(angularDisplacement) * sin(halfRotationScalar);
  return q;
}

__kernel void inputConstants(
  __global Constants* constants,
  __global const Grid* grid,
  __global const FluidSettings* fluidSettings,
  const float gravityAcceleration,
  const float deltaTime,
  const float splitDeltaTime,
  const float sphericalShellRadius
) {
  constants->grid = grid[0];
  constants->fluidSettings = fluidSettings[0];
  constants->gravityAcceleration = gravityAcceleration;
  constants->deltaTime = deltaTime;
  constants->splitDeltaTime = splitDeltaTime;
  constants->sphericalShellRadius = sphericalShellRadius;
}

__kernel void inputActors(
  __global const Actor* actors,
  __global ActorState* actorStates,
  __global const PhysicalQuantity* hostPhysicalQuantities,
  __global PhysicalQuantity* physicalQuantities
) {
  size_t i = get_global_id(0);
  physicalQuantities[i] = hostPhysicalQuantities[i];
  actorStates[i].constants = actors[i];
  actorStates[i].radius = physicalQuantities[i].radius;
  actorStates[i].mass = physicalQuantities[i].mass;
}

__kernel void inputSoftBodys(
  __global const SoftBody* hostSoftBodys,
  __global SoftBody* softBodys
) {
  size_t i = get_global_id(0);
  softBodys[i] = hostSoftBodys[i];
}

__kernel void inputSprings(
  __global const Spring* springs,
  __global SpringState* springStates
) {
  size_t i = get_global_id(0);
  springStates[i].constants = springs[i];
}

__kernel void inputFluids(
  __global Fluid* hostFluids,
  __global Fluid* fluids
) {
  size_t i = get_global_id(0);
  fluids[i] = hostFluids[i];
}

__kernel void initGridAndActorRelations(
  __global GridAndActorRelation* relations,
  const uint gridIndex,
  const unsigned short actorIndex
) {
  size_t i = get_global_id(0);
  relations[i].gridIndex = gridIndex;
  relations[i].actorIndex = actorIndex;
}

__kernel void fillGridIndex(
  __global Constants* constants,
  __global PhysicalQuantity* physicalQuantities,
  __global GridAndActorRelation* relations
) {
  __global Grid* grid = &constants->grid;
  size_t actorIndex = get_global_id(0);
  float edgeLength = grid->edgeLength;
  float3 position = physicalQuantities[actorIndex].position;
  float3 positionGridSpace = position - grid->origin;
  uint3 gridCorner0 = (uint3)(0);
  uint3 gridCorner1 = (uint3)(grid->xCount - 1, grid->yCount - 1, grid->zCount - 1);
  uint3 p = clamp(convert_uint3(positionGridSpace / edgeLength), gridCorner0, gridCorner1);
  uint gridIndex = (uint)p.x + (uint)p.y * grid->xCount + (uint)p.z * grid->xCount * grid->yCount;
  relations[actorIndex].actorIndex = actorIndex;
  relations[actorIndex].gridIndex = gridIndex;
}

__kernel void merge(
  __global GridAndActorRelation* relations,
  const uint distance
) {
  uint index = get_global_id(0);
  if (index % (distance << 1) < distance) {
    GridAndActorRelation left = relations[index];
    GridAndActorRelation right = relations[index + distance];
    bool b = left.gridIndex < right.gridIndex;
    relations[index] = b ? left : right;
    relations[index + distance] = b ? right : left;
  }
}

__kernel void bitonic(
  __global GridAndActorRelation* relations,
  const uint distance,
  const uint stageDistance
) {
  uint index = get_global_id(0);
  if (index % (distance << 1) < distance) {
    uint middleDistance = stageDistance << 1; // * 2
    GridAndActorRelation left = relations[index];
    GridAndActorRelation right = relations[index + distance];
    bool b1 = left.gridIndex < right.gridIndex;
    bool b2 = index % (middleDistance << 1) >= middleDistance;
    bool b3 = !(b1 ^ b2);
    relations[index] = b3 ? left : right;
    relations[index + distance] = b3 ? right : left;
  }
}

__kernel void setGridRelationIndexRange(
  __global GridAndActorRelation* relations,
  __global uint* gridStartIndices,
  __global uint* gridEndIndices,
  const uint actorCount
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
  if (next == actorCount - 1) {
    gridEndIndices[nextGridIndex] = next + 1;
  }
}

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
  __global PhysicalQuantity* physicalQuantities,
  __global GridAndActorRelation* relations,
  __global uint* gridStartIndices,
  __global uint* gridEndIndices,
  __global Constants* constants
) {
  float sphericalShellRadius = constants->sphericalShellRadius;
  float deltaTime = constants->deltaTime;
  float gravityAcceleration = constants->gravityAcceleration;
  __global Grid* grid = &constants->grid;
  size_t actorIndex = get_global_id(0);
  float edgeLength = (float)grid->edgeLength;
  __global PhysicalQuantity* physicalQuantity = &physicalQuantities[actorIndex];
  __global ActorState* actorState = &actorStates[actorIndex];
  __global Actor* actor = &(actorStates[actorIndex].constants);
  float3 position = physicalQuantity->position;
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
	  float3 w = physicalQuantities[otherActorIndex].position - position;
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

__kernel void initStepVariables(
  __global ActorState* actorStates,
  __global PhysicalQuantity* physicalQuantities,
  __global Constants* constants
) {
  size_t actorIndex = get_global_id(0);
  __global ActorState* actorState = &actorStates[actorIndex];
  __global PhysicalQuantity* physicalQuantity = &physicalQuantities[actorIndex];
  float gravityAcceleration = constants->gravityAcceleration;
  float deltaTime = constants->deltaTime;
  float radius = actorState->radius;
  float mass = actorState->mass;
  float momentOfInertia = (2.0f / 5.0f) * mass * radius * radius;
  bool isFloating = actorState->isFloating;
  actorState->momentOfInertia = momentOfInertia;
  float gravityTranslation = gravityAcceleration * deltaTime;
  float gravity = isFloating ? -gravityTranslation : 0.0f;
  actorState->linearVelocity = physicalQuantity->linearMomentum / mass + (float3)(0.0f, gravity, 0.0f);
  float ySpeed = actorState->linearVelocity.y;
  if (!isFloating && ySpeed * ySpeed < gravityTranslation * gravityTranslation * 16.0f) {
    actorState->linearVelocity.y = 0.0f;
  }
  actorState->angularVelocity = physicalQuantity->angularMomentum / momentOfInertia;
  actorState->massForIntersection = mass / actorState->intersectionCount;
  actorState->massForCollision = mass;
  actorState->fluidForce = (float3)(0.0f);
  actorState->pressure = 0.0f;
  actorState->density = 0.0f;
}

void accumulatePenaltyImpulse(
  __global Intersection* intersection,
  const float deltaTime,
  float penaltyFactor,
  float3* acc
) {
  *acc += (-penaltyFactor * log2(intersection->length + 1.0f) * deltaTime) * intersection->normal;
}

__kernel void updateByPenaltyImpulse(
  __global ActorState* actorStates,
  __global SoftBody* softBodys,
  __global Constants* constants
) {
  size_t subIndex = get_global_id(0);
  ushort actorIndex = softBodys[subIndex].actorIndex;
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->intersectionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = actorState->intersections;
  float3 impulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    accumulatePenaltyImpulse(&intersections[i], constants->deltaTime, 64.0f, &impulse);
  }
  actorState->linearVelocity += impulse / actorState->mass;
}

void accumulateFrictionalImpulse(
  __global ActorState* actorState,
  __global ActorState* actorStates,
  __global Intersection* intersection,
  float3* linearMomentumAcc,
  float3* angularMomentumAcc
) {
  unsigned int intersectionType = intersection->type;
  ushort otherIndex = intersection->otherIndex;
  float mass = actorState->massForIntersection;
  float s = intersectionType == 0 ? 1.0f / actorStates[otherIndex].massForIntersection : 0;
  float t = (7.0f / 2.0f) * ((1.0f / mass) + s);
  float3 intersectionNormal = intersection->normal;
  float3 otherLinearVelocity = intersectionType == 0 ? actorStates[otherIndex].linearVelocity : (float3)(0.0f);
  float3 relativeVelocity = otherLinearVelocity - actorState->linearVelocity;
  float3 relativeVelocityOnSurface = relativeVelocity - intersectionNormal * dot(relativeVelocity, intersectionNormal);
  float3 workingPoint = actorState->radius * intersectionNormal;
  float3 workingPointVelocity = cross(actorState->angularVelocity, workingPoint);
  float3 otherWorkingPointVelocity = intersectionType == 0 ? cross(actorStates[otherIndex].angularVelocity, -actorStates[otherIndex].radius * intersectionNormal) : (float3)(0.0f);
  float3 relativeWorkingPointVelocity = otherWorkingPointVelocity - workingPointVelocity;
  float3 frictionalImpulse = (relativeVelocityOnSurface + relativeWorkingPointVelocity) / t;
  *linearMomentumAcc += frictionalImpulse;
  *angularMomentumAcc += cross(workingPoint, frictionalImpulse);
}

__kernel void updateByFrictionalImpulse(
  __global ActorState* actorStates,
  __global SoftBody* softBodys
) {
  size_t subIndex = get_global_id(0);
  ushort actorIndex = softBodys[subIndex].actorIndex;
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->intersectionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = actorState->intersections;
  float3 impulse = (float3)(0.0f);
  float3 angularImpulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    if (intersections[i].type == ACTOR_TYPE_FLUID) {
      continue;
    }
    accumulateFrictionalImpulse(actorState, actorStates, &intersections[i], &impulse, &angularImpulse);
  }
  actorState->linearVelocity += impulse / actorState->mass;
  actorState->angularVelocity += angularImpulse / actorState->momentOfInertia;
}

__kernel void collectCollisions(
  __global ActorState* actorStates,
  __global SoftBody* softBodys
) {
  size_t subIndex = get_global_id(0);
  ushort actorIndex = softBodys[subIndex].actorIndex;
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->intersectionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = actorState->intersections;
  uchar collisionCount = 0;
  for (uchar i = 0; i < count; i++) {
    float3 relativeSpeed = intersections[i].type == ACTOR_TYPE_SOFT_BODY || intersections[i].type == ACTOR_TYPE_FLUID ? actorStates[intersections[i].otherIndex].linearVelocity - actorState->linearVelocity : - actorState->linearVelocity;
    intersections[i].speed = dot(intersections[i].normal, relativeSpeed);
    if (intersections[i].speed < 0.0f) {
      actorState->collisionIndices[collisionCount] = i;
      collisionCount++;
    }
  }
  actorState->collisionCount = collisionCount;
  actorState->massForCollision = actorState->mass / collisionCount;
}

void accumulateConstraintImpulse(
  __global Actor* actor,
  __global ActorState* actorState,
  __global ActorState* actorStates,
  __global Intersection* intersection,
  __global SoftBody* softBodys,
  float3* acc
) {
  if (intersection->speed >= 0.0f) {
    return;
  }
  unsigned int intersectionType = intersection->type;
  ushort otherIndex = intersection->otherIndex;
  float mass = actorState->massForCollision;
  float elasticity = intersectionType == ACTOR_TYPE_SOFT_BODY ? softBodys[actor->subPhysicalQuantityIndex].elasticity * softBodys[actorStates[otherIndex].constants.subPhysicalQuantityIndex].elasticity : softBodys[actor->subPhysicalQuantityIndex].elasticity;
  float massRatio = intersectionType == ACTOR_TYPE_SOFT_BODY || intersectionType == ACTOR_TYPE_FLUID ? mass / actorStates[otherIndex].massForCollision : 0.0f;
  float3 intersectionNormal = intersection->normal;
  float speedOnIntersectionNormal = dot(actorState->linearVelocity, intersectionNormal);
  float affectedSpeed = (intersection->speed * (1.0f + elasticity) / (1.0f + massRatio)) + speedOnIntersectionNormal;
  *acc += (actorState->linearVelocity + (affectedSpeed - speedOnIntersectionNormal) * intersectionNormal) * mass;
}

__kernel void updateByConstraintImpulse(
  __global ActorState* actorStates,
  __global SoftBody* softBodys
) {
  size_t subIndex = get_global_id(0);
  ushort actorIndex = softBodys[subIndex].actorIndex;
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->collisionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = actorState->intersections;
  __global Actor* actor = &actorStates[actorIndex].constants;

  float3 impulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    accumulateConstraintImpulse(actor, actorState, actorStates, &intersections[actorState->collisionIndices[i]], softBodys, &impulse);
  }
  actorState->linearVelocity = impulse / actorState->mass;
}

__kernel void updateDensityAndPressure(
  __global ActorState* actorStates,
  __global Fluid* fluids,
  __global Constants* constants
) {
  ushort actorIndex = fluids[get_global_id(0)].actorIndex;
  __global FluidSettings* fluidSettings = &constants->fluidSettings;
  __global ActorState* actorState = &actorStates[actorIndex];

  uchar count = actorState->intersectionCount;
  __global Intersection* intersections = actorState->intersections;
  float hh = fluidSettings->effectiveRadius * fluidSettings->effectiveRadius;
  float density = fluidSettings->particleMass * fluidSettings->poly6Constant * hh * hh * hh;
  for (uchar i = 0; i < count; i++) {
    float rr = intersections[i].distance * intersections[i].distance;
    float q = hh - rr;
    density += fluidSettings->particleMass * fluidSettings->poly6Constant * q * q * q;
  }
  actorState->density = density;
  actorState->pressure = fluidSettings->stiffness * fluidSettings->density * (pow(density / fluidSettings->density, 2) - 1.0f);
}


__kernel void updateFluidForce(
  __global ActorState* actorStates,
  __global Fluid* fluids,
  __global Constants* constants
) {
  __global FluidSettings* fluidSettings = &constants->fluidSettings;
  ushort actorIndex = fluids[get_global_id(0)].actorIndex;
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->intersectionCount;
  __global Intersection* intersections = actorState->intersections;
  float density = actorState->density;
  float3 velocity = actorState->linearVelocity;
  float3 force = (float3)(0.0f, 0.0f, 0.0f);
  float pressurePart1 = -density * fluidSettings->particleMass;
  float pressurePart2 = (actorState->pressure / (density * density));
  float viscosityPart1 = fluidSettings->viscosity * fluidSettings->particleMass;
  for (uchar i = 0; i < count; i++) {
    bool isOtherDynamic = intersections[i].type == ACTOR_TYPE_FLUID || intersections[i].type == ACTOR_TYPE_SOFT_BODY;
    float q = intersections[i].length;
    ushort otherIndex = intersections[i].otherIndex;
    float otherDensity = intersections[i].type == ACTOR_TYPE_FLUID ? actorStates[otherIndex].density : density;
    float otherPressure = intersections[i].type == ACTOR_TYPE_FLUID ? actorStates[otherIndex].pressure : actorState->pressure;
    float3 otherVelocity = isOtherDynamic ? actorStates[otherIndex].linearVelocity : 0.0f;
    force +=
      // pressure
      pressurePart1 * (pressurePart2 + (otherPressure / (otherDensity * otherDensity))) * fluidSettings->spikyGradientConstant * q * q * intersections[i].normal
      // viscosity
      + viscosityPart1 * ((otherVelocity - velocity) / otherDensity) * fluidSettings->viscosityLaplacianConstant * q;
  }
  actorState->fluidForce = force;
}

__kernel void moveFluid(
  __global Fluid* fluids,
  __global ActorState* actorStates,
  __global PhysicalQuantity* physicalQuantities,
  __global Constants* constants
) {
  __global Grid* grid = &constants->grid;
  ushort actorIndex = fluids[get_global_id(0)].actorIndex;
  __global ActorState* actorState = &actorStates[actorIndex];
  __global PhysicalQuantity* physicalQuantity = &physicalQuantities[actorIndex];
  actorState->linearVelocity += (actorState->fluidForce * constants->deltaTime) / constants->fluidSettings.particleMass;
  physicalQuantity->position += constants->deltaTime * actorState->linearVelocity;
}

__kernel void calcSpringImpulses(
  __global Constants* constants,
  __global SpringState* springStates,
  __global PhysicalQuantity* physicalQuantities
) {
  size_t i = get_global_id(0);
  __global SpringState* springState = &springStates[i];
  __global Spring* spring = &springState->constants;
  float3 pm0 = spring->nodePositionsModelSpace[0];
  float4 rot0 = physicalQuantities[spring->actorIndices[0]].rotation;
  float3 pm1 = spring->nodePositionsModelSpace[1];
  float4 rot1 = physicalQuantities[spring->actorIndices[1]].rotation;
  float3 p0 = rotateByQuat(pm0, rot0);
  float3 p1 = rotateByQuat(pm1, rot1);
  float3 impulse = ((p1 + physicalQuantities[spring->actorIndices[1]].position) - (p0 + physicalQuantities[spring->actorIndices[0]].position));
  float3 direction = normalize(impulse);
  float len = length(impulse);
  springState->linearImpulses[0] = log2(1.0f + len) * constants->splitDeltaTime * spring->k * direction;
  springState->linearImpulses[1] = -springState->linearImpulses[0];
  springState->angularImpulses[0] = cross(p0, springState->linearImpulses[0]);
  springState->angularImpulses[1] = cross(p1, springState->linearImpulses[1]);
}

__kernel void updateBySpringImpulse(
  __global Constants* constants,
  __global SoftBody* softBodys,
  __global ActorState* actorStates,
  __global PhysicalQuantity* physicalQuantities,
  __global SpringState* springStates
) {
  size_t softBodyIndex = get_global_id(0);
  __global SoftBody* softBody = &softBodys[softBodyIndex];
  size_t actorIndex = softBody->actorIndex;
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = softBody->springCount;
  float3 linearImpulse = (float3)(0.0f);
  float3 angularImpulse = (float3)(0.0f);

  for (uchar i = 0; i < count; i++) {
    linearImpulse += springStates[softBody->springIndices[i]].linearImpulses[softBody->springNodeIndices[i]];
    angularImpulse += springStates[softBody->springIndices[i]].angularImpulses[softBody->springNodeIndices[i]];;
  }

  actorState->linearVelocity += linearImpulse / actorState->mass;
  actorState->angularVelocity += angularImpulse / actorState->momentOfInertia;
  physicalQuantities[actorIndex].position += actorState->linearVelocity * constants->splitDeltaTime;
  physicalQuantities[actorIndex].rotation = mulQuat(createQuatFromDisplacement(actorState->angularVelocity * constants->splitDeltaTime), physicalQuantities[actorIndex].rotation);
}

__kernel void postProcessing(
  __global Constants* constants,
  __global ActorState* actorStates,
  __global PhysicalQuantity* physicalQuantities
) {
  size_t actorIndex = get_global_id(0);
  float3 maxLinearMomentum = (float3)((2.0f * (1.0f / constants->deltaTime)) * actorStates[actorIndex].mass);
  __global PhysicalQuantity* physicalQuantity = &physicalQuantities[actorIndex];
  physicalQuantity->angularMomentum = actorStates[actorIndex].angularVelocity * actorStates[actorIndex].momentOfInertia * 0.999f;
  physicalQuantity->linearMomentum = clamp(actorStates[actorIndex].linearVelocity * actorStates[actorIndex].mass * 0.999f, -maxLinearMomentum, maxLinearMomentum);

  if (length(physicalQuantity->linearMomentum) < 0.01f) {
    physicalQuantity->linearMomentum = (float3)(0.0f);
  }

  if (length(physicalQuantity->angularMomentum) < 0.01f) {
    physicalQuantity->angularMomentum = (float3)(0.0f);
  }

  float3 corner = constants->grid.origin + (float3)(0.0001f);

  physicalQuantity->position = clamp(physicalQuantity->position, corner, -corner);
}

__kernel void inputRenderers(
  __global const Renderer* hostRenderers,
  __global Renderer* renderers
) {
  size_t i = get_global_id(0);
  renderers[i] = hostRenderers[i];
}

__kernel void updateDrawingBuffer(
  __global float3* positions,
  __global float3* colors,
  __global float4* rotations0,
  __global float4* rotations1,
  __global float4* rotations2,
  __global float4* rotations3,
  __global PhysicalQuantity* physicalQuantities,
  __global Renderer* renderers
) {
  size_t i = get_global_id(0);
  positions[i] = physicalQuantities[i].position;

  if (renderers[i].instanceColorType == 3) {
    colors[i] = fabs(physicalQuantities[i].linearMomentum);
  }

  if (renderers[i].refersToRotations) {
    __global float4* r = &physicalQuantities[i].rotation;
    float x = r->x;
    float y = r->y;
    float z = r->z;
    float w = r->w;
    rotations0[i] = (float4)(1.0f - 2.0f * (y*y + z*z), 2.0f * (x*y + w*z), 2.0f * (x*z - w*y), 0.0f);
    rotations1[i] = (float4)(2.0f * (x*y - w*z), 1.0f - 2.0f * (x*x + z*z), 2.0f * (y*z + w*x), 0.0f);
    rotations2[i] = (float4)(2.0f * (x*z + w*y), 2.0f * (y*z - w*x), 1.0f - 2.0f * (x*x + y*y), 0.0f);
    rotations3[i] = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
  }
}

__kernel void updateDrawingBuffer_SingleColor(
  __global float3* positions,
  __global PhysicalQuantity* physicalQuantities
) {
  size_t i = get_global_id(0);
  positions[i] = physicalQuantities[i].position;
}

__kernel void updateDrawingBuffer_InstanceColor(
  __global float3* positions,
  __global float3* colors,
  __global PhysicalQuantity* physicalQuantities
) {
  size_t i = get_global_id(0);
  positions[i] = physicalQuantities[i].position;
  colors[i] = fabs(physicalQuantities[i].linearMomentum);
}

__kernel void updateDrawingBuffer_Texture_SingleColor(
  __global float3* positions,
  __global float4* rotations0,
  __global float4* rotations1,
  __global float4* rotations2,
  __global float4* rotations3,
  __global PhysicalQuantity* physicalQuantities
) {
  size_t i = get_global_id(0);
  positions[i] = physicalQuantities[i].position;
  __global float4* r = &physicalQuantities[i].rotation;
  float x = r->x;
  float y = r->y;
  float z = r->z;
  float w = r->w;

  rotations0[i] = (float4)(1.0f - 2.0f * (y*y + z*z), 2.0f * (x*y + w*z), 2.0f * (x*z - w*y), 0.0f);
  rotations1[i] = (float4)(2.0f * (x*y - w*z), 1.0f - 2.0f * (x*x + z*z), 2.0f * (y*z + w*x), 0.0f);
  rotations2[i] = (float4)(2.0f * (x*z + w*y), 2.0f * (y*z - w*x), 1.0f - 2.0f * (x*x + y*y), 0.0f);
  rotations3[i] = (float4)(0.0f, 0.0f, 0.0f, 1.0f);
}
