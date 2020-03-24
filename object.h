#include <optional>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "nlohmann/json.hpp"

#include "material.h"
#include "point.h"

#ifndef INCLUDE_OBJECT_H
#define INCLUDE_OBJECT_H

namespace object {

enum definition {
  O,      // object
  V,      // vertex
  VN,     // vertex normal
  F,      // face
  mtllib, // material library
  usemtl, // use material
};

static std::map<const std::string, definition> definitions = {
    {"o", O}, {"v", V},           {"vn", VN},
    {"f", F}, {"mtllib", mtllib}, {"usemtl", usemtl}};

// A face_group contains the geometry of faces that use the same material.
struct face_group {
  std::vector<int> vertex_indices;
  std::vector<double> normals;
  std::optional<material::material> material;
};

struct object {
  std::string name;
  std::vector<point::point> vertices;
  std::vector<point::point> vertex_normals;
  std::vector<face_group> faces;

  // Vertex indices and vertex normal indices are unique per file, not per
  // object. To convert a per-file index to a per-object index, decrement the
  // index by its offset.
  std::vector<point::point>::size_type v_offset;
  std::vector<point::point>::size_type vn_offset;

  // The Material Template Library (mtl) that defines the materials used by this
  // object.
  material::mtl materials;
};

std::vector<object> parse(const std::string &filename);
void to_json(nlohmann::json &j, const object &o);
void write(std::vector<object> objects, std::ostream &out);
}; // namespace object

#endif // INCLUDE_OBJECT_H
