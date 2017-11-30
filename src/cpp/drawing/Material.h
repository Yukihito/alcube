#ifndef ALCUBE_MATERIAL_H
#define ALCUBE_MATERIAL_H
#include <glm/vec3.hpp>

namespace alcube::drawing {
  class Material {
    public:
      glm::vec3 diffuse;
      glm::vec3 ambient;
      glm::vec3 specular;
  };
}

#endif //ALCUBE_MATERIAL_H
