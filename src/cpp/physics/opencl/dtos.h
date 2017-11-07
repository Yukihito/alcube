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
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class Cell {
    public:
      int id;
      float radius;
      float weight;
      cl_float3 position;
      cl_float4 rotation;
      cl_float3 linearMomentum;
      cl_float3 angularMomentum;
      unsigned int springStartIndex;
      unsigned int springEndIndex;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  class CellUpdate {
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
      cl_float3 startPositionModelSpace;
      cl_float3 endPositionModelSpace;
      cl_float3 endCellPosition;
      cl_float4 endCellRotation;
  };
#pragma pack(pop)

}
#endif //ALCUBE_DTOS_H
