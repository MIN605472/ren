#define _USE_MATH_DEFINES
#include "ren/bsdf.h"
#include <algorithm>
#include <cmath>
#include "ren/rng.h"
#include "ren/sampling.h"
#include "ren/transform.h"

using namespace ren;

Bsdf::Bsdf(Type type) : type_(type) {}

LambertianBrdf::LambertianBrdf(const Vec3 &kd)
    : Bsdf(Type(kReflective | kDiffuse)), kd_(kd) {}

Vec3 LambertianBrdf::F(const SurfaceDiff &surface, const Vec3 &w_o,
                       const Vec3 &w_i, bool adjoint) const {
  return kd_ / M_PI;
}

Vec3 LambertianBrdf::SampleF(const SurfaceDiff &surface, const Vec3 &w_o,
                             Vec3 &w_i, Real &pdf, bool adjoint) const {
  sampling::CosWeightedDirHemisphere(w_i, pdf);
  w_i = surface.x * w_i.x + surface.y * w_i.y + surface.z * w_i.z;
  return kd_ / M_PI;
}

const Vec3 &LambertianBrdf::kd() const { return kd_; }

PhongLobe::PhongLobe(const Vec3 &ks, Real n)
    : Bsdf(Type(kReflective | kGlossy)), ks_(ks), n_(n) {}

Vec3 PhongLobe::F(const SurfaceDiff &surface, const Vec3 &w_o, const Vec3 &w_i,
                  bool adjoint) const {
  auto reflected_w_i = ReflectAbout(w_i, surface.y);
  return ks_ * (n_ + 2) / (2 * M_PI) * std::pow(Dot(w_o, reflected_w_i), n_);
}

Vec3 PhongLobe::SampleF(const SurfaceDiff &surface, const Vec3 &w_o, Vec3 &w_i,
                        Real &pdf, bool adjoint) const {
  auto xi1 = rng::Uniform();
  auto xi2 = rng::Uniform();
  Real x =
      std::sqrt(1 - std::pow(xi1, 2.0 / (n_ + 1))) * std::cos(2.0 * M_PI * xi2);
  Real y = std::pow(xi1, 1.0 / (n_ + 1));
  Real z =
      std::sqrt(1 - std::pow(xi1, 2.0 / (n_ + 1))) * std::sin(2.0 * M_PI * xi2);
  auto wo_r = ReflectAbout(w_o, surface.y);
  auto x_axis = Normalize(NormalTo(wo_r));
  auto z_axis = Cross(x_axis, wo_r);
  w_i = x_axis * x + wo_r * y + z_axis * z;
  auto cos_theta = std::max(Dot(w_o, ReflectAbout(w_i, surface.y)), Real(0));
  auto cos_alpha = y;
  pdf = (n_ + 1) / (2 * M_PI) * std::pow(cos_alpha, n_);
  return ks_ * (n_ + 2) / (2 * M_PI) * std::pow(cos_theta, n_);
}

const Vec3 &PhongLobe::ks() const { return ks_; }

PhongBrdf::PhongBrdf()
    : Bsdf(Type(kReflective | kGlossy)),
      diffuse_component_(Vec3()),
      specular_component_(Vec3(), 0) {}

PhongBrdf::PhongBrdf(const Vec3 &kd, const Vec3 &ks, Real n)
    : Bsdf(Type(kReflective | kGlossy)),
      diffuse_component_(kd),
      specular_component_(ks, n) {}

Vec3 PhongBrdf::F(const SurfaceDiff &surface, const Vec3 &w_o, const Vec3 &w_i,
                  bool adjoint) const {
  auto avg_kd = Avg(diffuse_component_.kd());
  auto xi0 = rng::Uniform();
  if (xi0 < avg_kd) {
    return diffuse_component_.F(surface, w_o, w_i, adjoint) / avg_kd;
  } else {
    return specular_component_.F(surface, w_o, w_i, adjoint) / (1 - avg_kd);
  }
}

Vec3 PhongBrdf::SampleF(const SurfaceDiff &surface, const Vec3 &w_o, Vec3 &w_i,
                        Real &pdf, bool adjoint) const {
  auto avg_kd = Avg(diffuse_component_.kd());
  auto xi0 = rng::Uniform();
  if (xi0 < avg_kd) {
    auto f = diffuse_component_.SampleF(surface, w_o, w_i, pdf, adjoint);
    pdf *= avg_kd;
    return f;
  } else {
    auto f = specular_component_.SampleF(surface, w_o, w_i, pdf, adjoint);
    pdf *= 1 - avg_kd;
    return f;
  }
}

SpecularReflectionTransmission::SpecularReflectionTransmission(Real n1, Real n2)
    : Bsdf(Type(kSpecular | kTransmissive | kReflective)), n1_(n1), n2_(n2) {}

Vec3 SpecularReflectionTransmission::F(const SurfaceDiff &surface,
                                       const Vec3 &w_o, const Vec3 &w_i,
                                       bool adjoint) const {
  return Vec3();
}

bool Refract(const Vec3 &w_i, const Vec3 &normal, Real n, Vec3 &w_t) {
  Real cos_theta_i = Dot(normal, w_i);
  Real sin2_theta_i = std::max(0.0, 1.0 - cos_theta_i * cos_theta_i);
  Real sin2_theta_t = n * n * sin2_theta_i;
  if (sin2_theta_t >= 1) {
    return false;
  }
  Real cos_theta_t = std::sqrt(1 - sin2_theta_t);
  w_t = n * -w_i + (n * cos_theta_i - cos_theta_t) * normal;
  return true;
}

Vec3 SpecularReflectionTransmission::SampleF(const SurfaceDiff &surface,
                                             const Vec3 &w_o, Vec3 &w_i,
                                             Real &pdf, bool adjoint) const {
  Real fr = FresnelReflectance(surface, w_o, n1_, n2_);
  if (rng::Uniform() < fr) {
    // specular reflection
    pdf = fr;
    w_i = ReflectAbout(w_o, surface.y);
    return fr / std::abs(Dot(surface.y, w_i));
  } else {
    // specular transmission
    Real n1 = n1_;
    Real n2 = n2_;
    auto cos_n_wo = Dot(surface.y, w_o);
    if (cos_n_wo <= 0) std::swap(n1, n2);
    auto new_normal = cos_n_wo < 0 ? -surface.y : surface.y;
    if (!Refract(w_o, new_normal, n1 / n2, w_i)) return Vec3();
    pdf = 1 - fr;
    auto ft = Vec3(1 - fr);
    if (!adjoint) {
      ft *= (n1 * n1) / (n2 * n2);
    }
    return ft / std::abs(Dot(surface.y, w_i));
  }
}

Real ren::FresnelReflectance(const SurfaceDiff &surface, const Vec3 &i, Real n1,
                             Real n2) {
  Real cos_theta_i = Dot(surface.y, i);
  if (cos_theta_i <= 0) {
    std::swap(n1, n2);
    cos_theta_i = std::abs(cos_theta_i);
  }
  Real sin_theta_i =
      std::sqrt(std::max(Real(0), 1 - cos_theta_i * cos_theta_i));
  Real sin_theta_t = n1 * sin_theta_i / n2;
  if (sin_theta_t >= 1) return 1;
  Real cos_theta_t =
      std::sqrt(std::max(Real(0), 1 - sin_theta_t * sin_theta_t));
  Real r_p = (n2 * cos_theta_i - n1 * cos_theta_t) /
             (n2 * cos_theta_i + n1 * cos_theta_t);
  Real r_s = (n1 * cos_theta_i - n2 * cos_theta_t) /
             (n1 * cos_theta_i + n2 * cos_theta_t);
  return (r_p * r_p + r_s * r_s) / 2;
}
