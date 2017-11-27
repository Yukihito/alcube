#include "StandardShader.h"

namespace alcube::drawing::shaders {
  StandardShader::StandardShader(utils::FileUtil* fileUtil) {
    std::string vertexShaderCodeStr = fileUtil->readFile("../src/shaders/standard.vertexshader");
    std::string fragmentShaderCodeStr = fileUtil->readFile("../src/shaders/standard.fragmentshader");
    static const char* vertexShaderCode = vertexShaderCodeStr.c_str();
    static const char* fragmentShaderCode = fragmentShaderCodeStr.c_str();
    const char* uniformNames[] = { "MVP", "V", "M", "LightPosition_worldspace" };
    compile(vertexShaderCode, fragmentShaderCode, uniformNames, 4);
  }

  void StandardShader::shade(Context& context) {
    glUniformMatrix4fv(uniformIds[0], 1, GL_FALSE, &context.mvp[0][0]);
    glUniformMatrix4fv(uniformIds[1], 1, GL_FALSE, &context.v[0][0]);
    glUniformMatrix4fv(uniformIds[2], 1, GL_FALSE, &context.m[0][0]);
    glUniform3fv(uniformIds[3], 1, &glm::vec3(5.0f, 5.0f, 0.0f)[0]);
  }

  void StandardShader::setupBuffer(Buffer *buffer) {
    setVertexBuffer(buffer->vbos.vertices, 0);
    setIndexBuffer(buffer->vbos.indices);
    setVertexBuffer(buffer->vbos.normals, 1);
  }
}