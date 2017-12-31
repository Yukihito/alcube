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

  float3 linearVelocity = cellVars[cellIndex].linearVelocity;
  float3 linearMomentum = clamp(linearVelocity * mass, -maxLinearMomentum, maxLinearMomentum);
  float3 currentPosition = currentStates[cellIndex].position;
  float3 positionDiff = linearVelocity * deltaTime;

  float3 angularVelocity = cellVars[cellIndex].angularVelocity;
  float3 angularMomentum = angularVelocity * momentOfInertia;
  float4 currentRotation = currentStates[cellIndex].rotation;
  float3 rotationDiff = angularVelocity * deltaTime;
  float4 rotationDiffQuat = createQuatFromDisplacement(&rotationDiff);

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
  float3 corner = grid->origin + (float3)(0.0001f);
  float3 nextPosition = nextStates[cellIndex].position;

  nextStates[cellIndex].linearMomentum *= 0.999f;
  nextStates[cellIndex].angularMomentum *= 0.999f;
  if (length(nextStates[cellIndex].linearMomentum) < 0.01f) {
    nextStates[cellIndex].linearMomentum = (float3)(0.0f);
  }

  if (length(nextStates[cellIndex].angularMomentum) < 0.01f) {
    nextStates[cellIndex].angularMomentum = (float3)(0.0f);
  }

  nextStates[cellIndex].position = clamp(nextPosition, corner, -corner);
}
