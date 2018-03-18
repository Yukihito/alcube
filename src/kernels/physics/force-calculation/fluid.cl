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
    bool isOtherDynamic = intersections[i].type == ACTOR_TYPE_FLUID || intersections[i].type == ACTOR_TYPE_RIGID_BODY;
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
