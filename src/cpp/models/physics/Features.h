#ifndef ALCUBE_PHYSICSFEATURE_H
#define ALCUBE_PHYSICSFEATURE_H

namespace alcube::models::physics {
  class Features {
    public:
      virtual int getId() = 0;
      virtual float getMass() = 0;
  };

  class SoftBodyFeatures : public Features {
    public:
      virtual float getElasticity() = 0;
      virtual void setElasticity(float arg) = 0;
      virtual void setMass(float arg) = 0;
  };

  class FluidFeatures : public Features {
    public:
      virtual float getDensity() = 0;
      virtual void setDensity(float arg) = 0;
      virtual float getStiffness() = 0;
      virtual void setStiffness(float arg) = 0;
      virtual float getViscosity() = 0;
      virtual void setViscosity(float arg) = 0;
  };

  class FeaturesFactory {
    public:
      virtual SoftBodyFeatures* createSoftBodyFeatures() = 0;
      virtual FluidFeatures* createFluidFeatures() = 0;
  };

  class LinkFeatures {
      virtual int getId() = 0;
  };

  class SpringLinkFeatures : public LinkFeatures {
    public:
      virtual float getK() = 0;
  };
}

#endif //ALCUBE_PHYSICSFEATURE_H
