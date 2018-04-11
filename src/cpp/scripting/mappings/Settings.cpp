#include "Settings.h"

namespace alcube::scripting::mappings {
  using namespace alcube::scripting::utils;
  namespace PhysicsSettings {
    Prototype::Prototype(alcube::models::PhysicsSettings *underlying) : SingletonPrototype(underlying) {}

    void Prototype::init() {
      utils::Prototype::init();
      Accessor<models::PhysicsSettings, float, fields::timeStepSize>::define(this);
      Accessor<models::PhysicsSettings, float, fields::gravity>::define(this);
    }
  }

  namespace WindowSettings {
    Prototype::Prototype(alcube::models::WindowSettings *underlying) : SingletonPrototype(underlying) {
      Prototype::instance = this;
    }

    void Prototype::init() {
      utils::Prototype::init();
      Accessor<models::WindowSettings, int, fields::width>::define(this);
      Accessor<models::WindowSettings, int, fields::height>::define(this);
    }
  }

  namespace WorldSettings {
    Prototype::Prototype(alcube::models::WorldSettings *underlying) : SingletonPrototype(underlying) {
      Prototype::instance = this;
    }

    void Prototype::init() {
      utils::Prototype::init();
      Accessor<models::WorldSettings, float, fields::size>::define(this);
      Accessor<models::WorldSettings, int, fields::maxActorCount>::define(this);
    }
  }

  namespace Settings {
    Prototype::Prototype(alcube::models::Settings *underlying) : SingletonPrototype(underlying) {
      Prototype::instance = this;
    }

    void Prototype::init() {
      utils::Prototype::init();
      Accessor<models::Settings, int, fields::fps>::define(this);
    }
  }
}
