#include "ApplicationBase.h"

namespace alcube::samples {
  ApplicationBase* ApplicationBase::instance;
  ApplicationBase::ApplicationBase(const char* programName) {
    this->programName = programName;
    ApplicationBase::instance = this;
    atexit(atexitCallback);
    closingStatus = ApplicationClosingStatus::NONE;
  }

  void ApplicationBase::run() {
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

    std::thread th = std::thread(updateLoopCallback);
    th.detach();
    window->run();
    while (closingStatus == NONE) {
      std::chrono::milliseconds intervalMs(1);
      std::this_thread::sleep_for(intervalMs);
    }
    std::cout << "application finished" << std::endl;
    closingStatus = ApplicationClosingStatus::FINISHED;
    window->clean();
    exit(0);
  }

  void ApplicationBase::initServices() {
    window = new utils::app::OpenGLWindow(/*[&]() {
      profiler->start(profilers.updateDrawable);
      gpuAccessor->memories.positions.setCount(physicsSimulator->actorCount);
      gpuAccessor->memories.positions.read(); // 不要な気がする
      profiler->stop(profilers.updateDrawable);
      drawer->draw();
    }*/draw);
    settings = new models::Settings();
    settings->physics.gravity *= 2.0f;
    settings->physics.timeStepSize = 1.0f / 60.0f;
    window->setup(settings->window.width, settings->window.height, settings->fps, "alcube");
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
    drawer = new drawing::DrawerWithProfiler(camera, profiler);
    shaders = new drawing::shaders::Shaders(new utils::FileUtil(), drawer->context);
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

    evaluator->withScope([](alcube::scripting::Evaluator* e) {
      e->evaluate("../src/js/test.js");
    });


    profiler->setShowInterval(1000);
    profiler->enabled = true;
    profilers.update = profiler->create("update");
    profilers.all = profiler->create("all");
    profilers.updateDrawable = profiler->create("updateDrawable");
    profiler->start(profilers.all);
  }

  void ApplicationBase::draw() {
    instance->profiler->start(instance->profilers.updateDrawable);
    instance->gpuAccessor->memories.positions.setCount(instance->physicsSimulator->actorCount);
    instance->gpuAccessor->memories.positions.read(); // 不要な気がする
    instance->profiler->stop(instance->profilers.updateDrawable);
    instance->drawer->draw();
  }

  void ApplicationBase::onUpdate() {
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

  void ApplicationBase::updateLoopCallback() {
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
    std::cout << "update loop finished" << std::endl;
  }

  void ApplicationBase::onClose() {
    std::cout << "begin onClose" << std::endl;
    resourcesProvider->resources->release();
    std::cout << "closed" << std::endl;
  }

  void ApplicationBase::atexitCallback() {
    std::cout << "begin atexit" << std::endl;
    instance->onClose();
    std::cout << "begin terminate" << std::endl;
    glfwTerminate();
    std::cout << "end terminate" << std::endl;
  }
}
