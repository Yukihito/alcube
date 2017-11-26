#include "ParticleShader.h"

namespace alcube::drawing::shaders {
  ParticleShader::ParticleShader(utils::FileUtil *fileUtil) {
    std::string vertexShaderCodeStr = fileUtil->readFile("../src/shaders/particle.vertexshader");
    std::string fragmentShaderCodeStr = fileUtil->readFile("../src/shaders/particle.fragmentshader");
    static const char* vertexShaderCode = vertexShaderCodeStr.c_str();
    static const char* fragmentShaderCode = fragmentShaderCodeStr.c_str();
    const char* uniformNames[] = { "MVP" };
    compile(vertexShaderCode, fragmentShaderCode, uniformNames, 1);
  }

  void ParticleShader::shade(Context &context) {
    glUniformMatrix4fv(uniformIds[0], 1, GL_FALSE, &context.mvp[0][0]);
  }

  void ParticleShader::setupBuffer(Buffer *buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer->vertexBufferId);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)buffer->verticesSize, buffer->vertices);
    if (buffer->indices != nullptr) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->indexBufferId);
      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, (GLsizeiptr)buffer->indicesSize, buffer->indices);
    }
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
      0,        // 属性0：シェーダ内のlayoutと合わせる必要がある
      3,        // 頂点1つのサイズ。vec3のため3を指定
      GL_FLOAT, // 頂点の要素の型
      GL_FALSE, // 正規化
      0,        // ストライド
      nullptr   // 配列バッファオフセット
    );

    if (buffer->colors != nullptr) {
      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, buffer->colorBufferId);
      glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)buffer->colorsSize, buffer->colors);
      glVertexAttribPointer(
        1,                                // 属性
        3,                                // サイズ
        GL_FLOAT,                         // タイプ
        GL_FALSE,                         // 正規化？
        0,                                // ストライド
        nullptr                          // 配列バッファオフセット
      );
    }
  }
}