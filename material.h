#include <string>

#ifndef INCLUDE_MATERIAL_H
#define INCLUDE_MATERIAL_H

namespace material {
struct color {
  double r, g, b;
};

struct material {
  std::string name;
  color ambient;
  color diffuse;
  color specular;
  color specular_exponent;
};

material parse(const std::string &filename);
}; // namespace material

#endif // INCLUDE_MATERIAL_H