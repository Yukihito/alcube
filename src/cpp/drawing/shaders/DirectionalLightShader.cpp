#include "DirectionalLightShader.h"

namespace alcube::drawing::shaders {
  DirectionalLightShader::DirectionalLightShader(utils::FileUtil *fileUtil) {
    std::string vertexShaderCodeStr = fileUtil->readFile("../src/shaders/directional-light.vertexshader");
    std::string fragmentShaderCodeStr = fileUtil->readFile("../src/shaders/directional-light.fragmentshader");
    static const char* vertexShaderCode = vertexShaderCodeStr.c_str();
    static const char* fragmentShaderCode = fragmentShaderCodeStr.c_str();
    const char* uniformNames[] = { "MVP", "MV", "diffuse", "ambient", "specular" };
    compile(vertexShaderCode, fragmentShaderCode, uniformNames, 5);
    uniforms.MVP = 0;
    uniforms.MV = 1;
    uniforms.diffuse = 2;
    uniforms.ambient = 3;
    uniforms.specular = 4;

    attributes.vertices = 0;
    attributes.positions = 1;
    attributes.normals = 2;
  }

  void DirectionalLightShader::shade(Context &context) {
    glUniformMatrix4fv(uniformLocations[uniforms.MVP], 1, GL_FALSE, &context.mvp[0][0]);
    glUniformMatrix4fv(uniformLocations[uniforms.MV], 1, GL_FALSE, &context.mv[0][0]);
    glUniform3fv(uniformLocations[uniforms.diffuse], 1, &context.material.diffuse[0]);
    glUniform3fv(uniformLocations[uniforms.ambient], 1, &context.material.ambient[0]);
    glUniform3fv(uniformLocations[uniforms.specular], 1, &context.material.specular[0]);
  }

  void DirectionalLightShader::bindShape(Shape *shape) {
    shape->indexBuffer->enable();
    shape->instanceBuffers[POSITIONS]->update();
    shape->instanceBuffers[POSITIONS]->enable(attributes.positions);
    shape->vertexBuffers[VERTICES]->enable(attributes.vertices);
    shape->vertexBuffers[NORMALS]->enable(attributes.normals);
  }

  void DirectionalLightShader::unbindShape(Shape *shape) {
    shape->indexBuffer->disable();
    shape->instanceBuffers[POSITIONS]->disable();
    shape->vertexBuffers[VERTICES]->disable();
    shape->vertexBuffers[NORMALS]->disable();
  }

  /*
  void DirectionalLightShader::bindBuffer(Buffer *buffer) {
    glVertexAttribDivisor(attributes.positions, 1);
    buffer->positions->update();
    buffer->indices->enable();

    buffer->vertices->enable(attributes.vertices);
    buffer->positions->enable(attributes.positions);
    buffer->normals->enable(attributes.normals);
  }
   */
}