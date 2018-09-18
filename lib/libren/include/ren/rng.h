#ifndef REN_RNG_H_
#define REN_RNG_H_
#include "ren/typedefs.h"
namespace ren {
namespace rng {
// Return a random number uniformly distributed between [0,1).
// NOTE: If Real is a float, there is a bug and the number could be between
// [0,1]; 1 being included.
// @return random number
Real Uniform();
}  // namespace rng
}  // namespace ren
#endif  // REN_RNG_H_
