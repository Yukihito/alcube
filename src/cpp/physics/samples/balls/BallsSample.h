#ifndef ALCUBE_BALLSSAMPLE_H
#define ALCUBE_BALLSSAMPLE_H

#include "../../../drawing/Drawable.h"
#include "../../SoftBodyActor.h"
#include "../../Simulator.h"
#include "../../../drawing/Canvas.h"
#include "../../../drawing/shapes/Shapes.h"
#include "../../../drawing/shaders/Shaders.h"
#include "../../../utils/app/OpenGLWindow.h"
#include "../../../utils/Profiler.h"
#include "../ApplicationBase.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/transform.hpp>

namespace alcube::physics::samples::balls {
  class Ball : public drawing::Drawable, public SoftBodyParticle {
    public:
      Ball(drawing::shapes::Shapes* shapes, drawing::shaders::Shaders* shaders);
//    protected:
//      glm::mat4 getModelMat() override;
  };

  class BallsSample : public ApplicationBase {
    public:
      explicit BallsSample();
    protected:
      void onInit() override;
    private:
      void add(Ball* ball);
  };
}

#endif //ALCUBE_BALLSSAMPLE_H