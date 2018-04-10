#ifndef ALCUBE_SCRIPTING_MAPPINGS_SETTINGS_H
#define ALCUBE_SCRIPTING_MAPPINGS_SETTINGS_H

#include "../utils.h"
#include "../../models/Settings.h"

namespace alcube::scripting::mappings {
  namespace PhysicsSettings {
    namespace fields {
      class timeStepSize {};
      class gravity {};
    }

    class Prototype : public utils::SingletonPrototype<models::PhysicsSettings> {
      public:
        explicit Prototype(models::PhysicsSettings* underlying);
        void init() override;
        static Prototype* instance;
        static void constructor(const v8::FunctionCallbackInfo<v8::Value>& info);
    };
  }

  namespace WindowSettings {
    namespace fields {
      class width {};
      class height {};
    }

    class Prototype : public utils::SingletonPrototype<models::WindowSettings> {
      public:
        explicit Prototype(models::WindowSettings* underlying);
        void init() override;
        static Prototype* instance;
        static void constructor(const v8::FunctionCallbackInfo<v8::Value>& info);
    };
  }

  namespace WorldSettings {
    namespace fields {
      class size {};
      class maxActorCount {};
    }

    class Prototype : public utils::SingletonPrototype<models::WorldSettings> {
      public:
        explicit Prototype(models::WorldSettings* underlying);
        void init() override;
        static Prototype* instance;
        static void constructor(const v8::FunctionCallbackInfo<v8::Value>& info);
    };
  }

  namespace Settings {
    namespace fields {
      class fps {};
    }

    class Prototype : public utils::SingletonPrototype<models::Settings> {
      public:
        explicit Prototype(models::Settings* underlying);
        void init() override;
        static Prototype* instance;
        static void constructor(const v8::FunctionCallbackInfo<v8::Value>& info);
    };
  }
}

#endif //ALCUBE_SCRIPTING_MAPPINGS_SETTINGS_H
