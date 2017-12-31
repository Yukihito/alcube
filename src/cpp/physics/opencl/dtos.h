#ifndef ALCUBE_DTOS_H
#define ALCUBE_DTOS_H

#include <OpenCL/opencl.h>

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
      unsigned int gridIndex;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class Cell {
    public:
      float radius;
      float mass;
      float elasticity;
      float dynamicFrictionCoefficient;
      float staticFrictionCoefficient;
      unsigned int springIndices[16];
      unsigned int springCount;
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
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class CellVar {
    public:
      cl_float3 linearVelocity;
      cl_float3 angularVelocity;
      float momentOfInertia;
      float massForIntersection;
      float massForCollision;
      unsigned short intersectionCount;
      unsigned short collisionCount;
      int isFloating;
      unsigned char collisionIndices[16];
      Intersection intersections[16];
      cl_float3 springLinearImpulse;
      cl_float3 springAngularImpulse;
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
}
#endif //ALCUBE_DTOS_H
