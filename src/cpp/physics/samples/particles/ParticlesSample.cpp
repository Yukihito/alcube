#include "ParticlesSample.h"

namespace alcube::physics::samples::particles {
  Particles::Particles(
    drawing::shapes::Shapes *shapes,
    drawing::shaders::Shaders *shaders,
    unsigned int maxParticleCount
  ) {
    particlesShape = shapes->points.createParticles(maxParticleCount);
    shape = particlesShape;
    shader = &shaders->directionalLight;
    positions = new GLfloat[maxParticleCount * 3]();
    colors = new GLfloat[maxParticleCount * 3]();
    /*
    shape->buffer->vertices->size = 0;
    shape->buffer->colors->size = 0;
    shape->buffer->vertices->data = positions;
    shape->buffer->colors->data = colors;
     */
  }

  /*
  glm::mat4 Particles::getModelMat() {
    static float t = 0.0f;
    t += 0.001f;
    return glm::rotate(t, glm::vec3(0.0f, 1.0f, 0.0f));
  }
   */

  void Particles::update(std::vector<SoftBodyParticle *> &cells) {
    for (int i = 0; i < cells.size(); i++) {
      int j = i * 3;
      positions[j + 0] = cells[i]->position.x;
      positions[j + 1] = cells[i]->position.y;
      positions[j + 2] = cells[i]->position.z;
    }

    //
    // shape->buffer->vertices->size = cells.size() * sizeof(GLfloat) * 3;
    //shape->buffer->colors->size = cells.size() * sizeof(GLfloat) * 3;

    particlesShape->particleCount = (unsigned int)cells.size();
  }

  ParticlesSample::ParticlesSample() : ApplicationBase(1600, 1200, 30, "ParticlesSample") {}

  void ParticlesSample::onInit() {
    initBase(512, 65536);
    physicsSimulator->gravity = 0.0f;
    particles = new Particles(shapes, shaders, maxCellCount);

    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<float> randReal(0, 255);
    std::uniform_real_distribution<float> randReal2(-30, 30);
    std::uniform_real_distribution<float> randReal3(-1, 1);

    for (int i = 0; i < maxCellCount; i++) {
      auto cell = new SoftBodyParticle();
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
      /*
      particle->angularMomentum = glm::vec3(
        randReal2(mt),
        randReal2(mt),
        randReal2(mt)
      );
       */
      physicsSimulator->add(cell);
      cells.push_back(cell);
      int j = i * 3;
      particles->colors[j + 0] = (float)(fabs(cell->position.x) + 256.0f) / 512.0f;
      particles->colors[j + 1] = (float)(fabs(cell->position.y) + 256.0f) / 512.0f;
      particles->colors[j + 2] = (float)(fabs(cell->position.z) + 256.0f) / 512.0f;
    }

    particles->update(cells);
    drawer->add(particles);
  }

  void ParticlesSample::onDraw() {
    particles->update(cells);
    ApplicationBase::onDraw();
  }
}