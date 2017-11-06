#include "Drawer.h"

namespace drawing {
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

  void Drawer::draw() {
    for (Drawable* drawable: drawablesQueue) {
      addInternal(drawable);
    }
    drawablesQueueMutex.lock();
    drawablesQueue.clear();
    drawablesQueueMutex.unlock();

    glm::mat4 projection =
      glm::perspective(
        camera->angleOfView, // 視界
        camera->aspectRatio, // アス比
        camera->near, // 手前
        camera->far // 奥
      );

    glm::mat4 view =  glm::inverse(glm::toMat4(camera->rotation)) * glm::translate(-camera->position);
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
          drawablesMutex->lock();
          drawable->draw(context);
          drawablesMutex->unlock();
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
      }
    }

    glFlush();
  }

  Drawer::Drawer(Camera* camera, std::mutex *drawablesMutex) {
    this->drawablesMutex = drawablesMutex;
    this->camera = camera;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }
}

