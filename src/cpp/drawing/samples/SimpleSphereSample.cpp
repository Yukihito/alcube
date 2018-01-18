#include "SimpleSphereSample.h"


namespace alcube::drawing::samples {
  SimpleSphere::SimpleSphere(glm::vec3& position, shapes::Shapes* shapes, shaders::Shaders* shaders) {
    shape = &shapes->triangles.sphere;
    shader = &shaders->standard;
    this->position = position;
    this->material = new Material();
    this->material->diffuse = glm::vec3(0.5f, 1.0f, 1.0f);
    this->material->ambient = glm::vec3(0.25f, 0.5f, 0.5f);
    this->material->specular = glm::vec3(0.3f, 0.3f, 0.3f);
  }

  glm::mat4 SimpleSphere::getModelMat() {
    return glm::translate(position);
  }

  Material* SimpleSphere::getMaterial() {
    return this->material;
  }

  void SimpleSphereSample::initWindowParams() {
    windowWidth = 800;
    windowHeight = 600;
    fps = 30;
    appName = "SimpleSphereSample";
    isMultiSampleEnabled = false;
  }

  void SimpleSphereSample::onInit() {
    printSystemInfo();
    auto shaders = new shaders::Shaders(new utils::FileUtil());
    auto shapes = new shapes::Shapes();
    camera = new Camera(
      glm::vec3(0.0f, 0.0f, 200.0f),
      glm::quat(),
      glm::radians(45.0f),
      (float)windowWidth,
      (float)windowHeight,
      0.1f,
      10000.0f
    );
    drawer = new Drawer(camera, &drawablesMutex);
    glm::vec3 pos1 = glm::vec3();
    for (int i = 0; i < 1000; i++) {
      auto sphere = new SimpleSphere(pos1, shapes, shaders);
      spheres.push_back(sphere);
      drawer->add(sphere);
    }
  }

  void SimpleSphereSample::onDraw() {
    drawer->draw();
  }

  void SimpleSphereSample::onUpdate() {
    static float a = 0.0f;
    a += 0.04f;
    float offset = 0.0f;
    for (SimpleSphere* sphere: spheres) {
      sphere->position = glm::vec3(50.0f * cosf(a + offset), 50.0f * sinf(a + offset), 0.0f);
      offset += (3.1415f * 2.0f) / spheres.size();
    }

  }

  void SimpleSphereSample::onClose() {

  }
}