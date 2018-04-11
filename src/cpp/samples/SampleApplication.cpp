#include "SampleApplication.h"

namespace alcube::samples {
  SampleApplication::SampleApplication(
    models::Settings* settings,
    const char* programName
  ) : ApplicationBase(settings, "SampleApplication", programName) {}

  void SampleApplication::onInit() {
    beforeSetup();

    glm::vec3 color = glm::vec3(0.4f, 0.4f, 1.0f);
    drawable = new SphereDrawable(
      shaders->directionalLight,
      color,
      settings->world.maxActorCount,
      (GLfloat*)gpuAccessor->memories.positions.dto
    );
    drawable->shape->instanceCount = cube->getActorCount();
    drawer->add(drawable);
    afterSetup();
  }
}