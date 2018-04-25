#ifndef ALCUBE_RENDERER_H
#define ALCUBE_RENDERER_H

#include "../../utils.h"
#include "../../../models/drawing/Renderer.h"
#include "v8.h"

namespace alcube::scripting::mappings::drawing {
  class Renderer : public utils::TypedPrototype<models::drawing::Renderer> {
    public:
      void init() override;
      DEFACCESSOR(models::drawing::Renderer, glm::vec3, Diffuse);
      DEFACCESSOR(models::drawing::Renderer, glm::vec3, Ambient);
      DEFACCESSOR(models::drawing::Renderer, glm::vec3, Specular);
      DEFACCESSOR(models::drawing::Renderer, models::drawing::Texture, Texture);
      DEFACCESSOR(models::drawing::Renderer, models::drawing::InstanceColorType, InstanceColorType);
      static void setUpResources(const v8::FunctionCallbackInfo<v8::Value>&);
  };

  class RendererFactory : public utils::SingletonPrototype<models::drawing::RendererFactory> {
    public:
      void init() override;
      static void create(const v8::FunctionCallbackInfo<v8::Value>&);
  };
}

#endif //ALCUBE_RENDERER_H
