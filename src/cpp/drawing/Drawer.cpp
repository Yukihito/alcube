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
    drawablesQueue.push_back(drawable);
    if (drawable->isMulti) {
      multiDrawables.push_back(drawable);
    }
    drawablesQueueMutex.unlock();
  }

  void Drawer::waitVSync() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

    for (auto shaderShapesDrawables : drawables) {
      Shader* shader = shaderShapesDrawables.first;
      auto shapesDrawables = *shaderShapesDrawables.second;
      glUseProgram(shader->programId);
      for (auto shapeDrawables : shapesDrawables) {
        Shape* shape = shapeDrawables.first;
        Buffer *buffer = shape->buffer;
        //glBindVertexArray(buffer->arrayId);
        shader->setupBuffer(buffer);
        auto drawables = *shapeDrawables.second;
        for (Drawable* drawable: drawables) {
          drawable->draw(context);
        }
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glDisableVertexAttribArray(0);
        //glBindVertexArray(0);
      }
    }
    glFinish(); // TODO: Enable if profiler enabled.
  }

  Drawer::Drawer(
    Camera* camera,
    gpu::GPU* gpu
  ) {
    if (gpu != nullptr) {
      kernels = gpu->kernels;
      memories = gpu->memories;
    }
    this->camera = camera;


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }

  void Drawer::setUpMultiDrawables() {
    for (auto drawable : multiDrawables) {
      auto multiShape = (MultiShape*)drawable->shape;
      multiShape->buffer->vbos.positions->size = multiShape->instanceCount * sizeof(GLfloat);
      multiShape->positionsMemory->setCount(multiShape->instanceCount);
    }
  }

  void Drawer::transformMultiDrawables() {
    for (auto drawable : multiDrawables) {
      auto multiShape = (MultiShape*)drawable->shape;
      kernels.outputPositions(
        multiShape->instanceCount,
        *multiShape->positionsMemory,
        memories.physicalQuantities
      );
    }
  }

  void Drawer::updateMultiDrawables() {
    for (auto drawable : multiDrawables) {
      auto multiShape = (MultiShape*)drawable->shape;
      multiShape->positionsMemory->read();
      auto clPositions = multiShape->positionsMemory->at(0);
      auto positions = (float*)multiShape->buffer->vbos.positions->data;
      multiShape->buffer->vbos.positions->size = sizeof(GLfloat) * multiShape->instanceCount * 3;
      for (size_t i = 0; i < multiShape->instanceCount; i++) {
        positions[i * 3 + 0] = clPositions[i].x;
        positions[i * 3 + 1] = clPositions[i].y;
        positions[i * 3 + 2] = clPositions[i].z;
      }
    }
  }
}
