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
      int isFloating;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class Intersection {
    public:
      unsigned int type;
      unsigned short index;
      unsigned short otherIndex;
      float intersectionLength;
      float intersectionSpeed;
      cl_float3 constraintImpulse;
      cl_float3 frictionalImpulse;
      cl_float3 angularFrictionalImpulse;
      cl_float3 penaltyImpulse;
      cl_float3 normal;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class IntersectionBlock {
    public:
      unsigned int cumulativeIntersectionCount;
      unsigned short intersectionCount;
      unsigned short collisionCount;
      unsigned char collisionIndices[16];
      Intersection intersections[16];
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class IntersectionRef {
    public:
      unsigned short blockIndex;
      unsigned short intersectionIndex;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class Spring {
    public:
      float k;
      cl_float3 startPositionModelSpace;
      cl_float3 endPositionModelSpace;
      cl_float3 endCellPosition;
      cl_float4 endCellRotation;
  };
#pragma pack(pop)
}
#endif //ALCUBE_DTOS_H
