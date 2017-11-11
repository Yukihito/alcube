#ifndef ALCUBE_SPRING_H
#define ALCUBE_SPRING_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>


namespace alcube::physics::rigidbody {
  class Spring;

  class SpringsHolder {
    public:
      virtual glm::vec3 getPosition() = 0;
      virtual glm::quat getRotation() = 0;
      std::vector<Spring*> springs;
  };

  class SpringNode {
    public:
      SpringsHolder* holder;
      glm::vec3 positionModelSpace;
  };

  class Spring {
    public:
      SpringNode nodes[2];
      float k;
      SpringNode* getNode(SpringsHolder* holder);
      SpringNode* getOtherNode(SpringsHolder* holder);
      explicit Spring(SpringsHolder* h0, glm::vec3 pos0, SpringsHolder* h1, glm::vec3 pos1, float k);
  };
}


#endif //ALCUBE_SPRING_H
