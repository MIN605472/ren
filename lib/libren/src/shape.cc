#include "ren/shape.h"

using namespace ren;

Shape::Shape(const Mat4 &local_to_world)
    : local_to_world_(local_to_world),
      world_to_local_(Inverse(local_to_world)) {}

const Mat4 &Shape::world_to_local() const { return world_to_local_; }

const Mat4 &Shape::local_to_world() const { return local_to_world_; }
