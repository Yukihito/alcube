#ifndef ALCUBE_SIMPLESPHERESAMPLE_H
#define ALCUBE_SIMPLESPHERESAMPLE_H

#include "../Drawer.h"
#include "../shapes/Shapes.h"
#include "../shaders/Shaders.h"
#include "../../utils/app/OpenGLApplication.h"

namespace alcube::drawing::samples {
  class SimpleSphere : public Drawable {
    public:
      explicit SimpleSphere(glm::vec3& position, shapes::Shapes* shapes, shaders::Shaders* shaders);
      glm::vec3 position;
    protected:
      //glm::mat4 getModelMat() override;
      //Material* getMaterial() override;
      //Material* material;
  };

  class SimpleSphereSample : public utils::app::OpenGLApplication {
    public:
      explicit SimpleSphereSample();
    protected:
      void onInit() override;
      void onDraw() override;
      void onUpdate() override;
      void onClose() override;
    private:
      std::vector<SimpleSphere*> spheres;
      Drawer* drawer;
      Camera* camera;
      std::mutex drawablesMutex;
  };

}

#endif //ALCUBE_SIMPLESPHERESAMPLE_H
