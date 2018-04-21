#include "Shaders.h"

namespace alcube::drawing::shaders {
  DirectionalLight::DirectionalLight(utils::FileUtil *fileUtil, Context &context) :
    singleColor(fileUtil, context),
    instanceColor(fileUtil, context),
    textureSingleColor(fileUtil, context) {}

  Shaders::Shaders(utils::FileUtil* fileUtil, Context& context) : directionalLight(fileUtil, context) {}
}