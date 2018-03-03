#include "SimpleShader.h"

namespace alcube::drawing::shaders {
  SimpleShader::SimpleShader(utils::FileUtil* fileUtil) {
    std::string vertexShaderCodeStr = fileUtil->readFile("../src/shaders/simple.vertexshader");
    std::string fragmentShaderCodeStr = fileUtil->readFile("../src/shaders/simple.fragmentshader");
    static const char* vertexShaderCode = vertexShaderCodeStr.c_str();
    static const char* fragmentShaderCode = fragmentShaderCodeStr.c_str();
    const char* uniformNames[] = { "MVP" };
    compile(vertexShaderCode, fragmentShaderCode, uniformNames, 1);
  }

  void SimpleShader::shade(Context& context) {
    glUniformMatrix4fv(uniformIds[0], 1, GL_FALSE, &context.mvp[0][0]);
  }

  void SimpleShader::setupBuffer(Buffer *buffer) {
    setVertexBuffer(buffer->vertices, 0);
    setIndexBuffer(buffer->indices);
  }
}