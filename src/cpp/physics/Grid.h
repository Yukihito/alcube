#ifndef ALCUBE_GRID_H
#define ALCUBE_GRID_H

namespace alcube::physics {
  struct Grid {
    unsigned int xCount;
    unsigned int yCount;
    unsigned int zCount;
    unsigned int edgeLength;
    explicit Grid(unsigned int worldSize);
  };
}

#endif //ALCUBE_GRID_H
