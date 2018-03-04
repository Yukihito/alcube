#include "MultiSphere.h"

namespace alcube::drawing::shapes::triangles {
  void MultiSphere::draw() {
    glDrawElementsInstanced(GL_TRIANGLES, (int)this->indicesLength, GL_UNSIGNED_INT, nullptr, instanceCount);
  }

  Buffer* MultiSphere::createBuffer() {

    return buffer;
  }

  MultiSphere::MultiSphere(
    size_t splitCount,
    unsigned int maxInstanceCount,
    gpu::memories::Float3Memory* positionsMemory
  ) : MultiShape() {
    this->splitCount = splitCount;
    this->instanceCount = 0;
    this->maxInstanceCount = maxInstanceCount;
    this->positionsMemory = positionsMemory;

    auto iSplitCount = (int)splitCount;
    auto verticesLength = (unsigned int)(splitCount * splitCount);
    size_t vertexElementsLength = verticesLength * 3;
    size_t uvsLength = verticesLength * 2;
    auto vertices = new GLfloat[vertexElementsLength]();
    auto normals = new GLfloat[vertexElementsLength]();
    auto uvs = new GLfloat[uvsLength];
    float pi = 3.1415f;
    for (int i = 0; i < splitCount; i++) {
      float y = (float)i / (float)splitCount;
      float r = std::sin(y * pi);
      for (int j = 0; j < splitCount; j++) {
        float x = (float)j / (float)splitCount;
        size_t k = (i * splitCount + j) * 3;
        vertices[k + 0] = std::cos(x * pi * 2) * r;
        vertices[k + 1] = std::cos(y * pi);
        vertices[k + 2] = std::sin(x * pi * 2) * r;
        normals[k + 0] = std::cos(x * pi * 2) * r;
        normals[k + 1] = std::cos(y * pi);
        normals[k + 2] = std::sin(x * pi * 2) * r;
      }
    }
    this->indicesLength = splitCount * splitCount * 6;
    auto indices = new GLuint[indicesLength]();
    int indicesIndex = 0;
    for (int i = 0; i < splitCount; i++) {
      for (int j = 0; j < splitCount; j++) {
        indices[indicesIndex + 0] = (uint)(i * iSplitCount + j);
        indices[indicesIndex + 2] = (uint)((i + 1) * iSplitCount + j);
        indices[indicesIndex + 1] = (uint)(i * iSplitCount + j + 1);
        indices[indicesIndex + 3] = (uint)(i * iSplitCount + j + 1);
        indices[indicesIndex + 5] = (uint)((i + 1) * iSplitCount + j);
        indices[indicesIndex + 4] = (uint)((i + 1) * iSplitCount + j + 1);
        indicesIndex += 6;
        size_t l = (i * splitCount + j) * 2;
        uvs[l + 0] = (float)i / (float)(splitCount - 1);
        uvs[l + 1] = (float)j / (float)(splitCount - 1);
      }
    }

    indexBuffer = new IndexBuffer(1, (unsigned int)indicesLength, indices);
    vertexBuffers[VERTICES] = new VertexBuffer(3, verticesLength, vertices);
    vertexBuffers[NORMALS] = new VertexBuffer(3, verticesLength, normals);
    vertexBuffers[UVS] = new VertexBuffer(2, verticesLength, uvs);
    instanceBuffers[POSITIONS] = new InstanceBuffer(3, maxInstanceCount, new GLfloat[maxInstanceCount * 3]);

    indexBuffer->update();
    vertexBuffers[VERTICES]->update();
    vertexBuffers[NORMALS]->update();
    vertexBuffers[UVS]->update();
    /*
    auto buffer = new Buffer(
      verticesLength * sizeof(GLfloat),
      indicesLength * sizeof(GLuint),
      verticesLength * sizeof(GLfloat),
      0,
      uvsLength * sizeof(GLfloat),
      maxInstanceCount * sizeof(GLfloat) * 3
    );
    buffer->vertices->data = vertices;
    buffer->indices->data = indices;
    buffer->normals->data = normals;
    buffer->uvs->data = uvs;
    buffer->positions->data = new GLfloat[maxInstanceCount * 3];
    buffer->vertices->update();
    buffer->indices->update();
    buffer->normals->update();
    buffer->uvs->update();
     */
    //initialize();
  }
}
