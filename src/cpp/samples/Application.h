#ifndef ALCUBE_SAMPLES_APPLICATIONBASE_H
#define ALCUBE_SAMPLES_APPLICATIONBASE_H
#include "../utils/app/OpenGLWindow.h"
#include "../gpu/GPUAccessor.h"
#include "../drawing/Drawer.h"
#include "../drawing/shaders/Shaders.h"
#include "../utils/Profiler.h"
#include "../physics/Simulator.h"
#include "../drawing/DrawerWithProfiler.h"
#include "../drawing/textures/CheckTexture.h"
#include "../physics/fluid/Simulator.h"
#include "../physics/softbody/Simulator.h"
#include "../models/Alcube.h"
#include "../scripting/Evaluator.h"
#include "../models/Settings.h"
#include "../models/drawing/SphereDrawable.h"
#include "../scripting/mappings/drawing/Renderer.h"

namespace alcube::samples {
  class Profilers {
    public:
      int update;
      int all;
      int updateDrawable;
  };

  enum ApplicationClosingStatus {
    NONE,
    PROCESSING,
    FINISHED
  };

  struct SettingMappings {
    alcube::scripting::mappings::PhysicsSettings* physics;
    alcube::scripting::mappings::WorldSettings* world;
    alcube::scripting::mappings::WindowSettings* window;
    alcube::scripting::mappings::Settings* root;
  };

  struct ServiceMappings {
    alcube::scripting::mappings::ActorFactory* actorFactory;
    alcube::scripting::mappings::physics::fluid::FeaturesFactory* fluidFeaturesFactory;
    alcube::scripting::mappings::physics::softbody::SpringFactory* springFactory;
    alcube::scripting::mappings::physics::softbody::FeaturesFactory* softbodyFeaturesFactory;
    alcube::scripting::mappings::drawing::RendererFactory* rendererFactory;
    alcube::scripting::mappings::Alcube* cube;
  };

  struct EntityMappings {
    alcube::scripting::mappings::Actor* actor;
    alcube::scripting::mappings::physics::fluid::Features* fluidFeatures;
    alcube::scripting::mappings::physics::softbody::Spring* spring;
    alcube::scripting::mappings::physics::softbody::Features* softbodyFeatures;
    alcube::scripting::mappings::drawing::Renderer* renderer;
  };

  struct Mappings {
    SettingMappings settings;
    ServiceMappings services;
    EntityMappings entities;
    std::vector<alcube::scripting::utils::Prototype*> all;
    explicit Mappings();
  };

  struct Grid {
    unsigned int xCount;
    unsigned int yCount;
    unsigned int zCount;
    unsigned int edgeLength;
    explicit Grid(unsigned int worldSize);
  };

  class Application {
    public:
      static Application* instance;
      explicit Application(const char* programName);
      void run();
    private:
      ApplicationClosingStatus closingStatus;
      utils::app::OpenGLWindow* window;
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
      models::drawing::RendererFactory* rendererFactory;
      models::drawing::InstanceRendererFactory* instanceRendererFactory;
      scripting::Evaluator* evaluator;
      Profilers profilers;
      Mappings mappings;
      Grid* grid;

      void initServices();
      void onClose();
      void onUpdate();
      void onDraw();
      void waitCloseAllThreads();
      void loadBasicLibraries();
      void loadSettings();
      static void atexitCallback();
      static void updateLoopCallback();
  };
}

#endif //ALCUBE_SAMPLES_APPLICATIONBASE_H
