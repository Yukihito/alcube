#include <CL/cl_platform.h>
#include "Spheres.h"

namespace alcube::drawing::shapes::triangles {
  void Spheres::draw() {
    glDrawElements(GL_TRIANGLES, (int)this->indicesLength, GL_UNSIGNED_INT, nullptr);
  };

  Buffer* Spheres::createBuffer() {
    auto iSplitCount = (int)splitCount;
    modelVertexCount = splitCount * splitCount * 3;
    size_t verticesLength = modelVertexCount * maxModelCount;
    auto vertices = new GLfloat[verticesLength];

    size_t modelUVsLength = splitCount * splitCount * 2;
    size_t uvsLength = modelUVsLength * maxModelCount;
    modelVertices = new GLfloat[modelVertexCount]();
    auto normals = new GLfloat[verticesLength]();
    auto uvs = new GLfloat[uvsLength];

    float pi = 3.1415f;
    for (int i = 0; i < splitCount; i++) {
      float y = (float)i / (float)splitCount;
      float r = std::sin(y * pi);
      for (int j = 0; j < splitCount; j++) {
        float x = (float)j / (float)splitCount;
        size_t k = (i * splitCount + j) * 3;
        modelVertices[k + 0] = std::cos(x * pi * 2) * r;
        modelVertices[k + 1] = std::cos(y * pi);
        modelVertices[k + 2] = std::sin(x * pi * 2) * r;
        normals[k + 0] = std::cos(x * pi * 2) * r;
        normals[k + 1] = std::cos(y * pi);
        normals[k + 2] = std::sin(x * pi * 2) * r;
      }
    }


    for (unsigned int i = 0; i < verticesLength; i++) {
      normals[i] = normals[i % modelVertexCount];
    }

    size_t modelIndicesLength = (splitCount - 1) * (splitCount - 1) * 6;
    this->indicesLength = modelIndicesLength * maxModelCount;

    auto indices = new GLuint[indicesLength]();
    int indicesIndex = 0;
    for (int i = 0; i < splitCount - 1; i++) {
      for (int j = 0; j < splitCount - 1; j++) {
        indices[indicesIndex + 0] = (uint)(i * iSplitCount + j);
        indices[indicesIndex + 2] = (uint)((i + 1) * iSplitCount + j);
        indices[indicesIndex + 1] = (uint)(i * iSplitCount + j + 1);
        indices[indicesIndex + 3] = (uint)(i * iSplitCount + j + 1);
        indices[indicesIndex + 5] = (uint)((i + 1) * iSplitCount + j);
        indices[indicesIndex + 4] = (uint)((i + 1) * iSplitCount + j + 1);
        indicesIndex += 6;
        size_t l = (i * (splitCount - 1) + j) * 2;
        uvs[l + 0] = (float)i / (float)(splitCount - 1);
        uvs[l + 1] = (float)j / (float)(splitCount - 1);
      }
    }

    for (unsigned int i = 0; i < indicesLength; i++) {
      unsigned int j = i / (unsigned int)modelIndicesLength;
      indices[i] = indices[i % modelIndicesLength] + j * ((unsigned int)modelVertexCount / 3);
    }

    for (unsigned int i = 0; i < uvsLength; i++) {
      uvs[i] = uvs[i % modelUVsLength];
    }

    auto buffer = new Buffer(
      verticesLength * sizeof(GLfloat),
      indicesLength * sizeof(GLuint),
      verticesLength * sizeof(GLfloat),
      0,
      uvsLength * sizeof(GLfloat)
    );
    buffer->vbos.vertices->data = vertices;
    buffer->vbos.indices->data = indices;
    buffer->vbos.normals->data = normals;
    buffer->vbos.uvs->data = uvs;
    return buffer;
  }

  Spheres::Spheres(
    size_t splitCount,
    unsigned int maxModelCount,
    gpu::memories::Float3Memory* hostModelVerticesMemory,
    gpu::memories::Float3Memory* modelVerticesMemory,
    gpu::memories::Float3Memory* verticesMemory
  ) {
    this->splitCount = splitCount;
    this->modelCount = 0;
    this->maxModelCount = maxModelCount;
    this->hostModelVerticesMemory = hostModelVerticesMemory;
    this->modelVerticesMemory = modelVerticesMemory;
    this->verticesMemory = verticesMemory;
    initialize();
  }
}
