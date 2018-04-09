#include "ApplicationBase.h"

namespace alcube::samples {
  ApplicationBase::ApplicationBase(
    unsigned int windowWidth,
    unsigned int windowHeight,
    unsigned int fps,
    const std::string& appName,
    const char* programName
  ) : OpenGLApplication(windowWidth, windowHeight, fps, appName) {
    this->programName = programName;
  }

  void ApplicationBase::beforeSetup(unsigned int worldSize, unsigned int maxCellCount) {
    printSystemInfo();
    this->maxCellCount = maxCellCount;
    deltaTime = 1.0f / (float)fps;
    float gravity = 9.8f;
    unsigned int gridEdgeLength = 8;
    unsigned int xGridCount = worldSize / gridEdgeLength;
    unsigned int yGridCount = worldSize / gridEdgeLength;
    unsigned int zGridCount = worldSize / gridEdgeLength;
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
      maxCellCount,
      utils::math::powerOf2(maxCellCount),
      maxCellCount * 16,
      xGridCount * yGridCount * zGridCount
    );
    profiler = new utils::Profiler();
    drawer = new drawing::DrawerWithProfiler(camera, profiler);
    shaders = new drawing::shaders::Shaders(new utils::FileUtil(), drawer->context);
    softBodySimulator = new physics::softbody::Simulator();
    fluidSimulator = new physics::fluid::Simulator();
    physicsSimulator = new physics::Simulator(
      maxCellCount,
      gridEdgeLength,
      xGridCount,
      yGridCount,
      zGridCount,
      deltaTime,
      gpuAccessor
    );
    //physicsSimulator->add(softBodySimulator);
    //physicsSimulator->add();
    cube = new models::Alcube(
      fluidSimulator,
      softBodySimulator,
      physicsSimulator
    );
    actorFactory = new models::ActorFactory(new utils::MemoryPool<models::Actor>(65536));
    springFactory = new models::physics::softbody::SpringFactory(new utils::MemoryPool<models::physics::softbody::Spring>(65536));
    physicsSimulator->gravity = gravity;

    fluidFeaturesFactory = new models::physics::fluid::FeaturesFactory(new utils::MemoryPool<models::physics::fluid::Features>(65536));
    softbodyFeaturesFactory = new models::physics::softbody::FeaturesFactory(new utils::MemoryPool<models::physics::softbody::Features>(65536));

    evaluator = new scripting::Evaluator(actorFactory, fluidFeaturesFactory, springFactory, softbodyFeaturesFactory, cube, fileUtil, programName);
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
