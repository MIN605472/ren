#include "ren/rng.h"
#include <random>

ren::Real ren::rng::Uniform() {
  static std::default_random_engine generator((std::random_device())());
  static std::uniform_real_distribution<Real> distribution(0.0, 1.0);
  return distribution(generator);
}
