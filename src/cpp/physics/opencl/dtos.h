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
      // 32
      cl_float3 linearVelocity; // 4 * 3 = 12
      cl_float3 angularVelocity; // 4 * 3 = 12
      float momentOfInertia; // 4
      float splitMass; // 4

      // 4
      unsigned char cellIntersectionCount; // 1
      unsigned char cellCollisionCount; // 1
      unsigned char planeIntersectionCount; // 1
      unsigned char planeCollisionCount; // 1

      // 320
      unsigned short intersectedCellIndices[16]; // 2 * 16 = 32
      unsigned short collisionCellIndices[16]; // 2 * 16 = 32
      float  cellCollisionSpeeds[16]; // 4 * 16 = 64
      cl_float3 cellIntersectionNormals[16]; // 4 * 3 * 16 = 192

      // 32
      unsigned short intersectedPlaneIndices[4]; // 2 * 4 = 8
      unsigned short collisionPlaneIndices[4]; // 2 * 4 = 8
      float  planeCollisionSpeeds[4]; // 4 * 4 = 16
      int isFloating;
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
