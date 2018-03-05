#include "Shaders.h"

namespace alcube::drawing::shaders {
  Shaders::Shaders(utils::FileUtil* fileUtil, Context& context) :
    //simple(fileUtil),
    //standard(fileUtil),
    //particle(fileUtil),
    //tile(fileUtil),
    directionalLight(fileUtil, context){}
}