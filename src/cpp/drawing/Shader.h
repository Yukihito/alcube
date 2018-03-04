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

  /*
     class ShaderAttribute {
      GLuint location;
      bool isInstanceAttr;
      explicit ShaderAttribute(GLuint location, bool isInstanceAttr);
  };
  class ShaderAttributes {
    public:
      ShaderAttribute* vertices;
      ShaderAttribute* normals;
      ShaderAttribute* colors;
      ShaderAttribute* uvs;
      ShaderAttribute* positions;
  };
   */

  class ShaderAttributes {
    public:
      GLuint vertices;
      GLuint normals;
      GLuint colors;
      GLuint uvs;
      GLuint positions;
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
      //virtual void bindBuffer(Buffer *buffer) = 0; // TODO: Delete
      virtual void shade(Context& context) = 0;
      virtual void bindShape(Shape* shape) = 0;
      virtual void unbindShape(Shape* shape) = 0;
    protected:
      ShaderAttributes attributes;
      ShaderUniforms uniforms;
      GLint* uniformLocations = nullptr;
      void compile(
        const char *vertexShaderCode,
        const char *fragmentShaderCode,
        const char **uniformNames,
        unsigned int uniformsCount
      );

      void enableIndexBuffer(VBO *vbo); // TODO: Delete
      void enableVertexBuffer(VBO *vbo, GLuint location); // TODO Delete
  };
}

#endif //ALCUBE_SHADER_H
