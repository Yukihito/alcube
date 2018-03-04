#include "TileShader.h"

namespace alcube::drawing::shaders {
  TileShader::TileShader(utils::FileUtil* fileUtil) {
    std::string vertexShaderCodeStr = fileUtil->readFile("../src/shaders/tile.vertexshader");
    std::string fragmentShaderCodeStr = fileUtil->readFile("../src/shaders/tile.fragmentshader");
    static const char* vertexShaderCode = vertexShaderCodeStr.c_str();
    static const char* fragmentShaderCode = fragmentShaderCodeStr.c_str();
    const char* uniformNames[] = { "MVP", "V", "M", "LightPosition_worldspace", "diffuse", "ambient", "specular"};
    compile(vertexShaderCode, fragmentShaderCode, uniformNames, 7);
  }

  void TileShader::shade(Context& context) {
    glUniformMatrix4fv(uniformLocations[0], 1, GL_FALSE, &context.mvp[0][0]);
    glUniformMatrix4fv(uniformLocations[1], 1, GL_FALSE, &context.v[0][0]);
    glUniformMatrix4fv(uniformLocations[2], 1, GL_FALSE, &context.m[0][0]);
    glUniform3fv(uniformLocations[3], 1, &glm::vec3(0.0f, 0.0f, 8.0f)[0]);
    glUniform3fv(uniformLocations[4], 1, &context.material.diffuse[0]);
    glUniform3fv(uniformLocations[5], 1, &context.material.ambient[0]);
    glUniform3fv(uniformLocations[6], 1, &context.material.specular[0]);
  }
  /*

  void TileShader::bindBuffer(Buffer *buffer) {
    enableVertexBuffer(buffer->vertices, 0);
    enableIndexBuffer(buffer->indices);
    enableVertexBuffer(buffer->uvs, 1);
    enableVertexBuffer(buffer->normals, 2);
  }
   */
}