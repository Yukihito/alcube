#ifndef ALCUBE_SCRIPTING_MAPPINGS_RENDERER_H
#define ALCUBE_SCRIPTING_MAPPINGS_RENDERER_H

#include "../../utils.h"
#include "../../../models/drawing/Renderer.h"
#include "RenderingGroup.h"

namespace alcube::scripting::mappings::drawing {
  class Renderer : public utils::SingletonPrototype<models::drawing::Renderer> {
    public:
      void init() override;
      static void createGroup(const v8::FunctionCallbackInfo<v8::Value>& info);
  };
}

#endif //ALCUBE_SCRIPTING_MAPPINGS_RENDERER_H
