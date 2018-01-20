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
#include "Profilers.h"

namespace alcube::physics::samples::balls {
  class SoftBodySample : public utils::app::OpenGLApplication {
    public:
      explicit SoftBodySample();
    protected:
      void onInit() override;

      void onDraw() override;

      void onUpdate() override;

      void onClose() override;

    private:
      drawing::Drawer *drawer;
      drawing::Camera *camera;
      drawing::shaders::Shaders *shaders;
      drawing::shapes::Shapes *shapes;
      utils::opencl::Resources *resources;
      utils::FileUtil *fileUtil;
      utils::Profiler *profiler;
      Simulator *physicsSimulator;
      std::mutex mutex;
      Profilers profilers;
      unsigned int maxCellCount;
      float deltaTime;

      void add(ColorBall *ball);
  };
}

#endif //ALCUBE_SOFTBODYSAMPLE_H
