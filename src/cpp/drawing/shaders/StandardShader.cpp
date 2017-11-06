#include "StandardShader.h"

namespace drawing::shaders {
  StandardShader::StandardShader(utils::FileUtil* fileUtil) {
    std::string vertexShaderCodeStr = fileUtil->readFile("../src/shaders/standard.vertexshader");
    std::string fragmentShaderCodeStr = fileUtil->readFile("../src/shaders/standard.fragmentshader");
    static const char* vertexShaderCode = vertexShaderCodeStr.c_str();
    static const char* fragmentShaderCode = fragmentShaderCodeStr.c_str();
    const char* uniformNames[] = { "MVP", "V", "M", "LightPosition_worldspace" };
    compile(vertexShaderCode, fragmentShaderCode, uniformNames, 4);
  }

  void StandardShader::shade(Context& context) {
    glUniformMatrix4fv(uniformIds[0], 1, GL_FALSE, &context.mvp[0][0]);
    glUniformMatrix4fv(uniformIds[1], 1, GL_FALSE, &context.v[0][0]);
    glUniformMatrix4fv(uniformIds[2], 1, GL_FALSE, &context.m[0][0]);
    glUniform3fv(uniformIds[3], 1, &glm::vec3(5.0f, 5.0f, 0.0f)[0]);
  }

  void StandardShader::setupBuffer(Buffer *buffer) {
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

    if (buffer->normals != nullptr) {
      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, buffer->normalBufferId);
      glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)buffer->normalsSize, buffer->normals);
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