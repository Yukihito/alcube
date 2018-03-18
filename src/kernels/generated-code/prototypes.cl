float4 mulQuat(float4 q, float4 r);

float3 rotateByQuat(float3 v, float4 q);

float4 createQuatFromDisplacement(float3 angularDisplacement);

void setIntersection(__global Intersection* intersection, ushort type, ushort otherIndex, float intersectionLength, float speed, float3 normal, float3 relativePosition);

void accumulateConstraintImpulse(__global Actor* actor, __global ActorState* actorState, __global ActorState* actorStates, __global Intersection* intersection, __global SoftBodyState* softBodyStates, float3* acc);

void accumulateFrictionalImpulse(__global ActorState* actorState, __global ActorState* actorStates, __global Intersection* intersection, float3* linearMomentumAcc, float3* angularMomentumAcc);

void accumulatePenaltyImpulse(__global Intersection* intersection, const float deltaTime, float penaltyFactor, float3* acc);
