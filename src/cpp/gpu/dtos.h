#ifndef ALCUBE_GPU_DTOS_H
#define ALCUBE_GPU_DTOS_H

#include <CL/cl.h>

namespace alcube::gpu::dtos {
#pragma pack(push, 1)
  class Actor {
    public:
      float radius;
      float mass;
      unsigned short type;
      char _padding0[2];
      float elasticity;
      float dynamicFrictionCoefficient;
      float staticFrictionCoefficient;
      unsigned int springIndices[16];
      unsigned char springNodeIndices[16];
      unsigned int springCount;
      int alterEgoIndex;
      float radiusForAlterEgo;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class FluidSettings {
    public:
      float stiffness;
      float density;
      float viscosity;
      float particleMass;
      float effectiveRadius;
      float poly6Constant;
      float spikyGradientConstant;
      float viscosityLaplacianConstant;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class FluidState {
    public:
      cl_float3 velocity;
      float pressure;
      float density;
      cl_float3 force;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class Grid {
    public:
      unsigned int xCount;
      unsigned int yCount;
      unsigned int zCount;
      unsigned int edgeLength;
      cl_float3 origin;
      cl_float3 normals[6];
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class GridAndActorRelation {
    public:
      unsigned int gridIndex;
      unsigned short actorIndex;
      char _padding0[2];
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class Intersection {
    public:
      unsigned short type;
      char _padding0[2];
      unsigned short otherIndex;
      char _padding1[2];
      float length;
      float speed;
      cl_float3 normal;
      cl_float3 relativePosition;
      float distance;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class RigidBodyState {
    public:
      cl_float3 position;
      cl_float4 rotation;
      cl_float3 linearMomentum;
      cl_float3 angularMomentum;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class Spring {
    public:
      float k;
      cl_float3 nodePositionsModelSpace[2];
      unsigned short actorIndices[2];
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class SpringVar {
    public:
      cl_float3 linearImpulses[2];
      cl_float3 angularImpulses[2];
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class ActorState {
    public:
      Actor constants;
      cl_float3 linearVelocity;
      cl_float3 angularVelocity;
      float momentOfInertia;
      float massForIntersection;
      float massForCollision;
      unsigned short intersectionCount;
      char _padding0[2];
      unsigned short collisionCount;
      char _padding1[2];
      int isFloating;
      unsigned char collisionIndices[32];
      Intersection intersections[32];
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class Constants {
    public:
      Grid grid;
      FluidSettings fluidSettings;
      float gravityAcceleration;
      float deltaTime;
      float splitDeltaTime;
      float sphericalShellRadius;
      unsigned short rigidBodyParticleCount;
      char _padding0[2];
      unsigned short fluidParticleCount;
      char _padding1[2];
  };
#pragma pack(pop)
}
#endif //ALCUBE_GPU_DTOS_H

