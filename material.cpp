#include "material.h"
#include "parse.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

namespace material {
std::map<std::string, material> parse(const std::string &filename) {
  std::ifstream file = open(filename);
  std::string line;
  std::stringstream ss;
  std::string definition;
  std::map<std::string, material> materials;

  while (std::getline(file, line)) {
    ss = std::stringstream(line);
    ss >> definition;
    material _m;
    material &m = _m;

    try {
      switch (definitions.at(definition)) {
      case newmtl: {
        ss >> m.name;
        materials.emplace(m.name, m);
        break;
      }
      case Ka: {
        parse(ss, m.ambient);
        break;
      }
      case Kd: {
        parse(ss, m.diffuse);
        break;
      }
      case Ks: {
        parse(ss, m.specular);
        break;
      }
      case Ns: {
        ss >> m.specular_exponent;
        break;
      }
      case d:
      case Tr: {
        break;
      }
      case illum: {
        break;
      }
      }
    } catch (std::out_of_range) {
      // ignore any unsupported definitions
    }
  }

  return materials;
}

color &parse(std::stringstream &ss, color &c) {
  ss >> c.r >> c.g >> c.b;
  return c;
}

void to_json(nlohmann::json &j, const material &m) {
  j = nlohmann::json{
      {"name", m.name},
      {"ambient", m.ambient},
      {"diffuse", m.diffuse},
      {"specular", m.specular},
      {"specular_exponent", m.specular_exponent},
  };
}

void to_json(nlohmann::json &j, const color &c) {
  std::vector<double> rgb = {c.r, c.g, c.b};
  j = nlohmann::json(rgb);
}

void write(std::map<std::string, material> materials, std::ostream &out) {
  out << nlohmann::json(materials) << std::endl;
}
}; // namespace material