#include "Application.h"

namespace alcube::samples {
  Mappings::Mappings() {
    settings.window = new alcube::scripting::mappings::WindowSettings();
    settings.physics = new alcube::scripting::mappings::PhysicsSettings();
    settings.world = new alcube::scripting::mappings::WorldSettings();
    settings.root = new alcube::scripting::mappings::Settings();

    services.actorFactory = new alcube::scripting::mappings::ActorFactory();
    services.softbodyFeaturesFactory = new alcube::scripting::mappings::physics::softbody::FeaturesFactory();
    services.fluidFeaturesFactory = new alcube::scripting::mappings::physics::fluid::FeaturesFactory();
    services.springFactory = new alcube::scripting::mappings::physics::softbody::SpringFactory();
    services.cube = new alcube::scripting::mappings::Alcube();

    entities.actor = new alcube::scripting::mappings::Actor();
    entities.fluidFeatures = new alcube::scripting::mappings::physics::fluid::Features();
    entities.softbodyFeatures = new alcube::scripting::mappings::physics::softbody::Features();
    entities.spring = new alcube::scripting::mappings::physics::softbody::Spring();

    all = {
      entities.actor,
      services.actorFactory,
      entities.fluidFeatures,
      services.fluidFeaturesFactory,
      entities.spring,
      services.springFactory,
      entities.softbodyFeatures,
      services.softbodyFeaturesFactory,
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
    fileUtil = new utils::FileUtil();
    profiler = new utils::Profiler();
    evaluator = new scripting::Evaluator(fileUtil, programName);
    evaluator->add(mappings.all);
  }

  void Application::run() {
    evaluator->withScope([&]() {
      initServices();
      glm::vec3 color = glm::vec3(0.4f, 0.4f, 1.0f);
      auto drawable = new SphereDrawable(
        shaders->directionalLight,
        color,
        settings->world.maxActorCount,
        (GLfloat*)gpuAccessor->memories.positions.dto
      );
      drawable->shape->instanceCount = cube->getActorCount();
      drawer->add(drawable);
      physicsSimulator->input();
      gpuAccessor->memories.positions.setCount(physicsSimulator->actorCount);
      atexit(atexitCallback);
      std::thread th = std::thread(updateLoopCallback);
      th.detach();
      window->run();
      while (closingStatus == NONE) {
        std::chrono::milliseconds intervalMs(100);
        std::this_thread::sleep_for(intervalMs);
      }
      closingStatus = ApplicationClosingStatus::FINISHED;
    });
  }

  void Application::initServices() {

    //settings->physics.gravity *= 2.0f;
    //settings->physics.timeStepSize = 1.0f / 60.0f;
    //settings->fps = 30;

    evaluator->evaluate("../src/js/three.min.js");
    settings = new models::Settings();
    mappings.settings.window->setUnderlying(&settings->window);
    mappings.settings.physics->setUnderlying(&settings->physics);
    mappings.settings.world->setUnderlying(&settings->world);
    mappings.settings.root->setUnderlying(settings);
    evaluator->evaluate("../src/js/init-settings.js");
    evaluator->evaluate("../src/js/settings.js");
    unsigned int gridEdgeLength = 8;
    unsigned int xGridCount = (unsigned int)settings->world.size / gridEdgeLength;
    unsigned int yGridCount = (unsigned int)settings->world.size / gridEdgeLength;
    unsigned int zGridCount = (unsigned int)settings->world.size / gridEdgeLength;
    float near = 0.1f;
    float far = gridEdgeLength * xGridCount * 4.0f;
    camera = new drawing::Camera(
      glm::vec3(0.0f, 0.0f, far / 2.0f),
      glm::quat(),
      glm::radians(45.0f),
      (float)settings->window.width,
      (float)settings->window.height,
      near,
      far
    );

    resourcesProvider = new utils::opencl::ResourcesProvider(fileUtil, new utils::opencl::Resources());
    gpuAccessor = new gpu::GPUAccessor(
      resourcesProvider,
      settings->world.maxActorCount,
      utils::math::powerOf2(settings->world.maxActorCount),
      settings->world.maxActorCount * 16,
      xGridCount * yGridCount * zGridCount
    );

    softBodySimulator = new physics::softbody::Simulator();
    fluidSimulator = new physics::fluid::Simulator();
    physicsSimulator = new physics::Simulator(
      settings->world.maxActorCount,
      gridEdgeLength,
      xGridCount,
      yGridCount,
      zGridCount,
      settings->physics.timeStepSize,
      gpuAccessor
    );
    cube = new models::Alcube(
      fluidSimulator,
      softBodySimulator,
      physicsSimulator
    );
    actorFactory = new models::ActorFactory(new utils::MemoryPool<models::Actor>(settings->world.maxActorCount));
    springFactory = new models::physics::softbody::SpringFactory(new utils::MemoryPool<models::physics::softbody::Spring>(settings->world.maxActorCount));
    physicsSimulator->gravity = settings->physics.gravity;

    fluidFeaturesFactory = new models::physics::fluid::FeaturesFactory(new utils::MemoryPool<models::physics::fluid::Features>(settings->world.maxActorCount));
    softbodyFeaturesFactory = new models::physics::softbody::FeaturesFactory(new utils::MemoryPool<models::physics::softbody::Features>(settings->world.maxActorCount));
    mappings.services.actorFactory->setUnderlying(actorFactory);
    mappings.services.softbodyFeaturesFactory->setUnderlying(softbodyFeaturesFactory);
    mappings.services.fluidFeaturesFactory->setUnderlying(fluidFeaturesFactory);
    mappings.services.springFactory->setUnderlying(springFactory);
    mappings.services.cube->setUnderlying(cube);

    evaluator->evaluate("../src/js/init-services.js");
    evaluator->evaluate("../src/js/test.js");

    window = new utils::app::OpenGLWindow([&]() { onDraw(); });
    window->setup(settings->window.width, settings->window.height, settings->fps, "alcube");
    drawer = new drawing::DrawerWithProfiler(camera, profiler);
    shaders = new drawing::shaders::Shaders(new utils::FileUtil(), drawer->context);
    profiler->setShowInterval(1000);
    profiler->enabled = true;
    profilers.update = profiler->create("update");
    profilers.all = profiler->create("all");
    profilers.updateDrawable = profiler->create("updateDrawable");
    profiler->start(profilers.all);
  }

  void Application::onDraw() {
    profiler->start(profilers.updateDrawable);
    gpuAccessor->memories.positions.setCount(physicsSimulator->actorCount);
    gpuAccessor->memories.positions.read();
    profiler->stop(profilers.updateDrawable);
    drawer->draw();
  }

  void Application::onUpdate() {
    profiler->start(profilers.update);
    physicsSimulator->update();
    gpuAccessor->kernels.outputPositions(
      physicsSimulator->actorCount,
      gpuAccessor->memories.positions,
      gpuAccessor->memories.physicalQuantities
    );
    clFinish(resourcesProvider->queue->queue);

    //physicsSimulator->output(); // TODO: delete
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
      auto nextFlameInterval = (int)(instance->settings->physics.timeStepSize * 1000.0f - elapsedTime);
      if (nextFlameInterval > 0) {
        std::chrono::milliseconds intervalMs(nextFlameInterval);
        std::this_thread::sleep_for(intervalMs);
      }
    }
    instance->closingStatus = ApplicationClosingStatus::PROCESSING;
  }

  void Application::onClose() {
    resourcesProvider->resources->release();
    glfwTerminate();
  }

  void Application::atexitCallback() {
    instance->onClose();
  }
}
