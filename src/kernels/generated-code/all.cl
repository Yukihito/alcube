
typedef struct __attribute__ ((packed)) ActorStruct {
  float radius;
  float mass;
  ushort type;
  char _padding0[2];
  float elasticity;
  float dynamicFrictionCoefficient;
  float staticFrictionCoefficient;
  uint springIndices[16];
  uchar springNodeIndices[16];
  uint springCount;
  int alterEgoIndex;
  float radiusForAlterEgo;
} Actor;

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

typedef struct __attribute__ ((packed)) FluidStateStruct {
  float3 velocity;
  float pressure;
  float density;
  float3 force;
} FluidState;

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

typedef struct __attribute__ ((packed)) RigidBodyStateStruct {
  float3 position;
  float4 rotation;
  float3 linearMomentum;
  float3 angularMomentum;
} RigidBodyState;

typedef struct __attribute__ ((packed)) SpringStruct {
  float k;
  float3 nodePositionsModelSpace[2];
  ushort actorIndices[2];
} Spring;

typedef struct __attribute__ ((packed)) SpringVarStruct {
  float3 linearImpulses[2];
  float3 angularImpulses[2];
} SpringVar;

typedef struct __attribute__ ((packed)) ActorStateStruct {
  Actor constants;
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
} ActorState;

typedef struct __attribute__ ((packed)) ConstantsStruct {
  Grid grid;
  FluidSettings fluidSettings;
  float gravityAcceleration;
  float deltaTime;
  float splitDeltaTime;
  float sphericalShellRadius;
  ushort rigidBodyParticleCount;
  char _padding0[2];
  ushort fluidParticleCount;
  char _padding1[2];
} Constants;

float4 mulQuat(float4 q, float4 r);

float3 rotateByQuat(float3 v, float4 q);

float4 createQuatFromDisplacement(float3 angularDisplacement);

void accumulatePenaltyImpulse(__global Intersection* intersection, const float deltaTime, float3* acc);

void accumulateFrictionalImpulse(__global const Actor* actor, __global ActorState* actorState, __global const Actor* actors, __global ActorState* actorStates, __global Intersection* intersection, float3* linearMomentumAcc, float3* angularMomentumAcc);

void accumulateConstraintImpulse(__global const Actor* actor, __global ActorState* actorState, __global const Actor* actors, __global ActorState* actorStates, __global Intersection* intersection, float3* acc);

void setIntersection(__global Intersection* intersection, ushort type, ushort otherIndex, float intersectionLength, float speed, float3 normal, float3 relativePosition);

#define PARTICLE_TYPE_RIGID_BODY      = 0
#define PARTICLE_TYPE_FACE            = 1
#define PARTICLE_TYPE_SPHERICAL_SHELL = 2
#define PARTICLE_TYPE_FLUID           = 3

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
  const float sphericalShellRadius,
  const ushort rigidBodyParticleCount,
  const ushort fluidParticleCount
) {
  constants->grid = grid[0];
  constants->fluidSettings = fluidSettings[0];
  constants->gravityAcceleration = gravityAcceleration;
  constants->deltaTime = deltaTime;
  constants->splitDeltaTime = splitDeltaTime;
  constants->sphericalShellRadius = sphericalShellRadius;
  constants->rigidBodyParticleCount = rigidBodyParticleCount;
  constants->fluidParticleCount = fluidParticleCount;
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
  __global const Grid* grid,
  __global const Actor* actors,
  __global ActorState* actorStates,
  __global const RigidBodyState* currentStates,
  __global RigidBodyState* nextStates,
  __global GridAndActorRelation* relations
) {
  size_t actorIndex = get_global_id(0);
  float edgeLength = grid->edgeLength;
  float3 position = currentStates[actorIndex].position;
  float3 positionGridSpace = position - grid->origin;
  uint3 gridCorner0 = (uint3)(0);
  uint3 gridCorner1 = (uint3)(grid->xCount - 1, grid->yCount - 1, grid->zCount - 1);
  uint3 p = clamp(convert_uint3(positionGridSpace / edgeLength), gridCorner0, gridCorner1);
  uint gridIndex = (uint)p.x + (uint)p.y * grid->xCount + (uint)p.z * grid->xCount * grid->yCount;
  relations[actorIndex].actorIndex = actorIndex;
  relations[actorIndex].gridIndex = gridIndex;
  nextStates[actorIndex] = currentStates[actorIndex];
  actorStates[actorIndex].constants = actors[actorIndex];
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
  __global const Spring* springs,
  __global RigidBodyState* nextStates,
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
  __global RigidBodyState* nextState = &nextStates[actorIndex];
  __global ActorState* actorState = &actorStates[actorIndex];
  __global Actor* actor = &(actorStates[actorIndex].constants);
  float3 position = nextState->position;
  float* positionPtr = (float*)&position;
  float radius = actor->radius;
  int alterEgoIndex = actor->alterEgoIndex;
  float radiusForAlterEgo = actor->radiusForAlterEgo;
  float mass = actor->mass;
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
	  __global Actor* otherActor = &(actorStates[otherActorIndex].constants);
	  float3 w = nextStates[otherActorIndex].position - position;
	  float r = alterEgoIndex == -1 || alterEgoIndex != otherActorIndex ? radius + otherActor->radius : radiusForAlterEgo + otherActor->radiusForAlterEgo;
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
      setIntersection(&actorState->intersections[intersectionCount], 1, i, cornerPtr[i] - positionPtr[i], 0.0f, -grid->normals[i], grid->normals[i] * radius);
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
      setIntersection(&actorState->intersections[intersectionCount], 1, i, positionPtr[pi] + cornerPtr[pi], 0.0f, -grid->normals[i], grid->normals[i] * radius);
      intersectionCount++;
      isFullOfIntersection = intersectionCount >= maxIntersection;
    }
  }

  float shellIntersectionLength = length(position) + radius - sphericalShellRadius;
  if (!isFullOfIntersection && shellIntersectionLength + smallValue > 0.0f) {
    setIntersection(&actorState->intersections[intersectionCount], 2, 0, shellIntersectionLength, 0.0f, normalize(position), -normalize(position) * radius);
    intersectionCount++;
    isFullOfIntersection = intersectionCount >= maxIntersection;
  }

  float momentOfInertia = (2.0f / 5.0f) * mass * radius * radius;
  actorState->momentOfInertia = momentOfInertia;
  float gravityTranslation = gravityAcceleration * deltaTime;
  float gravity = isFloating ? -gravityTranslation : 0.0f;
  actorState->linearVelocity = nextState->linearMomentum / mass + (float3)(0.0f, gravity, 0.0f);
  float ySpeed = actorState->linearVelocity.y;
  if (!isFloating && ySpeed * ySpeed < gravityTranslation * gravityTranslation * 16.0f) {
    actorState->linearVelocity.y = 0.0f;
  }
  actorState->angularVelocity = nextState->angularMomentum / momentOfInertia;
  actorState->isFloating = isFloating ? 1 : 0;

  actorState->intersectionCount = intersectionCount;
  actorState->collisionCount = 0;

  actorState->massForIntersection = mass / intersectionCount;
  actorState->massForCollision = mass;
}

void accumulatePenaltyImpulse(
  __global Intersection* intersection,
  const float deltaTime,
  float3* acc
) {
  *acc += (-64.0f * log2(intersection->length + 1.0f) * deltaTime) * intersection->normal;
}

__kernel void updateByPenaltyImpulse(
  __global const Actor* actors,
  __global ActorState* actorStates,
  const float deltaTime
) {
  size_t actorIndex = get_global_id(0);
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->intersectionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = actorState->intersections;
  float3 impulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    accumulatePenaltyImpulse(&intersections[i], deltaTime, &impulse);
  }
  actorState->linearVelocity += impulse / actors[actorIndex].mass;
}

void accumulateFrictionalImpulse(
  __global const Actor* actor,
  __global ActorState* actorState,
  __global const Actor* actors,
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
  float3 workingPoint = actor->radius * intersectionNormal;
  float3 workingPointVelocity = cross(actorState->angularVelocity, workingPoint);
  float3 otherWorkingPointVelocity = intersectionType == 0 ? cross(actorStates[otherIndex].angularVelocity, -actors[otherIndex].radius * intersectionNormal) : (float3)(0.0f);
  float3 relativeWorkingPointVelocity = otherWorkingPointVelocity - workingPointVelocity;
  float3 frictionalImpulse = (relativeVelocityOnSurface + relativeWorkingPointVelocity) / t;
  *linearMomentumAcc += frictionalImpulse;
  *angularMomentumAcc += cross(workingPoint, frictionalImpulse);
}

__kernel void updateByFrictionalImpulse(
  __global const Actor* actors,
  __global ActorState* actorStates
) {
  size_t actorIndex = get_global_id(0);
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->intersectionCount;
  if (count == 0) {
    return;
  }
  __global const Actor* actor = &actors[actorIndex];
  __global Intersection* intersections = actorState->intersections;
  float3 impulse = (float3)(0.0f);
  float3 angularImpulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    if (intersections[i].type == 3) {
      continue;
    }
    accumulateFrictionalImpulse(actor, actorState, actors, actorStates, &intersections[i], &impulse, &angularImpulse);
  }
  actorState->linearVelocity += impulse / actor->mass;
  actorState->angularVelocity += angularImpulse / actorState->momentOfInertia;
}

__kernel void collectCollisions(
  __global const Actor* actors,
  __global ActorState* actorStates
) {
  size_t actorIndex = get_global_id(0);
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->intersectionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = actorState->intersections;
  uchar collisionCount = 0;
  for (uchar i = 0; i < count; i++) {
    float3 relativeSpeed = intersections[i].type == 0 ? actorStates[intersections[i].otherIndex].linearVelocity - actorState->linearVelocity : - actorState->linearVelocity;
    intersections[i].speed = dot(intersections[i].normal, relativeSpeed);
    if (intersections[i].type != 3 && intersections[i].speed < 0.0f) {
      actorState->collisionIndices[collisionCount] = i;
      collisionCount++;
    }
  }
  actorState->collisionCount = collisionCount;
  actorState->massForCollision = actors[actorIndex].mass / collisionCount;
}

void accumulateConstraintImpulse(
  __global const Actor* actor,
  __global ActorState* actorState,
  __global const Actor* actors,
  __global ActorState* actorStates,
  __global Intersection* intersection,
  float3* acc
) {
  if (intersection->speed >= 0.0f) {
    return;
  }
  unsigned int intersectionType = intersection->type;
  ushort otherIndex = intersection->otherIndex;
  float mass = actorState->massForCollision;
  float elasticity = intersectionType == 0 ? actor->elasticity * actors[otherIndex].elasticity : actor->elasticity;
  float massRatio = intersectionType == 0 ? mass / actorStates[otherIndex].massForCollision : 0.0f;
  float3 intersectionNormal = intersection->normal;
  float speedOnIntersectionNormal = dot(actorState->linearVelocity, intersectionNormal);
  float affectedSpeed = (intersection->speed * (1.0f + elasticity) / (1.0f + massRatio)) + speedOnIntersectionNormal;
  *acc += (actorState->linearVelocity + (affectedSpeed - speedOnIntersectionNormal) * intersectionNormal) * mass;
}

__kernel void updateByConstraintImpulse(
  __global const Actor* actors,
  __global ActorState* actorStates
) {
  size_t actorIndex = get_global_id(0);
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->collisionCount;
  if (count == 0) {
    return;
  }
  __global Intersection* intersections = actorState->intersections;
  __global const Actor* actor = &actors[actorIndex];

  float3 impulse = (float3)(0.0f);
  for (uchar i = 0; i < count; i++) {
    if (intersections[actorState->collisionIndices[i]].type == 3) {
      continue;
    }
    accumulateConstraintImpulse(actor, actorState, actors, actorStates, &intersections[actorState->collisionIndices[i]], &impulse);
  }
  actorState->linearVelocity = impulse / actor->mass;
}

__kernel void inputFluid(
  __global FluidState* inputFluidStates,
  __global FluidState* fluidStates
) {
  size_t i = get_global_id(0);
  fluidStates[i] = inputFluidStates[i];
}

__kernel void updateDensityAndPressure(
  __global ActorState* actorStates,
  __global FluidState* fluidStates,
  __global Constants* constants
) {
  size_t fluidParticleIndex = get_global_id(0);
  ushort particleIndex = fluidParticleIndex + constants->rigidBodyParticleCount;
  __global FluidSettings* fluidSettings = &constants->fluidSettings;
  __global ActorState* actorState = &actorStates[particleIndex];
  __global FluidState* fluidState = &fluidStates[fluidParticleIndex];
  uchar count = actorState->intersectionCount;
  __global Intersection* intersections = actorState->intersections;
  float hh = fluidSettings->effectiveRadius * fluidSettings->effectiveRadius;
  float density = fluidSettings->particleMass * fluidSettings->poly6Constant * hh * hh * hh;
  for (uchar i = 0; i < count; i++) {
    float rr = intersections[i].distance * intersections[i].distance;
    float q = hh - rr;
    density += intersections[i].type == 3 ? fluidSettings->particleMass * fluidSettings->poly6Constant * q * q * q : 0.0f;
  }
  fluidStates[fluidParticleIndex].density = density;
  fluidStates[fluidParticleIndex].pressure = fluidSettings->stiffness * fluidSettings->density * (pow(density / fluidSettings->density, 2) - 1.0f);
}


__kernel void updateFluidForce(
  __global ActorState* actorStates,
  __global FluidState* fluidStates,
  __global Constants* constants
) {
  __global FluidSettings* fluidSettings = &constants->fluidSettings;
  float gravity = constants->gravityAcceleration;
  size_t fluidParticleIndex = get_global_id(0);
  ushort particleIndex = fluidParticleIndex + constants->rigidBodyParticleCount;
  __global ActorState* actorState = &actorStates[particleIndex];
  __global FluidState* fluidState = &fluidStates[fluidParticleIndex];
  uchar count = actorState->intersectionCount;
  __global Intersection* intersections = actorState->intersections;
  float density = fluidState->density;
  float3 velocity = fluidState->velocity;
  float3 force = (float3)(0.0f, -gravity * density, 0.0f);
  float pressurePart1 = -density * fluidSettings->particleMass;
  float pressurePart2 = (fluidState->pressure / (density * density));
  float viscosityPart1 = fluidSettings->viscosity * fluidSettings->particleMass;
  for (uchar i = 0; i < count; i++) {
    if (intersections[i].type == 3) {
      float q = intersections[i].length;
      ushort otherFluidParticleIndex = intersections[i].otherIndex - constants->rigidBodyParticleCount;
      float otherDensity = fluidStates[otherFluidParticleIndex].density;
      float otherPressure = fluidStates[otherFluidParticleIndex].pressure;
      float3 otherVelocity = fluidStates[otherFluidParticleIndex].velocity;
      force +=
	// pressure
	pressurePart1 * (pressurePart2 + (otherPressure / (otherDensity * otherDensity))) * fluidSettings->spikyGradientConstant * q * q * intersections[i].normal
	// viscosity
	+ viscosityPart1 * ((otherVelocity - velocity) / otherDensity) * fluidSettings->viscosityLaplacianConstant * q;
    } else {
      force += (-1024.0f * log2(intersections[i].length + 1.0f)) * intersections[i].normal;
    }
  }
  fluidState->force = force;
}

__kernel void moveFluid(
  __global FluidState* fluidStates,
  __global RigidBodyState* nextStates,
  __global Constants* constants
) {
  __global Grid* grid = &constants->grid;
  size_t fluidParticleIndex = get_global_id(0);
  ushort particleIndex = fluidParticleIndex + constants->rigidBodyParticleCount;
  __global FluidState* fluidState = &fluidStates[fluidParticleIndex];
  __global RigidBodyState* nextState = &nextStates[particleIndex];
  fluidState->velocity += (fluidState->force * constants->deltaTime) / fluidState->density;
  nextState->position += constants->deltaTime * fluidState->velocity;
  float3 corner = grid->origin + (float3)(0.0001f);
  nextState->position = clamp(nextState->position, corner, -corner);
  nextState->linearMomentum = fluidState->velocity * constants->fluidSettings.particleMass;
  nextState->angularMomentum = (float3)(0.0f);
}

__kernel void postProcessing(
  __global const Grid* grid,
  __global const Actor* actors,
  __global ActorState* actorStates,
  __global RigidBodyState* nextStates,
  const float deltaTime
) {
  size_t actorIndex = get_global_id(0);
  float3 maxLinearMomentum = (float3)((2.0f * (1.0f / deltaTime)) * actors[actorIndex].mass);
  __global RigidBodyState* state = &nextStates[actorIndex];
  state->angularMomentum = actorStates[actorIndex].angularVelocity * actorStates[actorIndex].momentOfInertia * 0.999f;
  state->linearMomentum = clamp(actorStates[actorIndex].linearVelocity * actors[actorIndex].mass * 0.999f, -maxLinearMomentum, maxLinearMomentum);

  if (length(state->linearMomentum) < 0.01f) {
    state->linearMomentum = (float3)(0.0f);
  }

  if (length(state->angularMomentum) < 0.01f) {
    state->angularMomentum = (float3)(0.0f);
  }

  float3 corner = grid->origin + (float3)(0.0001f);

  state->position = clamp(state->position, corner, -corner);
}

__kernel void calcSpringImpulses(
  __global ActorState* actorStates,
  __global const Spring* springs,
  __global SpringVar* springVars,
  __global RigidBodyState* nextStates,
  const float deltaTime
) {
  size_t i = get_global_id(0);
  __global const Spring* spring = &springs[i];
  __global SpringVar* springVar = &springVars[i];
  float3 pm0 = spring->nodePositionsModelSpace[0];
  float4 rot0 = nextStates[spring->actorIndices[0]].rotation;
  float3 pm1 = spring->nodePositionsModelSpace[1];
  float4 rot1 = nextStates[spring->actorIndices[1]].rotation;
  float3 p0 = rotateByQuat(pm0, rot0);
  float3 p1 = rotateByQuat(pm1, rot1);
  float3 impulse = ((p1 + nextStates[spring->actorIndices[1]].position) - (p0 + nextStates[spring->actorIndices[0]].position));
  float3 direction = normalize(impulse);
  float len = length(impulse);
  springVar->linearImpulses[0] = log2(1.0f + len) * deltaTime * spring->k * direction;
  springVar->linearImpulses[1] = -springVar->linearImpulses[0];
  springVar->angularImpulses[0] = cross(p0, springVar->linearImpulses[0]);
  springVar->angularImpulses[1] = cross(p1, springVar->linearImpulses[1]);
}

__kernel void updateBySpringImpulse(
  __global const Actor* actors,
  __global ActorState* actorStates,
  __global RigidBodyState* nextStates,
  __global SpringVar* springVars,
  const float deltaTime
) {
  size_t actorIndex = get_global_id(0);
  __global const Actor* actor = &actors[actorIndex];
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actor->springCount;
  float3 linearImpulse = (float3)(0.0f);
  float3 angularImpulse = (float3)(0.0f);

  for (uchar i = 0; i < count; i++) {
    linearImpulse += springVars[actor->springIndices[i]].linearImpulses[actor->springNodeIndices[i]];
    angularImpulse += springVars[actor->springIndices[i]].angularImpulses[actor->springNodeIndices[i]];;
  }

  actorState->linearVelocity += linearImpulse / actor->mass;
  actorState->angularVelocity += angularImpulse / actorState->momentOfInertia;
  nextStates[actorIndex].position += actorState->linearVelocity * deltaTime;
  nextStates[actorIndex].rotation = mulQuat(createQuatFromDisplacement(actorState->angularVelocity * deltaTime), nextStates[actorIndex].rotation);
}