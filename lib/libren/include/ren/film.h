#ifndef REN_FILM_H_
#define REN_FILM_H_
#include <string>
#include <vector>
#include "ren/typedefs.h"
#include "ren/vec.h"
namespace ren {
// Class that represents that represent a photographic film. Used at camera FOV
// calculation and to save and write the digital image.
class Film {
 public:
  Film(Real film_height, Real film_width, Real image_height, Real image_width,
       const std::string &path);
  void SaveAsPpm() const;
  void Colorize(int row, int col, const Vec3 &c);
  Real film_height() const;
  Real film_width() const;
  int image_height() const;
  int image_width() const;

 private:
  inline int index(int row, int col) const { return row * image_width_ + col; }
  Real film_height_;
  Real film_width_;
  int image_height_;
  int image_width_;
  std::vector<Vec3> image_;
  std::string path_;
};
}  // namespace ren
#endif  // REN_FILM_H_
