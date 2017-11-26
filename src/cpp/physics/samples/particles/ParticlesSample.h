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

  class Profilers {
    public:
      int update;
      int all;
  };

  class ParticlesSample : public utils::app::OpenGLApplication {
    protected:
      void initWindowParams() override;
      void onInit() override;
      void onDraw() override;
      void onUpdate() override;
      void onClose() override;
    private:
      drawing::Drawer* drawer;
      drawing::Camera* camera;
      drawing::shaders::Shaders* shaders;
      drawing::shapes::Shapes* shapes;
      utils::opencl::Resources* resources;
      utils::FileUtil* fileUtil;
      utils::Profiler* profiler;
      Simulator* physicsSimulator;
      std::mutex mutex;
      Profilers profilers;
      std::vector<Cell*> cells;
      Particles* particles;
      unsigned int maxCellCount;
      float deltaTime;
  };
}

#endif //ALCUBE_PARTICLESSAMPLE_H
