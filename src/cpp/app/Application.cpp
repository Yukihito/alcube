#include "Application.h"

namespace alcube::app {
  Mappings::Mappings() {
    settings.window = new alcube::scripting::mappings::WindowSettings();
    settings.physics = new alcube::scripting::mappings::PhysicsSettings();
    settings.world = new alcube::scripting::mappings::WorldSettings();
    settings.root = new alcube::scripting::mappings::Settings();

    services.actorFactory = new alcube::scripting::mappings::ActorFactory();
    services.softbodyFeaturesFactory = new alcube::scripting::mappings::physics::softbody::FeaturesFactory();
    services.fluidFeaturesFactory = new alcube::scripting::mappings::physics::fluid::FeaturesFactory();
    services.springFactory = new alcube::scripting::mappings::physics::softbody::SpringFactory();
    services.renderer = new alcube::scripting::mappings::drawing::Renderer();
    services.cube = new alcube::scripting::mappings::Alcube();

    entities.actor = new alcube::scripting::mappings::Actor();
    entities.fluidFeatures = new alcube::scripting::mappings::physics::fluid::Features();
    entities.softbodyFeatures = new alcube::scripting::mappings::physics::softbody::Features();
    entities.spring = new alcube::scripting::mappings::physics::softbody::Spring();
    entities.renderer = new alcube::scripting::mappings::drawing::RenderingGroup();

    all = {
      entities.actor,
      services.actorFactory,
      entities.fluidFeatures,
      services.fluidFeaturesFactory,
      entities.spring,
      services.springFactory,
      entities.softbodyFeatures,
      services.softbodyFeaturesFactory,
      services.renderer,
      entities.renderer,
      settings.physics,
      settings.world,
      settings.window,
      settings.root,
      services.cube
    };
  }

  Application::Application(const char* programName) : mappings() {
    di = new DI();
    evaluator = new scripting::Evaluator(di->get<utils::FileUtil>(), programName, mappings.all);
  }

  void Application::run(const char* settingsFilePath, const char* mainFilePath) {
    evaluator->initV8();
    evaluator->withScope([&]() {
      loadBasicLibraries();
      loadSettings(settingsFilePath);
      initWindow();
      initServices();
      evaluator->evaluate(mainFilePath);
    });
    di->get<models::Alcube>()->setUp();
    auto settings = di->get<models::Settings>();
    auto th = scheduler.scheduleAsync(
      settings->physics.timeStepSize * 1000.0f,
      [&]{ return window->shouldClose(); },
      [&]{
        onUpdate();
        window->getKeyboard()->update();
      }
    );
    scheduler.schedule(
      1000.0f / settings->fps,
      [&]{ return window->shouldClose(); },
      [&]{
        onDraw();
        window->update();
      }
    );
    window->close();
    th.join();
    onClose();
    //window->run();
    evaluator->finalizeV8();
  }

  void Application::loadBasicLibraries() {
    evaluator->evaluate("../src/js/ext/three.min.js");
  }

  void Application::loadSettings(const char* settingsFilePath) {
    auto settings = di->get<models::Settings>();
    mappings.settings.window->setUnderlying(&settings->window);
    mappings.settings.physics->setUnderlying(&settings->physics);
    mappings.settings.world->setUnderlying(&settings->world);
    mappings.settings.root->setUnderlying(settings);
    evaluator->evaluate("../src/js/libs/init-settings.js");
    evaluator->evaluate(settingsFilePath);
  }

  void Application::initWindow() {
    auto settings = di->get<models::Settings>();
    window = new utils::OpenGLWindow(
      settings->window.width,
      settings->window.height,
      "alcube"
    );
    window->getKeyboard()->addKeyDownEventHandler([&](int key){onKeyDown(key);});
  }

  void Application::initServices() {
    auto settings = di->get<models::Settings>();
    di->get<physics::Simulator>()->gravity = settings->physics.gravity;
    mappings.services.cube->setUnderlying(di->get<models::Alcube>());
    mappings.services.renderer->setUnderlying(di->get<models::drawing::Renderer>());
    mappings.services.actorFactory->setUnderlying(di->get<models::ActorFactory>());
    mappings.services.springFactory->setUnderlying(di->get<models::physics::softbody::SpringFactory>());
    mappings.services.fluidFeaturesFactory->setUnderlying(di->get<models::physics::fluid::FeaturesFactory>());
    mappings.services.softbodyFeaturesFactory->setUnderlying(di->get<models::physics::softbody::FeaturesFactory>());
    evaluator->evaluate("../src/js/libs/init-services.js");
    auto profiler = di->get<utils::Profiler>();
    profiler->setShowInterval(1000);
    profiler->enabled = true;
    profilers.update = profiler->create("update");
    profilers.all = profiler->create("all");
    profiler->start(profilers.all);
  }

  void Application::onDraw() {
    di->get<models::Alcube>()->render();
  }

  void Application::onUpdate() {
    auto profiler = di->get<utils::Profiler>();
    profiler->start(profilers.update);
    di->get<models::Alcube>()->update();
    profiler->stop(profilers.update);
    profiler->stop(profilers.all);
    profiler->update();
    profiler->start(profilers.all);
  }

  void Application::onKeyDown(int key) {
    if (key == 69) { // e
      evaluateScratch();
    }
  }

  void Application::onClose() {
    di->get<utils::opencl::ResourcesProvider>()->resources->release();
  }

  void Application::evaluateScratch() {
    evaluator->withScope([&]() {
      evaluator->evaluate("../src/js/scratch.js");
    });
  }
}
