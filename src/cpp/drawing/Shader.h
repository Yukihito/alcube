#ifndef ALCUBE_SHADER_H
#define ALCUBE_SHADER_H
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "../utils/FileUtil.h"
#include "Context.h"
#include "Buffer.h"
#include "Shape.h"

namespace alcube::drawing {
  class VertexAttribute {
    public:
      VertexBufferType type;
      GLuint location;
      explicit VertexAttribute(VertexBufferType type, GLuint location);
  };

  class InstanceAttribute {
    public:
      InstanceBufferType type;
      GLuint location;
      explicit InstanceAttribute(InstanceBufferType type, GLuint location);
  };

  class ShaderUniforms {
    public:
      int MVP;
      int MV;
      int diffuse;
      int ambient;
      int specular;
  };

  class Shader {
    public:
      GLuint programId = 0;
      virtual void bindUniforms(Context &context) = 0;
      virtual void bindShape(Shape* shape);
      virtual void unbindShape(Shape* shape);
    protected:
      std::vector<VertexAttribute*> vertexAttributes;
      std::vector<InstanceAttribute*> instanceAttributes;
      ShaderUniforms uniforms;
      GLint* uniformLocations = nullptr;
      void compile(
        const char *vertexShaderCode,
        const char *fragmentShaderCode,
        const char **uniformNames,
        unsigned int uniformsCount
      );
  };
}

#endif //ALCUBE_SHADER_H
