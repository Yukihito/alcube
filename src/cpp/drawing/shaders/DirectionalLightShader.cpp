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

    vertexAttributes.push_back(new VertexAttribute(VERTICES, 0));
    vertexAttributes.push_back(new VertexAttribute(NORMALS, 2));
    instanceAttributes.push_back(new InstanceAttribute(POSITIONS, 1));
  }

  void DirectionalLightShader::bindUniforms(Context &context) {
    glUniformMatrix4fv(uniformLocations[uniforms.MVP], 1, GL_FALSE, &context.mvp[0][0]);
    glUniformMatrix4fv(uniformLocations[uniforms.MV], 1, GL_FALSE, &context.mv[0][0]);
    glUniform3fv(uniformLocations[uniforms.diffuse], 1, &context.material.diffuse[0]);
    glUniform3fv(uniformLocations[uniforms.ambient], 1, &context.material.ambient[0]);
    glUniform3fv(uniformLocations[uniforms.specular], 1, &context.material.specular[0]);
  }
}