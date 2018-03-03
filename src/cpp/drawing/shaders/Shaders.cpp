#include "Shaders.h"

namespace alcube::drawing::shaders {
  Shaders::Shaders(utils::FileUtil* fileUtil) :
    simple(fileUtil),
    standard(fileUtil),
    particle(fileUtil),
    tile(fileUtil),
    directionalLight(fileUtil){}
}