#ifndef ALCUBE_SIMPLESPHERESAMPLE_H
#define ALCUBE_SIMPLESPHERESAMPLE_H

#include <GLUT/glut.h>
#include "../Drawer.h"
#include "../shapes/Shapes.h"
#include "../shaders/Shaders.h"

namespace drawing::samples {
  class SimpleSphere : public Drawable {
    public:
      explicit SimpleSphere(glm::vec3& position, shapes::Shapes* shapes, shaders::Shaders* shaders);
    protected:
      glm::mat4 getModelMat() override;
      glm::vec3 position;
  };

  class SimpleSphereSample {
    public:
      void runApp(int argc, char *argv[]);
    private:
      static const int WINDOW_WIDTH = 800;
      static const int WINDOW_HEIGHT = 600;
      Drawer* drawer;
      Camera* camera;
      std::mutex drawablesMutex;
      void setupWindow(int argc, char* argv[]);
      void setEventListeners();
      static void drawEvent();
      static void keyEvent(unsigned char key, int x, int y);
  };

}

#endif //ALCUBE_SIMPLESPHERESAMPLE_H
