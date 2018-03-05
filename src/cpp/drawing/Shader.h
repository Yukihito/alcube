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

  class Uniform {
    public:
      explicit Uniform(std::string name, GLfloat* data);
      std::string name;
      GLint location;
      GLfloat* data;
      virtual void bind() = 0;
  };

  class MatrixUniform : public Uniform {
    public:
      explicit MatrixUniform(std::string name, GLfloat* data);
      void bind() override;
  };

  class Float3Uniform : public Uniform {
    public:
      explicit Float3Uniform(std::string name, GLfloat* data);
      void bind() override;
  };

  class Shader {
    public:
      GLuint programId = 0;
      virtual void bindUniforms();
      virtual void bindShape(Shape* shape);
      virtual void unbindShape(Shape* shape);
    protected:
      std::vector<VertexAttribute*> vertexAttributes;
      std::vector<InstanceAttribute*> instanceAttributes;
      std::vector<Uniform*> uniforms;
      void compile(
        const char *vertexShaderCode,
        const char *fragmentShaderCode
      );
  };
}

#endif //ALCUBE_SHADER_H
