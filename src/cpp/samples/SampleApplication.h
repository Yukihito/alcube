#ifndef ALCUBE_SAMPLEAPPLICATION_H
#define ALCUBE_SAMPLEAPPLICATION_H

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/transform.hpp>
#include "ApplicationBase.h"
#include "SphereDrawable.h"
#include "../models/physics/fluid/Features.h"
#include "../models/physics/softbody/Features.h"

namespace alcube::samples {
  class SampleApplication : public ApplicationBase {
    public:
      explicit SampleApplication(const char* programName);
    protected:
      void onInit() override;
    private:
      SphereDrawable* drawable;
  };
}


#endif //ALCUBE_SAMPLEAPPLICATION_H
