#include <CL/cl_platform.h>
#include "Drawer.h"

namespace alcube::drawing {
  void Drawer::addInternal(Drawable* drawable) {
    if (drawables[drawable->shader] == nullptr) {
      drawables[drawable->shader] = new std::unordered_map<Shape*, std::vector<Drawable*>*>();
      (*drawables[drawable->shader])[drawable->shape] = new std::vector<Drawable*> {drawable};
    } else if ((*drawables[drawable->shader])[drawable->shape] == nullptr) {
      (*drawables[drawable->shader])[drawable->shape] = new std::vector<Drawable*> {drawable};
    } else {
      (*drawables[drawable->shader])[drawable->shape]->push_back(drawable);
    }
  }

  void Drawer::add(Drawable* drawable) {
    drawablesQueueMutex.lock();
    drawable->bufferIndex = drawableBufferIndex;
    drawablesQueue.push_back(drawable);
    if (drawable->isGroup) {
      groupDrawables.push_back(drawable);
    }
    drawablesQueueMutex.unlock();
  }

  void Drawer::draw() {
    for (Drawable* drawable: drawablesQueue) {
      addInternal(drawable);
    }
    drawablesQueueMutex.lock();
    drawablesQueue.clear();
    drawablesQueueMutex.unlock();

    glm::mat4 projection =
      glm::perspective(
        camera->angleOfView,
        camera->aspectRatio,
        camera->near,
        camera->far
      );

    glm::mat4 view = glm::toMat4(glm::inverse(camera->rotation)) * glm::translate(-camera->position);
    glm::mat4 vp = projection * view;
    Context context;
    context.v = view;
    context.vp = vp;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto shaderShapesDrawables : drawables) {
      Shader* shader = shaderShapesDrawables.first;
      auto shapesDrawables = *shaderShapesDrawables.second;
      glUseProgram(shader->programId);
      for (auto shapeDrawables : shapesDrawables) {
        Shape* shape = shapeDrawables.first;
        Buffer *buffer = shape->buffer;
        glBindVertexArray(buffer->arrayId);
        shader->setupBuffer(buffer);
        auto drawables = *shapeDrawables.second;
        for (Drawable* drawable: drawables) {
          drawable->draw(context);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
      }
    }
    drawablesMutex->lock();
    drawableBufferIndex[0] = drawableBufferIndex[0] == 0 ? 1 : 0;
    drawablesMutex->unlock();
  }

  void Drawer::updateDrawableBuffers() {
    drawablesMutex->lock();
    for (auto shaderShapesDrawables : drawables) {
      auto shapesDrawables = *shaderShapesDrawables.second;
      for (auto shapeDrawables : shapesDrawables) {
        auto drawables = *shapeDrawables.second;
        for (Drawable *drawable: drawables) {
          drawable->updateBuffer();
        }
      }
    }
    drawablesMutex->unlock();
  }

  Drawer::Drawer(
    Camera* camera,
    std::mutex *drawablesMutex,
    gpu::GPU* gpu
  ) {
    if (gpu != nullptr) {
      kernels = gpu->kernels;
      memories = gpu->memories;
    }
    this->drawablesMutex = drawablesMutex;
    this->camera = camera;
    drawableBufferIndex[0] = 0;


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }

  void Drawer::setUpGroupDrawables() {
    for (auto drawable : groupDrawables) {
      auto groupShape = (GroupShape*)drawable->shape;
      groupShape->hostModelVerticesMemory->setCount(groupShape->modelCount); // やばい
      cl_float3* modelVerticesDto = groupShape->hostModelVerticesMemory->at(0);
      auto positions = groupShape->modelVertices;
      for (size_t i = 0; i < groupShape->modelVertexCount; i++) {
        size_t j = i * 3;
        modelVerticesDto[i] = {positions[j + 0], positions[j + 1], positions[j + 2]};
      }
      groupShape->hostModelVerticesMemory->write();
      kernels.inputModelVertices(
        (unsigned int)groupShape->modelVertexCount,
        *groupShape->hostModelVerticesMemory,
        *groupShape->modelVerticesMemory
      );
    }
  }

  void Drawer::updateGroupDrawables() {
    for (auto drawable : groupDrawables) {
      auto groupShape = (GroupShape*)drawable->shape;
      groupShape->verticesMemory->read();
      auto vertices = groupShape->verticesMemory->at(0);
      size_t vertexCount = groupShape->modelVertexCount * groupShape->modelCount;
      for (size_t i = 0; i < vertexCount; i++) {
        groupShape->modelVertices[i * 3 + 0] = vertices[i].x;
        groupShape->modelVertices[i * 3 + 1] = vertices[i].y;
        groupShape->modelVertices[i * 3 + 2] = vertices[i].z;
      }
    }
  }
}

