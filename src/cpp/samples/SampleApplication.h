#ifndef ALCUBE_SAMPLEAPPLICATION_H
#define ALCUBE_SAMPLEAPPLICATION_H

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/transform.hpp>
#include "ApplicationBase.h"
#include "SphereDrawable.h"

namespace alcube::samples {
  class SampleApplication : public ApplicationBase {
    public:
      explicit SampleApplication();
    protected:
      void onInit() override;
    private:
      SphereDrawable* drawable;
      unsigned short actorCount;
      void add(physics::SoftBodyActor *actor);
      void add(physics::FluidActor *actor);
  };
}


#endif //ALCUBE_SAMPLEAPPLICATION_H
