#ifndef ALCUBE_SIMPLESPHERESAMPLE_H
#define ALCUBE_SIMPLESPHERESAMPLE_H

#include <GLUT/glut.h>
#include "../Drawer.h"
#include "../shapes/Shapes.h"
#include "../shaders/Shaders.h"
#include "../../utils/app/OpenGLApplication.h"

namespace alcube::drawing::samples {
  class SimpleSphere : public Drawable {
    public:
      explicit SimpleSphere(glm::vec3& position, shapes::Shapes* shapes, shaders::Shaders* shaders);
    protected:
      glm::mat4 getModelMat() override;
      glm::vec3 position;
  };

  class SimpleSphereSample : public utils::app::OpenGLApplication {
    protected:
      void initWindowParams() override;
      void onInit() override;
      void onDraw() override;
      void onUpdate() override;
      void onClose() override;
    private:
      Drawer* drawer;
      Camera* camera;
      std::mutex drawablesMutex;
  };

}

#endif //ALCUBE_SIMPLESPHERESAMPLE_H
