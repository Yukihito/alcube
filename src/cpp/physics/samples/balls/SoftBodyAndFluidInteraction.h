#ifndef ALCUBE_SOFTBODYANDFLUIDINTERACTION_H
#define ALCUBE_SOFTBODYANDFLUIDINTERACTION_H
#include "../../../drawing/Drawable.h"
#include "../../SoftBodyParticle.h"
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
#include "../DrawableFluidParticle.h"

namespace alcube::physics::samples::balls {
  class SoftBodyAndFluidInteraction : public BaseApplication {
    public:
      explicit SoftBodyAndFluidInteraction();
    protected:
      void onInit() override;
    private:
      void add(ColorBall *ball);
      void add(DrawableFluidParticle *particle);
  };
}

#endif //ALCUBE_SOFTBODYANDFLUIDINTERACTION_H
