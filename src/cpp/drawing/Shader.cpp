#include "Shader.h"

#include <utility>

namespace alcube::drawing {
  using namespace std;

  VertexAttribute::VertexAttribute(VertexBufferType type, GLuint location) {
    this->type = type;
    this->location = location;
  }

  InstanceAttribute::InstanceAttribute(InstanceBufferType type, GLuint location) {
    this->type = type;
    this->location = location;
  }

  Uniform::Uniform(std::string name, GLfloat *data) {
    this->name = std::move(name);
    this->data = data;
  }

  MatrixUniform::MatrixUniform(std::string name, GLfloat *data) : Uniform(std::move(name), data) {}

  void MatrixUniform::bind() {
    glUniformMatrix4fv(location, 1, GL_FALSE, data);
  }

  Float3Uniform::Float3Uniform(std::string name, GLfloat *data) : Uniform(std::move(name), data) {}

  void Float3Uniform::bind() {
    glUniform3fv(location, 1, data);
  }

  void Shader::compile(
    const char *vertexShaderCode,
    const char *fragmentShaderCode
  ) {
    // シェーダを作ります。
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    GLint result = GL_FALSE;
    GLint infoLogLength;

    // 頂点シェーダをコンパイルします。
    cout << "Compiling shader" << endl;
    glShaderSource(vertexShaderId, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexShaderId);

    // 頂点シェーダをチェックします。
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> vertexShaderErrorMessage((unsigned long) infoLogLength);
    glGetShaderInfoLog(vertexShaderId, infoLogLength, nullptr, &vertexShaderErrorMessage[0]);
    if (!vertexShaderErrorMessage.empty()) {
      cout << &vertexShaderErrorMessage[0] << endl;
    }

    // フラグメントシェーダをコンパイルします。
    cout << "Compiling shader" << endl;
    glShaderSource(fragmentShaderId, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragmentShaderId);

    // フラグメントシェーダをチェックします。
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> fragmentShaderErrorMessage((unsigned long) infoLogLength);
    glGetShaderInfoLog(fragmentShaderId, infoLogLength, nullptr, &fragmentShaderErrorMessage[0]);
    if (!fragmentShaderErrorMessage.empty()) {
      cout << &fragmentShaderErrorMessage[0] << endl;
    }

    // プログラムをリンクします。
    cout << "Linking program" << endl;
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);

    // プログラムをチェックします。
    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> programErrorMessage((unsigned long) std::max(infoLogLength, int(1)));
    glGetProgramInfoLog(programId, infoLogLength, nullptr, &programErrorMessage[0]);
    if (!programErrorMessage.empty()) {
      cout << &programErrorMessage[0] << endl;
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    for (auto uniform : uniforms) {
      uniform->location = glGetUniformLocation(programId, uniform->name.c_str());
    }
  }

  void Shader::bindShape(Shape *shape) {
    shape->indexBuffer->enable();
    for (auto attr : vertexAttributes) {
      shape->vertexBuffers[attr->type]->enable(attr->location);
    }
    for (auto attr : instanceAttributes) {
      shape->instanceBuffers[attr->type]->enable(attr->location);
    }
  }

  void Shader::unbindShape(Shape *shape) {
    shape->indexBuffer->disable();
    for (auto attr : vertexAttributes) {
      shape->vertexBuffers[attr->type]->disable();
    }
    for (auto attr : instanceAttributes) {
      shape->instanceBuffers[attr->type]->disable();
    }
  }

  void Shader::bindUniforms() {
    for (auto uniform : uniforms) {
      uniform->bind();
    }
  }
}
