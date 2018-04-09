#include <map>
#include "../../utils/FileUtil.h"
#include "../../scripting/mappings/Actor.h"
#include "../../scripting/Evaluator.h"

int main(int argc, char * argv[]) {
  auto fileUtil = new alcube::utils::FileUtil();
  auto actorFactory = new alcube::models::ActorFactory(new alcube::utils::MemoryPool<alcube::models::Actor>(65536));
  auto fluidFeaturesFactory = new alcube::models::physics::fluid::FeaturesFactory(new alcube::utils::MemoryPool<alcube::models::physics::fluid::Features>(65536));
  auto springFactory = new alcube::models::physics::softbody::SpringFactory(new alcube::utils::MemoryPool<alcube::models::physics::softbody::Spring>(65536));
  auto softbodyFeaturesFactory = new alcube::models::physics::softbody::FeaturesFactory(new alcube::utils::MemoryPool<alcube::models::physics::softbody::Features>(65536));
  /*
  auto evaluator = new alcube::scripting::Evaluator(actorFactory, fluidFeaturesFactory, springFactory, softbodyFeaturesFactory, fileUtil, argv[0]);
  evaluator->withScope([](alcube::scripting::Evaluator* e) {
    e->evaluate("../src/js/test.js");
  });
   */
}
