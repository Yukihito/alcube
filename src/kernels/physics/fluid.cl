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
