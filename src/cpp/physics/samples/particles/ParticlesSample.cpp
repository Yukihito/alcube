#include "ParticlesSample.h"

namespace alcube::physics::samples::particles {
  Particles::Particles(
    drawing::shapes::Shapes *shapes,
    drawing::shaders::Shaders *shaders,
    unsigned int maxParticleCount
  ) {
    particlesShape = shapes->points.createParticles(maxParticleCount);
    shape = particlesShape;
    shader = &shaders->particle;
    positions = new GLfloat[maxParticleCount * 3]();
    colors = new GLfloat[maxParticleCount * 3]();
    shape->buffer->vbos.vertices->size = 0;
    shape->buffer->vbos.colors->size = 0;
    shape->buffer->vbos.vertices->data = positions;
    shape->buffer->vbos.colors->data = colors;
  }

  glm::mat4 Particles::getModelMat() {
    static float t = 0.0f;
    t += 0.01f;
    return glm::rotate(t, glm::vec3(0.0f, 1.0f, 0.0f));
  }

  void Particles::update(std::vector<Cell *> &cells) {
    for (int i = 0; i < cells.size(); i++) {
      int j = i * 3;
      positions[j + 0] = cells[i]->position.x;
      positions[j + 1] = cells[i]->position.y;
      positions[j + 2] = cells[i]->position.z;

      //colors[j + 0] = (float)fabs(cells[i]->linearMomentum.x) / 30.0f;
      //colors[j + 1] = (float)fabs(cells[i]->linearMomentum.y) / 30.0f;
      //colors[j + 2] = (float)fabs(cells[i]->linearMomentum.z) / 30.0f;
    }

    shape->buffer->vbos.vertices->size = cells.size() * sizeof(GLfloat) * 3;
    shape->buffer->vbos.colors->size = cells.size() * sizeof(GLfloat) * 3;

    particlesShape->particleCount = (unsigned int)cells.size();
  }

  void ParticlesSample::initWindowParams() {
    windowWidth = 800;
    windowHeight = 600;
    fps = 30;
    appName = "ParticlesSample";
    isMultiSampleEnabled = false;
  }

  void ParticlesSample::onInit() {
    maxCellCount = 65536 / 2;
    deltaTime = 1.0f / 30.0f;
    unsigned int gridEdgeLength = 8;
    unsigned int xGridCount = 64;
    unsigned int yGridCount = 64;
    unsigned int zGridCount = 64;
    float near = 0.1f;
    float far = gridEdgeLength * xGridCount * 4.0f;
    shaders = new drawing::shaders::Shaders(new utils::FileUtil());
    shapes = new drawing::shapes::Shapes();
    camera = new drawing::Camera(
      glm::vec3(0.0f, 0.0f, far / 2),
      glm::quat(),
      glm::radians(45.0f),
      (float)windowWidth,
      (float)windowHeight,
      near,
      far
    );
    drawer = new drawing::Drawer(camera, &mutex);
    resources = new utils::opencl::Resources();
    fileUtil = new utils::FileUtil();
    physicsSimulator = new Simulator(
      resources,
      fileUtil,
      &mutex,
      maxCellCount,
      gridEdgeLength,
      xGridCount,
      yGridCount,
      zGridCount
    );

    particles = new Particles(shapes, shaders, maxCellCount);

    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<float> randReal(0, 256);
    std::uniform_real_distribution<float> randReal2(-30, 30);

    for (int i = 0; i < maxCellCount; i++) {
      auto cell = new Cell();
      cell->position = glm::vec3(
        randReal(mt),
        randReal(mt),
        randReal(mt)
      );
      cell->linearMomentum = glm::vec3(
        randReal2(mt),
        randReal2(mt),
        randReal2(mt)
      );
      physicsSimulator->add(cell);
      cells.push_back(cell);
      int j = i * 3;
      particles->colors[j + 0] = (float)(fabs(cell->position.x) + 256.0f) / 512.0f;
      particles->colors[j + 1] = (float)(fabs(cell->position.y) + 256.0f) / 512.0f;
      particles->colors[j + 2] = (float)(fabs(cell->position.z) + 256.0f) / 512.0f;
      //particles->colors[j + 0] = (float)(fabs(cell->linearMomentum.x) + 30.0f) / 60.0f;
      //particles->colors[j + 1] = (float)(fabs(cell->linearMomentum.y) + 30.0f) / 60.0f;
      //particles->colors[j + 2] = (float)(fabs(cell->linearMomentum.z) + 30.0f) / 60.0f;
    }

    particles->update(cells);
    drawer->add(particles);
    profiler = new utils::Profiler();
    profiler->setShowInterval(1000);
    profiler->enabled = true;
    profilers.update = profiler->create("update");
    profilers.all = profiler->create("all");

    profiler->start(profilers.all);
  }

  void ParticlesSample::onDraw() {
    drawer->draw();
  }

  void ParticlesSample::onUpdate() {
    profiler->start(profilers.update);
    physicsSimulator->update(deltaTime);

    mutex.lock();
    particles->update(cells);
    mutex.unlock();
    profiler->stop(profilers.update);

    profiler->stop(profilers.all);
    profiler->update();
    profiler->start(profilers.all);
  }

  void ParticlesSample::onClose() {
    physicsSimulator->tearDownMemories();
    resources->release();
  }
}