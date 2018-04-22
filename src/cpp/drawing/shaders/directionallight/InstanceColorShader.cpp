#include "InstanceColorShader.h"

namespace alcube::drawing::shaders::directionallight {
  InstanceColorShader::InstanceColorShader(utils::FileUtil *fileUtil, Context &context) {
    std::string vertexShaderCodeStr = fileUtil->readFile("../src/shaders/directional-light/instance-color.vertexshader");
    std::string fragmentShaderCodeStr = fileUtil->readFile("../src/shaders/directional-light/instance-color.fragmentshader");
    static const char* vertexShaderCode = vertexShaderCodeStr.c_str();
    static const char* fragmentShaderCode = fragmentShaderCodeStr.c_str();
    uniforms.push_back(new MatrixUniform("VP", &context.vp[0][0]));
    uniforms.push_back(new MatrixUniform("V", &context.v[0][0]));
    uniforms.push_back(new Float3Uniform("diffuse", &context.material.diffuse[0]));
    uniforms.push_back(new Float3Uniform("ambient", &context.material.ambient[0]));
    uniforms.push_back(new Float3Uniform("specular", &context.material.specular[0]));

    compile(vertexShaderCode, fragmentShaderCode);
    vertexAttributes.push_back(new VertexAttribute(VERTICES, 0));
    vertexAttributes.push_back(new VertexAttribute(NORMALS, 1));
    instanceAttributes.push_back(new InstanceAttribute(POSITIONS, 2));
    instanceAttributes.push_back(new InstanceAttribute(COLORS, 3));
  }
}