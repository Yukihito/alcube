#include "DI.h"

namespace alcube::app {
  template <>
  utils::FileUtil* DI::inject() {
    return new utils::FileUtil();
  }

  template <>
  utils::Profiler* DI::inject() {
    return new utils::Profiler();
  }

  template <>
  models::Settings* DI::inject() {
    return new models::Settings();
  }

  template <>
  drawing::Camera* DI::inject() {
    auto settings = get<models::Settings>();
    return new drawing::Camera(
      glm::vec3(0.0f, 0.0f, settings->world.size * 2.0f),
      glm::quat(),
      glm::radians(45.0f),
      (float)settings->window.width,
      (float)settings->window.height,
      0.1f, // near
      settings->world.size * 4.0f // far
    );
  }

  template <>
  drawing::Canvas* DI::inject() {
    return new drawing::CanvasWithProfiler(get<drawing::Camera>(), get<utils::Profiler>());
  }

  template <>
  drawing::shaders::Shaders* DI::inject() {
    return new drawing::shaders::Shaders(get<utils::FileUtil>(), get<drawing::Canvas>()->context);
  }

  template <>
  Grid* DI::inject() {
    return new Grid((unsigned int)get<models::Settings>()->world.size);
  }

  template <>
  utils::opencl::Resources* DI::inject() {
    return new utils::opencl::Resources();
  }
  
  template <>
  utils::opencl::ResourcesProvider* DI::inject() {
    return new utils::opencl::ResourcesProvider(get<utils::FileUtil>(), get<utils::opencl::Resources>());
  }
  
  template <>
  gpu::GPUAccessor* DI::inject() {
    auto settings = get<models::Settings>();
    auto grid = get<Grid>();
    return new gpu::GPUAccessor(
      get<utils::opencl::ResourcesProvider>(),
      settings->world.maxActorCount,
      utils::math::powerOf2(settings->world.maxActorCount),
      settings->world.maxActorCount * 16,
      grid->xCount * grid->yCount * grid->zCount
    );
  }
  
  template <>
  physics::softbody::Simulator* DI::inject() {
    return new physics::softbody::Simulator();
  }

  template <>
  physics::fluid::Simulator* DI::inject() {
    return new physics::fluid::Simulator();
  }

  template <>
  physics::Simulator* DI::inject() {
    auto settings = get<models::Settings>();
    auto grid = get<Grid>();
    return new physics::Simulator(
      settings->world.maxActorCount,
      grid->edgeLength,
      grid->xCount,
      grid->yCount,
      grid->zCount,
      settings->physics.timeStepSize,
      get<gpu::GPUAccessor>()
    );
  }

  template <>
  models::drawing::Renderer* DI::inject() {
    return new models::drawing::Renderer(
      get<gpu::GPUAccessor>(),
      get<drawing::Canvas>(),
      get<utils::opencl::ResourcesProvider>(),
      get<models::Settings>()->world.maxActorCount * 4);
  }

  template <>
  models::Alcube* DI::inject() {
    return new models::Alcube(
      get<physics::fluid::Simulator>(),
      get<physics::softbody::Simulator>(),
      get<physics::Simulator>(),
      get<models::drawing::Renderer>()
    );
  }

  template <>
  models::drawing::Model3DFactory* DI::inject() {
    return new models::drawing::Model3DFactory(
      new utils::MemoryPool<models::drawing::Model3D>(get<models::Settings>()->world.maxActorCount)
    );
  }

  template <>
  models::drawing::RenderingGroupFactory* DI::inject() {
    auto settings = get<models::Settings>();
    return new models::drawing::RenderingGroupFactory(
      new utils::MemoryPool<models::drawing::RenderingGroup>(settings->world.maxActorCount),
      get<drawing::shaders::Shaders>(),
      settings
    );
  }

  template <>
  models::ActorFactory* DI::inject() {
    auto settings = get<models::Settings>();
    return new models::ActorFactory(
      new utils::MemoryPool<models::Actor>(settings->world.maxActorCount),
      get<models::drawing::Model3DFactory>()
    );
  }

  template <>
  models::physics::softbody::SpringFactory* DI::inject() {
    return new models::physics::softbody::SpringFactory(
      new utils::MemoryPool<models::physics::softbody::Spring>(get<models::Settings>()->world.maxActorCount)
    );
  }

  template <>
  models::physics::fluid::FeaturesFactory* DI::inject() {
    return new models::physics::fluid::FeaturesFactory(
      new utils::MemoryPool<models::physics::fluid::Features>(get<models::Settings>()->world.maxActorCount),
      get<alcube::physics::ActorFactory>()
    );
  }

  template <>
  models::physics::softbody::FeaturesFactory* DI::inject() {
    return new models::physics::softbody::FeaturesFactory(
      new utils::MemoryPool<models::physics::softbody::Features>(get<models::Settings>()->world.maxActorCount),
      get<alcube::physics::ActorFactory>()
    );
  }

  template <>
  alcube::physics::ActorFactory* DI::inject() {
    return new alcube::physics::ActorFactory(get<models::Settings>()->world.maxActorCount, get<gpu::GPUAccessor>());
  }
}
