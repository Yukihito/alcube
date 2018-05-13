#include "Grid.h"

namespace alcube::physics {
  Grid::Grid(unsigned int worldSize) {
    edgeLength = 8;
    xCount = worldSize / edgeLength;
    yCount = worldSize / edgeLength;
    zCount = worldSize / edgeLength;

    dto.edgeLength = edgeLength;
    dto.xCount = xCount;
    dto.yCount = yCount;
    dto.zCount = zCount;
    dto.origin = {
      -(dto.xCount * dto.edgeLength / 2.0f),
      -(dto.yCount * dto.edgeLength / 2.0f),
      -(dto.zCount * dto.edgeLength / 2.0f)
    };

    for (int i = 0; i < 3; i++) {
      dto.normals[i] = {0.0f, 0.0f, 0.0f};
      dto.normals[i].s[i] = 1.0f;
    }

    for (int i = 3; i < 6; i++) {
      dto.normals[i] = {0.0f, 0.0f, 0.0f};
      dto.normals[i].s[i - 3] = -1.0f;
    }
  }
}
