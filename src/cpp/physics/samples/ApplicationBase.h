#ifndef ALCUBE_PHYSICS_SAMPLE_BASEAPPLICATION_H
#define ALCUBE_PHYSICS_SAMPLE_BASEAPPLICATION_H

#include "../../utils/app/OpenGLApplication.h"
#include "../Simulator.h"
#include "../../utils/Profiler.h"
#include "../../drawing/shaders/Shaders.h"
#include "../../drawing/Camera.h"
#include "../../drawing/Drawer.h"


namespace alcube::physics::samples {
  class Profilers {
    public:
      int update;
      int draw;
      int all;
      int updateDrawable;
      int drawActors;
  };

  class ApplicationBase : public utils::app::OpenGLApplication {
    public:
      explicit ApplicationBase(
        unsigned int windowWidth,
        unsigned int windowHeight,
        unsigned int fps,
        const std::string &appName
      );

    protected:
      void onDraw() override;

      void onUpdate() override;

      void onClose() override;
      gpu::GPU* gpu;
      drawing::Drawer *drawer;
      drawing::Camera *camera;
      drawing::shaders::Shaders *shaders;
      utils::opencl::ResourcesProvider *resourcesProvider;
      utils::FileUtil *fileUtil;
      utils::Profiler *profiler;
      Simulator *physicsSimulator;
      Profilers profilers;
      unsigned int maxCellCount;
      float deltaTime;
      bool initialized;

      void initBase(
        unsigned int worldSize,
        unsigned int maxCellCount
      );
  };
}

#endif //ALCUBE_PHYSICS_SAMPLE_BASEAPPLICATION_H
