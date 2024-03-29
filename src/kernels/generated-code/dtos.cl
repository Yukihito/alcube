
typedef struct __attribute__ ((packed)) ActorStruct {
  ushort type;
  char _padding0[2];
  ushort subIndex;
  char _padding1[2];
  float radius;
  float mass;
  float3 position;
  float4 rotation;
  float3 linearMomentum;
  float3 angularMomentum;
  int isAlive;
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

typedef struct __attribute__ ((packed)) RendererStruct {
  ushort actorIndex;
  char _padding0[2];
  int refersToRotations;
  uint instanceColorType;
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
