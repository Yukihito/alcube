#ifndef ALCUBE_DRAWABLEFLUIDPARTICLE_H
#define ALCUBE_DRAWABLEFLUIDPARTICLE_H

#include "../../drawing/Drawable.h"
#include "../FluidParticle.h"
#include "../../drawing/shaders/Shaders.h"
#include "../../drawing/shapes/Shapes.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/transform.hpp>

namespace alcube::physics::samples {
  class DrawableFluidParticle : public drawing::Drawable, public FluidParticle {
    public:
      DrawableFluidParticle(drawing::shapes::Shapes *shapes, drawing::shaders::Shaders *shaders, glm::vec3 diffuse);
    protected:
      //glm::mat4 getModelMat() override;
      //drawing::Material* getMaterial() override;
  };
}

#endif //ALCUBE_DRAWABLEFLUIDPARTICLE_H
