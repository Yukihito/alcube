#include "Grid.h"

namespace alcube::app {
  Grid::Grid(unsigned int worldSize) {
    edgeLength = 8;
    xCount = worldSize / edgeLength;
    yCount = worldSize / edgeLength;
    zCount = worldSize / edgeLength;
  }
}