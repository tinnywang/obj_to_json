#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "nlohmann/json.hpp"

#include "point.h"

#ifndef INCLUDE_OBJECT_H
#define INCLUDE_OBJECT_H

namespace object {
struct object {
  std::string name;
  std::vector<point::point> vertices;
  std::vector<point::point> vertex_normals;
  std::vector<int> faces;
  std::vector<std::pair<point::point, int>> vertex_normals_aggregate;
  std::vector<double> normals;

  // Vertex indices and vertex normal indices are unique per file, not per
  // object. To convert a per-file index to a per-object index, decrement the
  // index by its offset.
  std::vector<point::point>::size_type v_offset;
  std::vector<point::point>::size_type vn_offset;
};

std::vector<object> parse(const std::string &filename);
void to_json(nlohmann::json &j, const object &o);
void write(std::vector<object> objects, std::ostream &out);
}; // namespace object

#endif // INCLUDE_OBJECT_H