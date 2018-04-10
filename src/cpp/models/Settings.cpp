#include "Settings.h"

namespace alcube::models {
  Settings::Settings() {
    fps = 60;
    physics.gravity = 9.8f;
    physics.timeStepSize = 1.0f / (float)fps;
    window.height = 1080;
    window.width = 1920;
    world.size = 64.0f;
    world.maxActorCount = 65536;
  }
}
