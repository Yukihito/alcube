#ifndef ALCUBE_GRID_H
#define ALCUBE_GRID_H
#include "../gpu/dtos.h"

namespace alcube::physics {
  struct Grid {
    unsigned int xCount;
    unsigned int yCount;
    unsigned int zCount;
    unsigned int edgeLength;
    gpu::dtos::Grid dto;
    explicit Grid(unsigned int worldSize);
  };
}

#endif //ALCUBE_GRID_H
