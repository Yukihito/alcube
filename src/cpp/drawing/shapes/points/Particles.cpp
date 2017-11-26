#include "Particles.h"

namespace alcube::drawing::shapes::points {
  Particles::Particles(unsigned int maxParticleCount) {
    this->maxParticleCount = maxParticleCount;
    particleCount = 0;
    initialize();
  }

  void Particles::draw() {
    glDrawArrays(GL_POINTS, 0, particleCount * 3);
  }

  Buffer* Particles::createBuffer() {
    auto buffer = new Buffer(
      maxParticleCount * sizeof(GLfloat) * 3,
      0,
      0,
      maxParticleCount * sizeof(GLfloat) * 3
    );
    buffer->vertices = nullptr;
    buffer->indices = nullptr;
    buffer->normals = nullptr;
    buffer->colors = nullptr;
    return buffer;
  }
}