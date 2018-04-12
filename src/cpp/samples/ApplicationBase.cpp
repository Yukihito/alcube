#include "ApplicationBase.h"

namespace alcube::samples {
  ApplicationBase::ApplicationBase(
    models::Settings* settings,
    const std::string& appName,
    const char* programName
  ) : OpenGLApplication(settings->window.width, settings->window.height, settings->fps, appName) {
    this->settings = settings;
    this->programName = programName;
  }

  void ApplicationBase::beforeSetup() {
    printSystemInfo();
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
      (float)windowWidth,
      (float)windowHeight,
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
    evaluator->withScope([](alcube::scripting::Evaluator* e) { e->evaluate("../src/js/test.js"); });
    profiler->setShowInterval(1000);
    profiler->enabled = true;
    profilers.update = profiler->create("update");
    profilers.all = profiler->create("all");
    profilers.updateDrawable = profiler->create("updateDrawable");
    profiler->start(profilers.all);
  }

  void ApplicationBase::afterSetup() {
    physicsSimulator->input();
    gpuAccessor->memories.positions.setCount(physicsSimulator->actorCount);
  }

  void ApplicationBase::onDraw() {
    profiler->start(profilers.updateDrawable);
    gpuAccessor->memories.positions.setCount(physicsSimulator->actorCount);
    gpuAccessor->memories.positions.read();
    profiler->stop(profilers.updateDrawable);
    drawer->draw();
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

  void ApplicationBase::onClose() {
    resourcesProvider->resources->release();
  }

}
