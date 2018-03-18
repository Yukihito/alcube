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
}
