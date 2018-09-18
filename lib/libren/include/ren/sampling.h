#ifndef REN_SAMPLING_H_
#define REN_SAMPLING_H_
#include "ren/typedefs.h"
#include "ren/vec.h"
namespace ren {
namespace sampling {
// Sample a direction on the hemisphere propotional to the cosine of the angle
// between the normal and incident direction.
// @param dir the sampled direction
// @param pdf the sample probability
void CosWeightedDirHemisphere(Vec3& dir, Real& pdf);
// Sample a direction on the hemisphere uniformly
// @param dir the sampled direction
// @param pdf the sample probability
void UniformDirHemisphere(Vec3& dir, Real& pdf);
// Sample a direction on the sphere uniformly
// @param dir the sampled direction
// @param pdf the sample probability
void UniformDirSphere(Vec3& dir, Real& pdf);
}  // namespace sampling
}  // namespace ren
#endif  // REN_SAMPLING_H_
