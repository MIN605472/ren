#define _USE_MATH_DEFINES
#include "ren/scene.h"
#include <limits>
#include "ren/plane.h"
#include "ren/sphere.h"
#include "ren/transform.h"

using namespace ren;

bool Scene::Intersect(const Ray &ray, SurfaceDiff &surface_diff) const {
  Ray r(ray);
  bool intersected = false;
  for (const auto &object : objects_) {
    SurfaceDiff tmp_surface_diff;
    Real tmp_t;
    if (object->Intersect(r, tmp_t, tmp_surface_diff)) {
      surface_diff = tmp_surface_diff;
      r.set_tmax(tmp_t);
      intersected = true;
    }
  }
  return intersected;
}

const std::vector<std::unique_ptr<Light>> &Scene::lights() const {
  return lights_;
}

void Scene::AddObject(std::unique_ptr<Object> o) {
  objects_.push_back(std::move(o));
}

void Scene::AddLight(std::unique_ptr<Light> l) {
  lights_.push_back(std::move(l));
}

bool Scene::AnyObjectWithBsdf(Bsdf::Type type) const {
  for (const auto &o : objects_) {
    if (o->bsdf().type_ & type) {
      return true;
    }
  }
  return false;
}
