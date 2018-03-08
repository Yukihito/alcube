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
    drawablesQueueMutex.unlock();
  }

  void Drawer::waitVSync() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void Drawer::addWaitingDrawables() {
    drawablesQueueMutex.lock();
    for (Drawable* drawable: drawablesQueue) {
      addInternal(drawable);
    }
    drawablesQueue.clear();
    drawablesQueueMutex.unlock();
  }

  void Drawer::initContext() {
    glm::mat4 projection =
      glm::perspective(
        camera->angleOfView,
        camera->aspectRatio,
        camera->near,
        camera->far
      );

    glm::mat4 view = glm::toMat4(glm::inverse(camera->rotation)) * glm::translate(-camera->position);
    glm::mat4 vp = projection * view;

    context.v = view;
    context.vp = vp;
  }

  void Drawer::drawAllDrawables() {
    for (auto shaderShapesDrawables : drawables) {
      Shader* shader = shaderShapesDrawables.first;
      auto shapesDrawables = *shaderShapesDrawables.second;
      glUseProgram(shader->programId);
      for (auto shapeDrawables : shapesDrawables) {
        Shape* shape = shapeDrawables.first;
        shape->update();
        shader->bindShape(shape);
        auto drawables = *shapeDrawables.second;
        for (Drawable* drawable: drawables) {
          drawable->draw(context);
        }
        shader->unbindShape(shape);
      }
    }
    glFinish(); // TODO: Use only if profiler enabled.
  }

  void Drawer::draw() {
    addWaitingDrawables();
    initContext();
    drawAllDrawables();
  }

  Drawer::Drawer(
    Camera* camera,
    gpu::GPU* gpu
  ) {
    kernels = gpu->kernels;
    memories = gpu->memories;
    this->camera = camera;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }
}
