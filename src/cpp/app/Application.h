#ifndef ALCUBE_APPLICATION_H
#define ALCUBE_APPLICATION_H
#include "../utils/OpenGLWindow.h"
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
#include "DI.h"
#include "../scripting/mappings/drawing/Renderer.h"

namespace alcube::app {
  class Profilers {
    public:
      int update;
      int all;
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
    alcube::scripting::mappings::drawing::Renderer* renderer;
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

  class Application {
    public:
      DI* di;
      explicit Application(const char* programName);
      void run(const char* settingsFilePath, const char* mainFilePath);
    private:
      utils::OpenGLWindow* window;
      Profilers profilers;
      scripting::Evaluator* evaluator;
      Mappings mappings;

      void initWindow();
      void initServices();
      void onDraw();
      void onUpdate();
      void onKeyDown(int key);
      void onClose();
      void evaluateScratch();
      void loadBasicLibraries();
      void loadSettings(const char* settingsFilePath);
  };
}

#endif //ALCUBE_APPLICATION_H
