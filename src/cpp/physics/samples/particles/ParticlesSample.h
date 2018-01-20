#ifndef ALCUBE_PARTICLESSAMPLE_H
#define ALCUBE_PARTICLESSAMPLE_H

#include "../../../drawing/Drawable.h"
#include "../../../drawing/shaders/Shaders.h"
#include "../../../drawing/shapes/Shapes.h"
#include "../../../utils/app/OpenGLApplication.h"
#include "../../../drawing/Drawer.h"
#include "../../../utils/opencl/Resources.h"
#include "../../../utils/Profiler.h"
#include "../../Simulator.h"
#include "../BaseApplication.h"

namespace alcube::physics::samples::particles {
  class Particles : public drawing::Drawable {
    public:
      Particles(
        drawing::shapes::Shapes* shapes,
        drawing::shaders::Shaders* shaders,
        unsigned int maxParticleCount
      );
      GLfloat* positions;
      GLfloat* colors;
      void update(std::vector<Cell*> &cells);
    protected:
      glm::mat4 getModelMat() override;
    private:
      drawing::shapes::points::Particles* particlesShape;
  };

  class ParticlesSample : public BaseApplication {
    public:
      explicit ParticlesSample();
    protected:
      void onInit() override;

      void onDraw() override;
    private:
      std::vector<Cell*> cells;
      Particles* particles;
  };
}

#endif //ALCUBE_PARTICLESSAMPLE_H
