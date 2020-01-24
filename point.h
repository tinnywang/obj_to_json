#ifndef INCLUDE_POINT_H
#define INCLUDE_POINT_H

namespace point {
struct point {
  double x, y, z;
};

point add(const point &p1, const point &p2);
point normalize(const point &p);
}; // namespace point

#endif