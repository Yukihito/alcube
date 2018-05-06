#ifndef ALCUBE_SCRIPTING_MAPPINGS_RENDERING_GROUP_H
#define ALCUBE_SCRIPTING_MAPPINGS_RENDERING_GROUP_H

#include "../../utils.h"
#include "../../../models/drawing/RenderingGroup.h"
#include "v8.h"
#include "../../../models/Actor.h"

namespace alcube::scripting::mappings::drawing {
  class RenderingGroup : public utils::TypedPrototype<models::drawing::RenderingGroup> {
    public:
      void init() override;
      DEFACCESSOR(models::drawing::RenderingGroup, glm::vec3, Diffuse);
      DEFACCESSOR(models::drawing::RenderingGroup, glm::vec3, Ambient);
      DEFACCESSOR(models::drawing::RenderingGroup, glm::vec3, Specular);
      DEFACCESSOR(models::drawing::RenderingGroup, models::drawing::Texture, Texture);
      DEFACCESSOR(models::drawing::RenderingGroup, models::drawing::InstanceColorType, InstanceColorType);
  };
}

#endif //ALCUBE_SCRIPTING_MAPPINGS_RENDERER_H
