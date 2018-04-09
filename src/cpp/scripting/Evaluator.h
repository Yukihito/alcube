#ifndef ALCUBE_EVALUATOR_H
#define ALCUBE_EVALUATOR_H

#include "libplatform/libplatform.h"
#include "v8.h"
#include "../models/Actor.h"
#include "mappings/Actor.h"
#include "mappings/physics/fluid/Features.h"
#include "../models/physics/softbody/Spring.h"
#include <vector>
#include "mappings/physics/softbody/Spring.h"
#include "mappings/physics/softbody/Features.h"

namespace alcube::scripting {
  class Evaluator {
    public:
      explicit Evaluator(
        models::ActorFactory* actorFactory,
        alcube::models::physics::fluid::FeaturesFactory* featuresFactory,
        alcube::models::physics::softbody::SpringFactory* springFactory,
        alcube::models::physics::softbody::FeaturesFactory* softbodyFeaturesFactory,
        alcube::utils::FileUtil* fileUtil,
        const char* programName
      );

      void evaluate(const char* path);
      void withScope(std::function<void(Evaluator*)> f);

    private:
      const char* programName;
      alcube::utils::FileUtil* fileUtil;
      v8::Isolate* isolate;
      v8::Platform* platform;
      v8::Isolate::CreateParams createParams;
      v8::Local<v8::ObjectTemplate> global;
      v8::Local<v8::Context> context;
      std::vector<utils::Prototype*> prototypes;
      static void print(const v8::FunctionCallbackInfo<v8::Value> &args);
      void initV8();
      void loadLibs();
      void loadLib(const char* filePath);
      void initTemplates();
      void registerFunctions();
      void registerFunction(const char* name, v8::FunctionCallback f);
  };
}

#endif //ALCUBE_EVALUATOR_H
