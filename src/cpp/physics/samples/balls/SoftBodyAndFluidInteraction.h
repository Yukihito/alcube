#ifndef ALCUBE_SOFTBODYANDFLUIDINTERACTION_H
#define ALCUBE_SOFTBODYANDFLUIDINTERACTION_H
#include "../../../drawing/Drawable.h"
#include "../../SoftBodyActor.h"
#include "../../Simulator.h"
#include "../../../drawing/Drawer.h"
#include "../../../drawing/shaders/Shaders.h"
#include "../../../utils/app/OpenGLApplication.h"
#include "../../../utils/Profiler.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/transform.hpp>
#include "../ApplicationBase.h"
#include "ColorBalls.h"

namespace alcube::physics::samples::balls {
  class SoftBodyAndFluidInteraction : public ApplicationBase {
    public:
      explicit SoftBodyAndFluidInteraction();
    protected:
      void onInit() override;
    private:
      ColorBalls* colorBalls;
      unsigned short actorCount;
      void add(SoftBodyParticle *ball);
      void add(FluidParticle *particle);
  };
}

#endif //ALCUBE_SOFTBODYANDFLUIDINTERACTION_H
