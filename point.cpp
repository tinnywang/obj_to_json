#include "point.h"

#include <cmath>

namespace point {
point add(const point &p1, const point &p2) {
  return point{
      .x = p1.x + p2.x,
      .y = p1.y + p2.y,
      .z = p1.z + p2.z,
  };
}

point scale(const point &p, double factor) {
  return point{
      .x = p.x * factor,
      .y = p.y * factor,
      .z = p.z * factor,
  };
}

point normalize(const point &p) {
  double magnitude = std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
  return point{
      .x = p.x / magnitude,
      .y = p.y / magnitude,
      .z = p.z / magnitude,
  };
}
}; // namespace point
