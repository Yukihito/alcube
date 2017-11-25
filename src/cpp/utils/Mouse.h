#ifndef ALCUBE_MOUSE_H
#define ALCUBE_MOUSE_H

namespace alcube::utils {
  class Mouse {
    public:
      int x;
      int y;
      int dx;
      int dy;
      void update();
      void onMouseMove(int x, int y);

    private:
      int lastX;
      int lastY;
  };
}

#endif //ALCUBE_MOUSE_H
