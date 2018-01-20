#ifndef ALCUBE_SOFTBODYSAMPLE_H
#define ALCUBE_SOFTBODYSAMPLE_H

#include "../../../drawing/Drawable.h"
#include "../../Cell.h"
#include "../../Simulator.h"
#include "../../../drawing/Drawer.h"
#include "../../../drawing/shapes/Shapes.h"
#include "../../../drawing/shaders/Shaders.h"
#include "../../../utils/app/OpenGLApplication.h"
#include "../../../utils/Profiler.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/transform.hpp>
#include "ColorBall.h"
#include "../BaseApplication.h"

namespace alcube::physics::samples::balls {
  class SoftBodySample : public BaseApplication {
    public:
      explicit SoftBodySample();
    protected:
      void onInit() override;

    private:
      void add(ColorBall *ball);
  };
}

#endif //ALCUBE_SOFTBODYSAMPLE_H
