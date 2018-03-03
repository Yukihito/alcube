#include "ParticleShader.h"

namespace alcube::drawing::shaders {
  ParticleShader::ParticleShader(utils::FileUtil *fileUtil) {
    std::string vertexShaderCodeStr = fileUtil->readFile("../src/shaders/particle.vertexshader");
    std::string fragmentShaderCodeStr = fileUtil->readFile("../src/shaders/particle.fragmentshader");
    static const char* vertexShaderCode = vertexShaderCodeStr.c_str();
    static const char* fragmentShaderCode = fragmentShaderCodeStr.c_str();
    const char* uniformNames[] = { "MVP" };
    compile(vertexShaderCode, fragmentShaderCode, uniformNames, 1);
  }

  void ParticleShader::shade(Context &context) {
    glUniformMatrix4fv(uniformIds[0], 1, GL_FALSE, &context.mvp[0][0]);
  }

  void ParticleShader::setupBuffer(Buffer *buffer) {
    setVertexBuffer(buffer->vertices, 0);
    setIndexBuffer(buffer->indices);
    setVertexBuffer(buffer->colors, 1);
  }
}