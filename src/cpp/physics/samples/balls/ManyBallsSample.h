#ifndef ALCUBE_MANYBALLSSAMPLE_H
#define ALCUBE_MANYBALLSSAMPLE_H

#include "../../../drawing/Drawable.h"
#include "../../Cell.h"
#include "../../Simulator.h"
#include "../../../drawing/Drawer.h"
#include "../../../drawing/shapes/Shapes.h"
#include "../../../drawing/shaders/Shaders.h"
#include "../../../utils/app/OpenGLApplication.h"
#include "../../../utils/Profiler.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/transform.hpp>
#include "BallsSample.h"

namespace alcube::physics::samples::balls {
  class ColorBall : public drawing::Drawable, public Cell {
    public:
      ColorBall(drawing::shapes::Shapes *shapes, drawing::shaders::Shaders *shaders, glm::vec3 diffuse);

    protected:
      glm::mat4 getModelMat() override;
      drawing::Material* getMaterial() override;
    private:
      drawing::Material* material;
  };

  class ManyBallsSample : public utils::app::OpenGLApplication {
    protected:
      void initWindowParams() override;

      void onInit() override;

      void onDraw() override;

      void onUpdate() override;

      void onClose() override;

    private:
      drawing::Drawer *drawer;
      drawing::Camera *camera;
      drawing::shaders::Shaders *shaders;
      drawing::shapes::Shapes *shapes;
      utils::opencl::Resources *resources;
      utils::FileUtil *fileUtil;
      utils::Profiler *profiler;
      Simulator *physicsSimulator;
      std::mutex mutex;
      Profilers profilers;
      unsigned int maxCellCount;
      float deltaTime;

      void add(ColorBall *ball);
  };
}
#endif //ALCUBE_MANYBALLSSAMPLE_H
