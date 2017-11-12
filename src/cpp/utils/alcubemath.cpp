#include "alcubemath.h"

namespace alcube::utils::math {
  unsigned int powerOf2(unsigned int n) {
    auto m = (unsigned int)ceil(log2(n));
    unsigned int r = 1;
    for (int i = 0; i < m; i++) {
      r *= 2;
    }
    return r;
  }
}