#ifndef ALCUBE_PHYSICS_OPENCL_DTOS_H
#define ALCUBE_PHYSICS_OPENCL_DTOS_H

#include <CL/cl.h>

namespace alcube::physics::opencl::dtos {
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
  class GridAndCellRelation {
    public:
      unsigned int gridIndex;
      unsigned short cellIndex;
      short padding;
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
  class Cell { // collider
    public:
      // common
      float radius;
      float mass;
      unsigned short type;
      short padding;
      // only rigid body
      float elasticity;
      float dynamicFrictionCoefficient;
      float staticFrictionCoefficient;
      unsigned int springIndices[16];
      unsigned char springNodeIndices[16];
      unsigned int springCount;
      int alterEgoIndex;
      float radiusForAlterEgo;
      // only fluid
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class Intersection {
    public:
      unsigned short type;
      unsigned short otherIndex;
      float length;
      float speed;
      cl_float3 normal;
      cl_float3 relativePosition;
      float distance;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class CellVar {
    public:
      Cell constants;
      cl_float3 linearVelocity;
      cl_float3 angularVelocity;
      float momentOfInertia;
      float massForIntersection;
      float massForCollision;
      unsigned short intersectionCount;
      unsigned short collisionCount;
      int isFloating;
      unsigned char collisionIndices[32];
      Intersection intersections[32];
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class Spring {
    public:
      float k;
      cl_float3 nodePositionsModelSpace[2];
      unsigned short cellIndices[2];
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
  class FluidState {
    public:
      cl_float3 velocity;
      float pressure;
      float density;
      cl_float3 force;
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
  class Constants {
    public:
      Grid grid;
      FluidSettings fluidSettings;
      float gravityAcceleration;
      float deltaTime;
      float splitDeltaTime;
      float sphericalShellRadius;
      unsigned short rigidBodyParticleCount;
      unsigned short fluidParticleCount;
  };
#pragma pack(pop)

}
#endif //ALCUBE_PHYSICS_OPENCL_DTOS_H
