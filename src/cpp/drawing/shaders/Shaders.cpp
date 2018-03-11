#include "Shaders.h"

namespace alcube::drawing::shaders {
  Shaders::Shaders(utils::FileUtil* fileUtil, Context& context) :
    directionalLight(fileUtil, context){}
}