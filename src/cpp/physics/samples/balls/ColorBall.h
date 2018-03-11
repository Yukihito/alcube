#ifndef ALCUBE_COLORBALL_H
#define ALCUBE_COLORBALL_H

#include "../../../drawing/Material.h"
#include "../../SoftBodyActor.h"
#include "../../../drawing/shapes/Shapes.h"
#include "../../../drawing/shaders/Shaders.h"
#include "../../../drawing/Drawable.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/transform.hpp>

namespace alcube::physics::samples::balls {

  class ColorBall : public drawing::Drawable, public SoftBodyParticle {
    public:
      ColorBall(drawing::shapes::Shapes *shapes, drawing::shaders::Shaders *shaders, glm::vec3 diffuse);
    private:
      drawing::Material* material;
  };
}

#endif //ALCUBE_COLORBALL_H
