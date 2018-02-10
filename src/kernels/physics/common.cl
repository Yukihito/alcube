#define PARTICLE_TYPE_RIGID_BODY      = 0
#define PARTICLE_TYPE_FACE            = 1
#define PARTICLE_TYPE_SPHERICAL_SHELL = 2
#define PARTICLE_TYPE_FLUID           = 3

typedef struct __attribute__ ((packed)) GridStruct {
  unsigned int xCount;
  unsigned int yCount;
  unsigned int zCount;
  unsigned int edgeLength;
  float3 origin;
  float3 normals[6];
} Grid;

typedef struct __attribute__ ((packed)) GridAndActorRelationStruct {
  unsigned int gridIndex;
  unsigned short actorIndex;
  short padding;
} GridAndActorRelation;

typedef struct __attribute__ ((packed)) RigidBodyStateStruct {
  float3 position;
  float4 rotation;
  float3 linearMomentum;
  float3 angularMomentum;
} RigidBodyState;

typedef struct __attribute__ ((packed)) ActorStruct {
  float radius;
  float mass;
  unsigned short type;
  short padding;
  float elasticity;
  float dynamicFrictionCoefficient;
  float staticFrictionCoefficient;
  uint springIndices[16];
  uchar springNodeIndices[16];
  uint springCount;
  int alterEgoIndex;
  float radiusForAlterEgo;
} Actor;

typedef struct __attribute__ ((packed)) IntersectionStruct {
  unsigned short type;
  unsigned short otherIndex;
  float length;
  float speed;
  float3 normal;
  float3 relativePosition;
  float distance;
} Intersection;

typedef struct __attribute__ ((packed)) ActorStateStruct {
  Actor constants;
  float3 linearVelocity;
  float3 angularVelocity;
  float momentOfInertia;
  float massForIntersection;
  float massForCollision;
  ushort intersectionCount;
  ushort collisionCount;
  int isFloating;
  unsigned char collisionIndices[32];
  Intersection intersections[32];
} ActorState;

typedef struct __attribute__ ((packed)) SpringStruct {
  float k;
  float3 nodePositionsModelSpace[2];
  ushort actorIndices[2];
} Spring;

typedef struct __attribute__ ((packed)) SpringVarStruct {
  float3 linearImpulses[2];
  float3 angularImpulses[2];
} SpringVar;

typedef struct __attribute__ ((packed)) FluidStateStruct {
  float3 velocity;
  float pressure;
  float density;
  float3 force;
} FluidState;

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

typedef struct __attribute__ ((packed)) ConstantsStruct {
  Grid grid;
  FluidSettings fluidSettings;
  float gravityAcceleration;
  float deltaTime;
  float splitDeltaTime;
  float sphericalShellRadius;
  unsigned short rigidBodyParticleCount;
  unsigned short fluidParticleCount;
} Constants;

float4 mulQuat(float4 q, float4 r);
float3 rotateByQuat(float3 v, float4 q);
float4 createQuatFromDisplacement(float3 angularDisplacement);

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
