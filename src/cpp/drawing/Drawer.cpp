#include "Drawer.h"

namespace alcube::drawing {
  void Drawer::addInternal(Drawable* drawable) {
    if (shaderToDrawables[drawable->shader] == nullptr) {
      shaderToDrawables[drawable->shader] = new std::vector<Drawable*>();
    }
    shaderToDrawables[drawable->shader]->push_back(drawable);
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
    for (auto kv : shaderToDrawables) {
      auto shader = kv.first;
      auto drawables = *kv.second;
      glUseProgram(shader->programId);
      for (auto drawable : drawables) {
        drawable->draw(context);
      }
    }
  }

  void Drawer::draw() {
    waitVSync();
    addWaitingDrawables();
    initContext();
    drawAllDrawables();
  }

  Drawer::Drawer(Camera* camera) {
    this->camera = camera;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }
}
