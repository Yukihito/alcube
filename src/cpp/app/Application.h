#ifndef ALCUBE_APPLICATION_H
#define ALCUBE_APPLICATION_H
#include "../utils/app/OpenGLWindow.h"
#include "../gpu/GPUAccessor.h"
#include "../drawing/Canvas.h"
#include "../drawing/shaders/Shaders.h"
#include "../utils/Profiler.h"
#include "../physics/Simulator.h"
#include "../drawing/CanvasWithProfiler.h"
#include "../drawing/textures/CheckTexture.h"
#include "../physics/fluid/Simulator.h"
#include "../physics/softbody/Simulator.h"
#include "../models/Alcube.h"
#include "../scripting/Evaluator.h"
#include "../models/Settings.h"
#include "../models/drawing/SphereDrawable.h"
#include "../scripting/mappings/drawing/RenderingGroup.h"

namespace alcube::app {
  class Profilers {
    public:
      int update;
      int all;
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
    alcube::scripting::mappings::drawing::RenderingGroupFactory* renderingGroupFactory;
    alcube::scripting::mappings::Alcube* cube;
  };

  struct EntityMappings {
    alcube::scripting::mappings::Actor* actor;
    alcube::scripting::mappings::physics::fluid::Features* fluidFeatures;
    alcube::scripting::mappings::physics::softbody::Spring* spring;
    alcube::scripting::mappings::physics::softbody::Features* softbodyFeatures;
    alcube::scripting::mappings::drawing::RenderingGroup* renderer;
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
      void run(const char* settingsFilePath, const char* mainFilePath);
    private:
      ApplicationClosingStatus closingStatus;
      utils::app::OpenGLWindow* window;
      models::Settings* settings;
      gpu::GPUAccessor* gpuAccessor;
      drawing::Canvas *canvas;
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
      models::drawing::RenderingGroupFactory* renderingGroupFactory;
      models::drawing::Model3DFactory* model3DFactory;
      models::drawing::Renderer* renderer;
      scripting::Evaluator* evaluator;
      Profilers profilers;
      Mappings mappings;
      Grid* grid;

      void initServices(const char* mainFilePath);
      void onClose();
      void onUpdate();
      void onDraw();
      void waitCloseAllThreads();
      void loadBasicLibraries();
      void loadSettings(const char* settingsFilePath);
      static void atexitCallback();
      static void updateLoopCallback();
  };
}

#endif //ALCUBE_APPLICATION_H