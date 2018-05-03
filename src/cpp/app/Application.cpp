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
    services.renderingGroupFactory = new alcube::scripting::mappings::drawing::RenderingGroupFactory();
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
      services.renderingGroupFactory,
      entities.renderer,
      settings.physics,
      settings.world,
      settings.window,
      settings.root,
      services.cube
    };
  }

  Application* Application::instance;
  Application::Application(const char* programName) : mappings() {
    Application::instance = this;
    closingStatus = ApplicationClosingStatus::NONE;
    //fileUtil = new utils::FileUtil();
    //profiler = new utils::Profiler();
    di = new DI();
    evaluator = new scripting::Evaluator(di->get<utils::FileUtil>(), programName);
    evaluator->add(mappings.all);
  }

  void Application::run(const char* settingsFilePath, const char* mainFilePath) {
    evaluator->withScope([&]() {
      loadBasicLibraries();
      loadSettings(settingsFilePath);
      initServices(mainFilePath);
      di->get<models::Alcube>()->setUp();
      atexit(atexitCallback);
      std::thread(updateLoopCallback).detach();
      window->run();
      waitCloseAllThreads();
      closingStatus = ApplicationClosingStatus::FINISHED;
    });
  }

  void Application::waitCloseAllThreads() {
    while (closingStatus == NONE) {
      std::chrono::milliseconds intervalMs(100);
      std::this_thread::sleep_for(intervalMs);
    }
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

  void Application::initServices(const char* mainFilePath) {
    auto settings = di->get<models::Settings>();
    // Window
    window = new utils::app::OpenGLWindow([&]() { onDraw(); });
    window->setup(settings->window.width, settings->window.height, settings->fps, "alcube");

    // Drawing
    /*
    camera = new drawing::Camera(
      glm::vec3(0.0f, 0.0f, settings->world.size * 2.0f),
      glm::quat(),
      glm::radians(45.0f),
      (float)settings->window.width,
      (float)settings->window.height,
      0.1f, // near
      settings->world.size * 4.0f // far
    );
    canvas = new drawing::CanvasWithProfiler(camera, profiler);
    shaders = new drawing::shaders::Shaders(new utils::FileUtil(), canvas->context);

    // Grid
    grid = new Grid((unsigned int)settings->world.size);

    // Basic resources
    resourcesProvider = new utils::opencl::ResourcesProvider(fileUtil, new utils::opencl::Resources());
    gpuAccessor = new gpu::GPUAccessor(
      resourcesProvider,
      settings->world.maxActorCount,
      utils::math::powerOf2(settings->world.maxActorCount),
      settings->world.maxActorCount * 16,
      grid->xCount * grid->yCount * grid->zCount
    );

    // Simulators
    softBodySimulator = new physics::softbody::Simulator();
    fluidSimulator = new physics::fluid::Simulator();
    physicsSimulator = new physics::Simulator(
      settings->world.maxActorCount,
      grid->edgeLength,
      grid->xCount,
      grid->yCount,
      grid->zCount,
      settings->physics.timeStepSize,
      gpuAccessor
    );
     */
    di->get<physics::Simulator>()->gravity = settings->physics.gravity;

    // Cube
    /*
    renderer = new models::drawing::Renderer(gpuAccessor, canvas, resourcesProvider, settings->world.maxActorCount * 4);
    cube = new models::Alcube(
      fluidSimulator,
      softBodySimulator,
      physicsSimulator,
      renderer
    );
     */
    mappings.services.cube->setUnderlying(di->get<models::Alcube>());

    // Factories
    // model3DFactory = new models::drawing::Model3DFactory(new utils::MemoryPool<models::drawing::Model3D>(settings->world.maxActorCount));
    // renderingGroupFactory = new models::drawing::RenderingGroupFactory(new utils::MemoryPool<models::drawing::RenderingGroup>(settings->world.maxActorCount), shaders, settings);
    mappings.services.renderingGroupFactory->setUnderlying(di->get<models::drawing::RenderingGroupFactory>());

    // actorFactory = new models::ActorFactory(new utils::MemoryPool<models::Actor>(settings->world.maxActorCount), model3DFactory);
    mappings.services.actorFactory->setUnderlying(di->get<models::ActorFactory>());

    // springFactory = new models::physics::softbody::SpringFactory(new utils::MemoryPool<models::physics::softbody::Spring>(settings->world.maxActorCount));
    mappings.services.springFactory->setUnderlying(di->get<models::physics::softbody::SpringFactory>());

    // fluidFeaturesFactory = new models::physics::fluid::FeaturesFactory(new utils::MemoryPool<models::physics::fluid::Features>(settings->world.maxActorCount));
    mappings.services.fluidFeaturesFactory->setUnderlying(di->get<models::physics::fluid::FeaturesFactory>());

    //softbodyFeaturesFactory = new models::physics::softbody::FeaturesFactory(new utils::MemoryPool<models::physics::softbody::Features>(settings->world.maxActorCount));
    mappings.services.softbodyFeaturesFactory->setUnderlying(di->get<models::physics::softbody::FeaturesFactory>());

    // Load initial cube states
    evaluator->evaluate("../src/js/libs/init-services.js");
    evaluator->evaluate(mainFilePath);

    // Profiler
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

  void Application::updateLoopCallback() {
    while (!utils::app::OpenGLWindow::instance->isClosed()) {
      std::chrono::system_clock::time_point updateStartTime = std::chrono::system_clock::now();
      instance->onUpdate();
      std::chrono::system_clock::time_point updateEndTime = std::chrono::system_clock::now();
      int elapsedTime = (int) std::chrono::duration_cast<std::chrono::milliseconds>(updateEndTime - updateStartTime).count();
      auto nextFlameInterval = (int)(instance->di->get<models::Settings>()->physics.timeStepSize * 1000.0f - elapsedTime);
      if (nextFlameInterval > 0) {
        std::chrono::milliseconds intervalMs(nextFlameInterval);
        std::this_thread::sleep_for(intervalMs);
      }
    }
    instance->closingStatus = ApplicationClosingStatus::PROCESSING;
  }

  void Application::onClose() {
    di->get<utils::opencl::ResourcesProvider>()->resources->release();
    glfwTerminate();
  }

  void Application::atexitCallback() {
    instance->onClose();
  }
}
