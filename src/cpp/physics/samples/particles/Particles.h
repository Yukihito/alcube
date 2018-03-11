#ifndef ALCUBE_PHYSICS_SAMPLE_PARTICLES_H
#define ALCUBE_PHYSICS_SAMPLE_PARTICLES_H


#include "../../../drawing/Drawable.h"
#include "../../../drawing/shapes/Shapes.h"
#include "../../../drawing/shaders/Shaders.h"
#include "../../SoftBodyActor.h"
#include "../../../drawing/Drawer.h"
#include "../../../utils/opencl/Resources.h"
#include "../../../utils/Profiler.h"
#include "../../Simulator.h"

namespace alcube::physics::samples::particles {
  class Particles : public drawing::Drawable {
    public:
      Particles(
        drawing::shapes::Shapes *shapes,
        drawing::shaders::Shaders *shaders,
        unsigned int maxParticleCount
      );

      GLfloat *positions;
      GLfloat *colors;

      void update(std::vector<SoftBodyParticle *> &cells);

    //protected:
//      glm::mat4 getModelMat() override;

    private:
      drawing::shapes::points::Particles *particlesShape;
  };
}
#endif // ALCUBE_PHYSICS_SAMPLE_PARTICLES_H
