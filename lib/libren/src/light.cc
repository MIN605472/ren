#include "ren/light.h"

using namespace ren;

Light::Light(const Mat4 &local_to_world, const Vec3 &power)
    : local_to_world_(local_to_world),
      world_to_local_(Inverse(local_to_world)),
      power_(power) {}

Vec3 Light::power() { return power_; }
