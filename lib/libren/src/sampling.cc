#define _USE_MATH_DEFINES
#include "ren/sampling.h"
#include "ren/rng.h"
#include "ren/vec.h"

using namespace ren;

void sampling::CosWeightedDirHemisphere(Vec3& dir, Real& pdf) {
  auto xi1 = rng::Uniform();
  auto xi2 = rng::Uniform();
  dir.x = std::cos(2 * M_PI * xi2) * std::sqrt(xi1);
  dir.y = std::sqrt(1 - xi1);
  dir.z = std::sin(2 * M_PI * xi2) * std::sqrt(xi1);
  pdf = dir.y / M_PI;
}

void sampling::UniformDirHemisphere(Vec3& dir, Real& pdf) {
  auto xi1 = rng::Uniform();
  auto xi2 = rng::Uniform();
  dir.x = std::cos(2 * M_PI * xi2) * std::sqrt(1 - xi1 * xi1);
  dir.y = xi1;
  dir.z = std::sin(2 * M_PI * xi2) * std::sqrt(1 - xi1 * xi1);
  pdf = 1 / (2 * M_PI);
}

void sampling::UniformDirSphere(Vec3& dir, Real& pdf) {
  Real theta = 2 * M_PI * rng::Uniform();
  Real phi = std::acos(1 - 2 * rng::Uniform());
  dir.x = std::sin(phi) * std::cos(theta);
  dir.y = std::cos(phi);
  dir.z = std::sin(phi) * std::sin(theta);
  pdf = 1 / (4.0 * M_PI);
}
