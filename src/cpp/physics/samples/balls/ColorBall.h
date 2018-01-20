//
// Created by yukihito on 18/01/20.
//

#ifndef ALCUBE_COLORBALL_H
#define ALCUBE_COLORBALL_H

#include "../../../drawing/Material.h"
#include "../../Cell.h"
#include "../../../drawing/shapes/Shapes.h"
#include "../../../drawing/shaders/Shaders.h"
#include "../../../drawing/Drawable.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/transform.hpp>

namespace alcube::physics::samples::balls {

  class ColorBall : public drawing::Drawable, public Cell {
    public:
      ColorBall(drawing::shapes::Shapes *shapes, drawing::shaders::Shaders *shaders, glm::vec3 diffuse, bool showsLinearMomentum);

    protected:
      glm::mat4 getModelMat() override;
      drawing::Material* getMaterial() override;
    private:
      drawing::Material* material;
      bool showsLinearMomentum;
  };
}

#endif //ALCUBE_COLORBALL_H
