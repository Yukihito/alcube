#include "DirectionalLightShader.h"

namespace alcube::drawing::shaders {
  DirectionalLightShader::DirectionalLightShader(utils::FileUtil *fileUtil, Context& context) {
    std::string vertexShaderCodeStr = fileUtil->readFile("../src/shaders/directional-light.vertexshader");
    std::string fragmentShaderCodeStr = fileUtil->readFile("../src/shaders/directional-light.fragmentshader");
    static const char* vertexShaderCode = vertexShaderCodeStr.c_str();
    static const char* fragmentShaderCode = fragmentShaderCodeStr.c_str();
    uniforms.push_back(new MatrixUniform("MVP", &context.mvp[0][0]));
    uniforms.push_back(new MatrixUniform("MV", &context.mv[0][0]));
    uniforms.push_back(new Float3Uniform("diffuse", &context.material.diffuse[0]));
    uniforms.push_back(new Float3Uniform("ambient", &context.material.ambient[0]));
    uniforms.push_back(new Float3Uniform("specular", &context.material.specular[0]));
    compile(vertexShaderCode, fragmentShaderCode);
    vertexAttributes.push_back(new VertexAttribute(VERTICES, 0));
    vertexAttributes.push_back(new VertexAttribute(NORMALS, 2));
    instanceAttributes.push_back(new InstanceAttribute(POSITIONS, 1));
  }
}