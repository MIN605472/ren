#include "ren/object.h"

using namespace ren;

Object::Object(std::unique_ptr<Shape> shape, std::unique_ptr<Bsdf> bsdf,
               AreaLight* area_light)
    : shape_(std::move(shape)),
      bsdf_(std::move(bsdf)),
      area_light_(area_light) {}

bool Object::Intersect(const Ray& ray, Real& t, SurfaceDiff& surface_diff) {
  if (shape_->Intersect(ray, t, surface_diff)) {
    surface_diff.o = this;
    return true;
  }
  return false;
}

const Bsdf& Object::bsdf() const { return *bsdf_; }

const AreaLight* Object::area_light() const { return area_light_; }
