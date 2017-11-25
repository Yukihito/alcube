#ifndef ALCUBE_BALLSSAMPLE_H
#define ALCUBE_BALLSSAMPLE_H

#include "../../../drawing/Drawable.h"
#include "../../Cell.h"
#include "../../Simulator.h"
#include "../../../drawing/Drawer.h"
#include "../../../drawing/shapes/Shapes.h"
#include "../../../drawing/shaders/Shaders.h"
#include "../../../utils/app/OpenGLApplication.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/transform.hpp>

namespace alcube::physics::samples::balls {
  class Ball : public drawing::Drawable, public Cell {
      Ball(drawing::shapes::Shapes* shapes, drawing::shaders::Shaders* shaders);
      glm::mat4 getModelMat() override;
  };

  class BallsSample : public utils::app::OpenGLApplication {
    protected:
      void initWindowParams() override;
      void onInit() override;
      void onDraw() override;
      void onUpdate() override;
      void onClose() override;
    private:
      drawing::Drawer* drawer;
      drawing::Camera* camera;
      Simulator physicsSimulator;
      std::mutex mutex;
  };
}


#endif //ALCUBE_BALLSSAMPLE_H
