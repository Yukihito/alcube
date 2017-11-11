#include "Spring.h"

namespace alcube::physics::rigidbody {
  Spring::Spring(SpringsHolder *h0, glm::vec3 pos0, SpringsHolder *h1, glm::vec3 pos1, float k) {
    nodes[0].positionModelSpace = pos0;
    nodes[0].holder = h0;
    nodes[1].positionModelSpace = pos1;
    nodes[1].holder = h1;
    this->k = k;
    h0->springs.push_back(this);
    h1->springs.push_back(this);
  }

  SpringNode* Spring::getNode(SpringsHolder *holder) {
    if (nodes[0].holder == holder) {
      return &nodes[0];
    } else {
      return &nodes[1];
    }
  }

  SpringNode* Spring::getOtherNode(SpringsHolder *holder) {
    if (nodes[0].holder == holder) {
      return &nodes[1];
    } else {
      return &nodes[0];
    }
  }
}