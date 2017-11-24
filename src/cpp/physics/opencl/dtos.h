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
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class GridAndCellRelation {
    public:
      unsigned int gridIndex;
      unsigned short cellIndex;
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
      cl_float3 tmpPosition;
      unsigned int springStartIndex;
      unsigned int springEndIndex;
      bool collisionOccurred;
      unsigned short collisionCellIndex;
      float collisionTime;
      unsigned char neighborCellCount;
      unsigned short neighborCellIndices[16];
      unsigned char collisionType;
      unsigned char collisionWallAxis;
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
