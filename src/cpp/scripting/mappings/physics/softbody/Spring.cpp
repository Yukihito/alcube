#include "Spring.h"

namespace alcube::scripting::mappings::physics::softbody {
  using namespace utils;
  namespace Spring {
    Prototype* Prototype::instance;
    Prototype::Prototype() {
      Prototype::instance = this;
    }
  }

  namespace SpringFactory {
    Prototype* Prototype::instance;
    Prototype::Prototype(models::physics::softbody::SpringFactory *underlying) : utils::SingletonPrototype<models::physics::softbody::SpringFactory>(underlying) {
      Prototype::instance = this;
    }

    void Prototype::init() {
      utils::Prototype::init();
      Accessor<models::physics::softbody::SpringFactory, float, fields::k>::define(this);
      v8::Isolate* isolate = v8::Isolate::GetCurrent();
      objectTemplate->Set(
        v8::String::NewFromUtf8(isolate, "create"),
        v8::FunctionTemplate::New(isolate, Prototype::create)
      );
    }

    void Prototype::constructor(const v8::FunctionCallbackInfo<v8::Value> &info) {
      v8::Isolate *isolate = v8::Isolate::GetCurrent();
      auto v8Instance = Prototype::instance->objectTemplate->NewInstance();
      v8Instance->SetInternalField(0, v8::External::New(isolate, Prototype::instance->underlying));
      info.GetReturnValue().Set(v8Instance);
    }

    void Prototype::create(const v8::FunctionCallbackInfo<v8::Value> &info) {
      v8::Isolate* isolate = v8::Isolate::GetCurrent();
      v8::HandleScope scope(isolate);
      if (info.Length() < 4) {
        info.GetReturnValue().Set(v8::Undefined(isolate));
      }
      auto factory = self(info);
      auto actor0 = getUnderlying<models::Actor>(info[0]);
      auto pos0 = convertV8ValueTo<glm::vec3>(info[1]);
      auto actor1 = getUnderlying<models::Actor>(info[2]);
      auto pos1 = convertV8ValueTo<glm::vec3>(info[3]);
      auto spring = factory->create(actor0, pos0, actor1, pos1);
      auto wrapper = Spring::Prototype::instance->objectTemplate->NewInstance();
      wrapper->SetInternalField(0, v8::External::New(isolate, spring));
      info.GetReturnValue().Set(wrapper);
    }
  }
}

namespace alcube::scripting::utils {
  using namespace mappings::physics::softbody::SpringFactory;

  template <>
  float Accessor<models::physics::softbody::SpringFactory, float, fields::k>::get(alcube::models::physics::softbody::SpringFactory* factory) {
    return factory->getK();
  }

  template <>
  void Accessor<models::physics::softbody::SpringFactory, float, fields::k>::set(alcube::models::physics::softbody::SpringFactory* factory, float k) {
    factory->setK(k);
  }
}
