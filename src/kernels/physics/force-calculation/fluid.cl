__kernel void updateDensityAndPressure(
  __global ActorState* actorStates,
  __global FluidState* fluidStates,
  __global Constants* constants
) {
  size_t fluidParticleIndex = get_global_id(0);
  __global FluidState* fluidState = &fluidStates[fluidParticleIndex];
  ushort actorIndex = fluidState->actorIndex;
  __global FluidSettings* fluidSettings = &constants->fluidSettings;
  __global ActorState* actorState = &actorStates[actorIndex];

  uchar count = actorState->intersectionCount;
  __global Intersection* intersections = actorState->intersections;
  float hh = fluidSettings->effectiveRadius * fluidSettings->effectiveRadius;
  float density = fluidSettings->particleMass * fluidSettings->poly6Constant * hh * hh * hh;
  for (uchar i = 0; i < count; i++) {
    float rr = intersections[i].distance * intersections[i].distance;
    float q = hh - rr;
    density += /*intersections[i].type == ACTOR_TYPE_FLUID || intersections[i].type == ACTOR_TYPE_RIGID_BODY ? */fluidSettings->particleMass * fluidSettings->poly6Constant * q * q * q/* : 0.0f*/;
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
  size_t fluidParticleIndex = get_global_id(0);
  __global FluidState* fluidState = &fluidStates[fluidParticleIndex];
  ushort actorIndex = fluidState->actorIndex;
  __global ActorState* actorState = &actorStates[actorIndex];
  uchar count = actorState->intersectionCount;
  __global Intersection* intersections = actorState->intersections;
  float density = fluidState->density;
  float3 velocity = actorState->linearVelocity;
  float3 force = (float3)(0.0f, 0.0f, 0.0f);
  float pressurePart1 = -density * fluidSettings->particleMass;
  float pressurePart2 = (fluidState->pressure / (density * density));
  float viscosityPart1 = fluidSettings->viscosity * fluidSettings->particleMass;
  for (uchar i = 0; i < count; i++) {
    bool isOtherDynamic = intersections[i].type == ACTOR_TYPE_FLUID || intersections[i].type == ACTOR_TYPE_RIGID_BODY;
    //if (isOtherDynamic) {
      float q = intersections[i].length;
      ushort otherFluidParticleIndex = actorStates[intersections[i].otherIndex].constants.subPhysicalQuantityIndex;
      float otherDensity = intersections[i].type == ACTOR_TYPE_FLUID ? fluidStates[otherFluidParticleIndex].density : density;
      float otherPressure = intersections[i].type == ACTOR_TYPE_FLUID ? fluidStates[otherFluidParticleIndex].pressure : fluidState->pressure;
      float3 otherVelocity = isOtherDynamic ? actorStates[intersections[i].otherIndex].linearVelocity : 0.0f;
      force +=
	// pressure
	pressurePart1 * (pressurePart2 + (otherPressure / (otherDensity * otherDensity))) * fluidSettings->spikyGradientConstant * q * q * intersections[i].normal
	// viscosity
	+ viscosityPart1 * ((otherVelocity - velocity) / otherDensity) * fluidSettings->viscosityLaplacianConstant * q;
      //}/* else if (intersections[i].type == ACTOR_TYPE_RIGID_BODY) {
      //force += (-1024.0f * log2(intersections[i].length + 1.0f)) * intersections[i].normal;
      //}*/
  }
  fluidState->force = force;
}
