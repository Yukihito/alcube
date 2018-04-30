#ifndef ALCUBE_SCRIPTING_MAPPINGS_RENDERER_H
#define ALCUBE_SCRIPTING_MAPPINGS_RENDERER_H

#include "../../utils.h"
#include "../../../models/drawing/RenderingGroup.h"
#include "v8.h"

namespace alcube::scripting::mappings::drawing {
  class Renderer : public utils::TypedPrototype<models::drawing::RenderingGroup> {
    public:
      void init() override;
      DEFACCESSOR(models::drawing::RenderingGroup, glm::vec3, Diffuse);
      DEFACCESSOR(models::drawing::RenderingGroup, glm::vec3, Ambient);
      DEFACCESSOR(models::drawing::RenderingGroup, glm::vec3, Specular);
      DEFACCESSOR(models::drawing::RenderingGroup, models::drawing::Texture, Texture);
      DEFACCESSOR(models::drawing::RenderingGroup, models::drawing::InstanceColorType, InstanceColorType);
      static void setUpResources(const v8::FunctionCallbackInfo<v8::Value>&);
  };

  class RendererFactory : public utils::SingletonPrototype<models::drawing::RenderingGroupFactory> {
    public:
      void init() override;
      static void create(const v8::FunctionCallbackInfo<v8::Value>&);
  };
}

#endif //ALCUBE_SCRIPTING_MAPPINGS_RENDERER_H
