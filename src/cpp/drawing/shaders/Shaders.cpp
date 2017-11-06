#include "Shaders.h"

namespace drawing::shaders {
  Shaders::Shaders(utils::FileUtil* fileUtil) : simple(fileUtil), standard(fileUtil) {}
}