#include <map>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#ifndef INCLUDE_MATERIAL_H
#define INCLUDE_MATERIAL_H

namespace material {
enum definition {
  newmtl, // newmtl
  Ka,     // ambient
  Kd,     // diffuse
  Ks,     // specular
  Ns,     // specular exponent
  d,      // transparency
  Tr,     // transparency (Tr = 1 - d)
  illum,  // illumination model
};

static std::map<const std::string, definition> definitions = {
    {"newmtl", newmtl}, {"Ka", Ka}, {"Kd", Kd}, {"Ks", Ks},
    {"Ns", Ns},         {"d", d},   {"Tr", Tr}, {"illum", illum},
};

struct color {
  double r, g, b;
};

struct material {
  std::string name;
  color ambient;
  color diffuse;
  color specular;
  double specular_exponent;
};

std::map<std::string, material> parse(const std::string &filename);
color &parse(std::stringstream &ss, color &c);
void to_json(nlohmann::json &j, const material &m);
void to_json(nlohmann::json &j, const color &c);
void write(std::map<std::string, material> materials, std::ostream &out);
}; // namespace material

#endif // INCLUDE_MATERIAL_H