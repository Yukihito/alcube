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
    glUniformMatrix4fv(uniformIds[0], 1, GL_FALSE, &context.mvp[0][0]);
    glUniformMatrix4fv(uniformIds[1], 1, GL_FALSE, &context.mv[0][0]);
    glUniform3fv(uniformIds[2], 1, &context.material.diffuse[0]);
    glUniform3fv(uniformIds[3], 1, &context.material.ambient[0]);
    glUniform3fv(uniformIds[4], 1, &context.material.specular[0]);
  }

  void DirectionalLightShader::bindBuffer(Buffer *buffer) {
    glVertexAttribDivisor(1, 1);
    buffer->positions->update();
    buffer->indices->enable();
    buffer->vertices->enable(0);
    buffer->positions->enable(1);
    buffer->normals->enable(2);
  }

  void DirectionalLightShader::unbindBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }
}