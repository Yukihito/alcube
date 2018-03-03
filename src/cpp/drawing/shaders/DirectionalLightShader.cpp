#include "DirectionalLightShader.h"

namespace alcube::drawing::shaders {
  DirectionalLightShader::DirectionalLightShader(utils::FileUtil *fileUtil) {
    std::string vertexShaderCodeStr = fileUtil->readFile("../src/shaders/directional-light.vertexshader");
    std::string fragmentShaderCodeStr = fileUtil->readFile("../src/shaders/directional-light.fragmentshader");
    static const char* vertexShaderCode = vertexShaderCodeStr.c_str();
    static const char* fragmentShaderCode = fragmentShaderCodeStr.c_str();
    const char* uniformNames[] = { "MVP", "MV", "diffuse", "ambient", "specular" };
    compile(vertexShaderCode, fragmentShaderCode, uniformNames, 5);
  }

  void DirectionalLightShader::shade(Context &context) {
    glm::mat4 mv = context.v * context.m;
    glUniformMatrix4fv(uniformIds[0], 1, GL_FALSE, &context.mvp[0][0]);
    glUniformMatrix4fv(uniformIds[1], 1, GL_FALSE, &mv[0][0]);
    glUniform3fv(uniformIds[2], 1, &context.material.diffuse[0]);
    glUniform3fv(uniformIds[3], 1, &context.material.ambient[0]);
    glUniform3fv(uniformIds[4], 1, &context.material.specular[0]);
  }

  void DirectionalLightShader::setupBuffer(Buffer *buffer) {
    glVertexAttribDivisor(1, 1);
    setVertexBuffer(buffer->positions, 1); // DYNAMIC
    if (!initialized) {
      setVertexBuffer(buffer->vertices, 0); // STATIC
      setIndexBuffer(buffer->indices); // STATIC
      setVertexBuffer(buffer->normals, 2); // STATIC
      initialized = true;
    }
  }
}