#ifndef ALCUBE_SHADER_H
#define ALCUBE_SHADER_H
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "../utils/FileUtil.h"
#include "Context.h"
#include "Buffer.h"

namespace alcube::drawing {
  class Shader {
    public:
      GLuint programId = 0;
      virtual void setupBuffer(Buffer* buffer) = 0;
      virtual void shade(Context& context) = 0;
    protected:
      GLint* uniformIds = nullptr;
      void compile(
        const char *vertexShaderCode,
        const char *fragmentShaderCode,
        const char **uniformNames,
        unsigned int uniformsCount
      );

      void setIndexBuffer(VBO* vbo);
      void setVertexBuffer(VBO* vbo, GLuint location);
  };
}

#endif //ALCUBE_SHADER_H
