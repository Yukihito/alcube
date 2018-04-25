#include "Actor.h"

namespace alcube::scripting::mappings {
  using namespace utils;

  void Actor::init() {
    Prototype::init();
    define<glm::vec3, Position>();
    define<glm::quat, Rotation>();
    define<glm::vec3, LinearMomentum>();
    define<glm::vec3, AngularMomentum>();
    define<glm::vec3, Color>();
  }

  void ActorFactory::init() {
    Prototype::init();
    DEFMETHOD(create);
  }

  void ActorFactory::create(const v8::FunctionCallbackInfo<v8::Value> &info) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);
    if (info.Length() < 2) {
      info.GetReturnValue().Set(v8::Undefined(isolate));
      return;
    }
    auto features = getUnderlying<models::physics::fluid::Features>(info[0]);
    auto renderer = getUnderlying<models::drawing::Renderer>(info[1]);
    auto underlying = self(info)->create(features, renderer);
    auto actor = Actor::instance->objectTemplate->NewInstance();
    actor->SetInternalField(0, v8::External::New(isolate, underlying));
    info.GetReturnValue().Set(actor);
  }
}
