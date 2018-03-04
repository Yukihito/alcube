#include "Shader.h"

namespace alcube::drawing {
  using namespace std;

  void Shader::compile(
    const char *vertexShaderCode,
    const char *fragmentShaderCode,
    const char **uniformNames,
    unsigned int uniformsCount
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

    uniformLocations = nullptr;
    if (uniformsCount > 0) {
      uniformLocations = new GLint[uniformsCount];
      for (int i = 0; i < uniformsCount; i++) {
        uniformLocations[i] = glGetUniformLocation(programId, uniformNames[i]);
      }
    }
  }

  void Shader::enableIndexBuffer(VBO *vbo) {
    if (vbo == nullptr) {
      return;
    }
    glBindBuffer(vbo->target, vbo->bufferId);
    glBufferSubData(vbo->target, 0, vbo->size, vbo->data);
  }

  void Shader::enableVertexBuffer(VBO *vbo, GLuint location) {
    if (vbo == nullptr) {
      return;
    }
    glEnableVertexAttribArray(location);
    glBindBuffer(vbo->target, vbo->bufferId);
    glBufferSubData(vbo->target, 0, vbo->size, vbo->data);
    glVertexAttribPointer(
      location,
      vbo->vertexSize,
      GL_FLOAT,
      GL_FALSE,
      0,
      nullptr
    );
  }
}
