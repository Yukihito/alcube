#ifndef ALCUBE_SETTINGS_H
#define ALCUBE_SETTINGS_H

namespace alcube::models {
  struct PhysicsSettings {
      float timeStepSize;
      float gravity;
  };

  struct WindowSettings {
      unsigned int width;
      unsigned int height;
  };

  struct WorldSettings {
      float size;
      unsigned int maxActorCount;
  };

  struct Settings {
      explicit Settings();
      unsigned int fps;
      PhysicsSettings physics;
      WindowSettings window;
      WorldSettings world;
  };
}

#endif //ALCUBE_SETTINGS_H
