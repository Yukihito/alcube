#ifndef ALCUBE_SCRIPTING_MAPPINGS_SETTINGS_H
#define ALCUBE_SCRIPTING_MAPPINGS_SETTINGS_H

#include "../utils.h"
#include "../../models/Settings.h"

namespace alcube::scripting::mappings {
  class PhysicsSettings : public utils::SingletonPrototype<models::PhysicsSettings> {
    public:
      //explicit PhysicsSettings(models::PhysicsSettings* underlying);
      void init() override;
      DEFVAR(models::PhysicsSettings, float, timeStepSize);
      DEFVAR(models::PhysicsSettings, float, gravity);
  };

  class WindowSettings : public utils::SingletonPrototype<models::WindowSettings> {
    public:
      //explicit WindowSettings(models::WindowSettings* underlying);
      void init() override;
      DEFVAR(models::WindowSettings, int, width);
      DEFVAR(models::WindowSettings, int, height);
  };

  class WorldSettings : public utils::SingletonPrototype<models::WorldSettings> {
    public:
      //explicit WorldSettings(models::WorldSettings* underlying);
      void init() override;
      DEFVAR(models::WorldSettings, float, size);
      DEFVAR(models::WorldSettings, int, maxActorCount);
  };

  class Settings : public utils::SingletonPrototype<models::Settings> {
    public:
      //explicit Settings(models::Settings* underlying);
      void init() override;
      DEFVAR(models::Settings, int, fps);
  };
}

#endif //ALCUBE_SCRIPTING_MAPPINGS_SETTINGS_H
