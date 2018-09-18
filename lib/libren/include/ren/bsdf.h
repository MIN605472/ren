#ifndef REN_BRDF_H_
#define REN_BRDF_H_
#include "ren/surface_diff.h"
#include "ren/typedefs.h"
#include "ren/vec.h"
namespace ren {

// Calculate the Fresnel reflectance, i.e. the portion of the light that gets
// reflected.
// @param surface the surface of the object
// @param i the incident direction
// @param n1 the index of refraction incident medium
// @param n2 the index of refraction of the transmissive medium
Real FresnelReflectance(const SurfaceDiff &surface, const Vec3 &i, Real n1,
                        Real n2);

// Generic BSDF interface.
class Bsdf {
 public:
  enum Type {
    kReflective = 1 << 0,
    kTransmissive = 1 << 1,
    kDiffuse = 1 << 2,
    kGlossy = 1 << 3,
    kSpecular = 1 << 4,
    kAll = kReflective | kTransmissive | kDiffuse | kGlossy | kSpecular
  };
  Bsdf(Type type);

  // Evaluate the BSDF.
  // @param surface the differential surface at where it should be evaluated
  // @param w_o the outgoing light direction
  // @param w_i the incoming light direction
  // @param adjoint if the adjoint BSDF should be used
  // @return the value of evaluating the BSDF with the given parameters
  virtual Vec3 F(const SurfaceDiff &surface, const Vec3 &w_o, const Vec3 &w_i,
                 bool adjoint = false) const = 0;

  // Sample the BSDF.
  // @param surface the differential surface at where it should be evaluated
  // @param w_o the outgoing light direction
  // @param w_i the incoming light direction
  // @param pdf the probability of sampling the outgoing direction
  // @param adjoint if the adjoint BSDF should be used
  // @return the value of evaluating the BSDF with the given parameters
  virtual Vec3 SampleF(const SurfaceDiff &surface, const Vec3 &w_o, Vec3 &w_i,
                       Real &pdf, bool adjoint = false) const = 0;
  Bsdf::Type type_;
};

// Specular refraction and reflection BSDF.
class SpecularReflectionTransmission : public Bsdf {
 public:
  SpecularReflectionTransmission(Real n1, Real n2);
  virtual Vec3 F(const SurfaceDiff &surface, const Vec3 &w_o, const Vec3 &w_i,
                 bool adjoint = false) const override;
  virtual Vec3 SampleF(const SurfaceDiff &surface, const Vec3 &w_o, Vec3 &w_i,
                       Real &pdf, bool adjoint = false) const override;

 private:
  Real n1_;
  Real n2_;
};

// Diffuse or Lambertian BRDF.
class LambertianBrdf : public Bsdf {
 public:
  LambertianBrdf(const Vec3 &kd);
  virtual Vec3 F(const SurfaceDiff &surface, const Vec3 &w_o, const Vec3 &w_i,
                 bool adjoint = false) const override;
  virtual Vec3 SampleF(const SurfaceDiff &surface, const Vec3 &w_o, Vec3 &w_i,
                       Real &pdf, bool adjoint = false) const override;
  const Vec3 &kd() const;

 private:
  Vec3 kd_;
};

// The specular or glossy component of the Phong BRDF.
class PhongLobe : public Bsdf {
 public:
  PhongLobe(const Vec3 &ks, Real n);
  virtual Vec3 F(const SurfaceDiff &surface, const Vec3 &w_o, const Vec3 &w_i,
                 bool adjoint = false) const override;
  virtual Vec3 SampleF(const SurfaceDiff &surface, const Vec3 &w_o, Vec3 &w_i,
                       Real &pdf, bool adjoint = false) const override;
  const Vec3 &ks() const;

 private:
  Vec3 ks_;
  Real n_;
};

// The Phong BRDF.
class PhongBrdf : public Bsdf {
 public:
  PhongBrdf();
  PhongBrdf(const Vec3 &kd, const Vec3 &ks, Real n);
  Vec3 F(const SurfaceDiff &surface, const Vec3 &w_o, const Vec3 &w_i,
         bool adjoint = false) const;
  Vec3 SampleF(const SurfaceDiff &surface, const Vec3 &w_o, Vec3 &w_i,
               Real &pdf, bool adjoint = false) const;

 private:
  LambertianBrdf diffuse_component_;
  PhongLobe specular_component_;
};

}  // namespace ren
#endif  // REN_BRDF_H_
