#include "Application.h"

namespace alcube::samples {
  Application* Application::instance;
  Application::Application(const char* programName) {
    this->programName = programName;
    Application::instance = this;
    closingStatus = ApplicationClosingStatus::NONE;
  }

  void Application::run() {
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
  }

  void Application::initServices() {
    settings = new models::Settings();
    settings->physics.gravity *= 2.0f;
    settings->physics.timeStepSize = 1.0f / 60.0f;
    settings->fps = 30;
    window = new utils::app::OpenGLWindow([&]() {onDraw();});
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
    fileUtil = new utils::FileUtil();

    resourcesProvider = new utils::opencl::ResourcesProvider(fileUtil, new utils::opencl::Resources());
    gpuAccessor = new gpu::GPUAccessor(
      resourcesProvider,
      settings->world.maxActorCount,
      utils::math::powerOf2(settings->world.maxActorCount),
      settings->world.maxActorCount * 16,
      xGridCount * yGridCount * zGridCount
    );
    profiler = new utils::Profiler();
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

    evaluator = new scripting::Evaluator(actorFactory, fluidFeaturesFactory, springFactory, softbodyFeaturesFactory, settings, cube, fileUtil, programName);
    evaluator->withScope([](alcube::scripting::Evaluator* e) { e->evaluate("../src/js/test.js"); });
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
