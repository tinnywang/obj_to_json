#include "material.h"
#include "parse.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

namespace material {
color &parse(std::stringstream &ss, color &c) {
  ss >> c.r >> c.g >> c.b;
  return c;
}

material &parse(std::ifstream &file, material &m) {
  std::string line, definition;
  std::stringstream ss;
  std::streampos pos;

  while (std::getline(file, line)) {
    ss = std::stringstream(line);
    ss >> definition;

    try {
      switch (definitions.at(definition)) {
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
      default: {
        // Seek to the beginning of the line so that the definition can be
        // re-read by a different parse function.
        file.seekg(pos);
        return m;
      }
      }
    } catch (std::out_of_range) {
      // ignore any unsupported definitions
    }

    pos = file.tellg();
  }

  return m;
}

std::map<std::string, material> parse(const std::string &filename) {
  std::ifstream file = parse::open(filename);
  std::string line, definition;
  std::stringstream ss;
  std::map<std::string, material> materials;

  while (std::getline(file, line)) {
    ss = std::stringstream(line);
    ss >> definition;

    try {
      switch (definitions.at(definition)) {
      case newmtl: {
        material m;
        ss >> m.name;
        materials.emplace(m.name, parse(file, m));
        break;
      }
      }
    } catch (std::out_of_range) {
      // ignore any unsupported definitions
    }
  }

  return materials;
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
  std::vector<double> rgb = {parse::round(c.r), parse::round(c.g),
                             parse::round(c.b)};
  j = nlohmann::json(rgb);
}

void write(std::map<std::string, material> materials, std::ostream &out) {
  out << nlohmann::json(materials) << std::endl;
}
}; // namespace material