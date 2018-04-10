#ifndef ALCUBE_SAMPLES_APPLICATIONBASE_H
#define ALCUBE_SAMPLES_APPLICATIONBASE_H
#include "../utils/app/OpenGLApplication.h"
#include "../gpu/GPUAccessor.h"
#include "../drawing/Drawer.h"
#include "../drawing/shaders/Shaders.h"
#include "../utils/Profiler.h"
#include "../physics/Simulator.h"
#include "../drawing/DrawerWithProfiler.h"
#include "../physics/fluid/Simulator.h"
#include "../physics/softbody/Simulator.h"
#include "../models/Alcube.h"
#include "../scripting/Evaluator.h"
#include "../models/Settings.h"


namespace alcube::samples {
  class Profilers {
    public:
      int update;
      int all;
      int updateDrawable;
  };

  class ApplicationBase : public utils::app::OpenGLApplication {
    public:
      explicit ApplicationBase(
        models::Settings* settings,
        const std::string &appName,
        const char* programName
      );

    protected:
      void onDraw() override;

      void onUpdate() override;

      void onClose() override;
      models::Settings* settings;
      gpu::GPUAccessor* gpuAccessor;
      drawing::Drawer *drawer;
      drawing::Camera *camera;
      drawing::shaders::Shaders *shaders;
      utils::opencl::ResourcesProvider *resourcesProvider;
      utils::FileUtil *fileUtil;
      utils::Profiler *profiler;
      physics::Simulator *physicsSimulator;
      physics::softbody::Simulator* softBodySimulator;
      physics::fluid::Simulator* fluidSimulator;
      models::Alcube* cube;
      models::ActorFactory* actorFactory;
      models::physics::softbody::SpringFactory* springFactory;
      models::physics::fluid::FeaturesFactory* fluidFeaturesFactory;
      models::physics::softbody::FeaturesFactory* softbodyFeaturesFactory;
      scripting::Evaluator* evaluator;
      Profilers profilers;
      const char* programName;

      void beforeSetup();

      void afterSetup();
  };
}

#endif //ALCUBE_SAMPLES_APPLICATIONBASE_H
