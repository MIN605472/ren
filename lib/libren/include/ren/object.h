#ifndef REN_OBJECT_H_
#define REN_OBJECT_H_
#include <memory>
#include "ren/area_light.h"
#include "ren/bsdf.h"
#include "ren/shape.h"
#include "ren/surface_diff.h"
namespace ren {
// Class representing an object in the scene.
class Object {
 public:
  // Construct a new object.
  // @param shape the shape associated with the object, i.e., geometric
  // information
  // @param bsdf the BSDF of the object, i.e., the material of the object
  // @param ara_light if the oject is an emitter then it should have pointer to
  // some AreaLight describing the emission properties
  Object(std::unique_ptr<Shape> shape, std::unique_ptr<Bsdf> bsdf,
         AreaLight *area_light = nullptr);
  // Test for interesection with a given ray.
  // @param ray the ray to test with
  // @param t the distance along the ray where it intersected
  // @param surface_diff the geometric and material information about the
  // intersction point
  // @return true if the ray \p ray intersected with this shape, false otherwise
  bool Intersect(const Ray &ray, Real &t, SurfaceDiff &surface_diff);
  const Bsdf &bsdf() const;
  const AreaLight *area_light() const;

 private:
  std::unique_ptr<Shape> shape_;
  std::unique_ptr<Bsdf> bsdf_;
  AreaLight *area_light_;
};
}  // namespace ren
#endif  // REN_OBJECT_H_
