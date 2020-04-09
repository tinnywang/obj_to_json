#include "object.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "nlohmann/json.hpp"

#include "material.h"
#include "parse.h"
#include "point.h"

namespace object {
object &parse(std::ifstream &file, const std::string &filename,
              unsigned int &line_number, object &o) {
  std::string line, definition;
  std::stringstream ss;
  std::streampos pos;

  while (std::getline(file, line)) {
    ss = std::stringstream(line);
    ss >> definition;
    line_number++;

    try {
      switch (definitions.at(definition)) {
      case V: {
        point::point p;
        ss >> p.x >> p.y >> p.z;
        o.vertices.push_back(p);

        o.vn_aggregate.push_back(std::make_pair(point::point{}, 0));
        break;
      }
      case VN: {
        point::point p;
        ss >> p.x >> p.y >> p.z;
        o.vertex_normals.push_back(p);
        break;
      }
      case F: {
        if (o.faces.empty()) {
          o.faces.push_back(face_group{});
        };
        face_group &fg = o.faces.back();

        std::string indices, index;
        while (ss >> indices) {
          try {
            std::stringstream indices_ss(indices);

            // vertex index
            std::getline(indices_ss, index, '/');
            int v_index = std::stoi(index) - o.v_offset - 1;
            fg.vertex_indices.push_back(v_index);

            // vertex texture index (ignore)
            std::getline(indices_ss, index, '/');

            // vertex normal index
            std::getline(indices_ss, index);
            if (!index.empty()) {
              int vn_index = std::stoi(index) - o.vn_offset - 1;
              std::pair<point::point, int> &pair = o.vn_aggregate.at(v_index);
              pair.first =
                  point::add(pair.first, o.vertex_normals.at(vn_index));
              pair.second++;
            }
          } catch (std::invalid_argument e) {
            std::cerr << filename << ", line " << line_number << ": "
                      << e.what() << std::endl;
          } catch (std::out_of_range e) {
            std::cerr << filename << ", line " << line_number << ": "
                      << e.what() << std::endl;
          }
        }

        break;
      }
      case usemtl: {
        std::string name;
        ss >> name;
        if (!o.materials.empty()) {
          try {
            face_group fg;
            fg.material = o.materials.at(name);
            o.faces.push_back(fg);
          } catch (std::out_of_range e) {
            std::cerr << filename << ", line " << line_number << ": "
                      << e.what() << std::endl;
          }
        }
        break;
      }
      default: {
        // Seek to the beginning of the line so that the definition can be
        // re-read by a different parse function.
        file.seekg(pos);
        return o;
      }
      }
    } catch (std::out_of_range) {
    }

    pos = file.tellg();
  }

  for (auto it = o.vn_aggregate.begin(); it != o.vn_aggregate.end(); it++) {
    point::point normal =
        point::normalize(point::scale(it->first, 1.0 / it->second));
    o.normals.push_back(normal.x);
    o.normals.push_back(normal.y);
    o.normals.push_back(normal.z);
  }

  return o;
}

std::vector<object> parse(const std::string &filename) {
  std::ifstream file = parse::open(filename);
  std::string line, definition;
  unsigned int line_number = 0;
  std::stringstream ss;
  std::vector<object> objects;

  material::mtl materials;
  while (std::getline(file, line)) {
    line_number++;
    ss = std::stringstream(line);
    ss >> definition;
    try {
      switch (definitions.at(definition)) {
      case mtllib: {
        std::string material_filename;
        ss >> material_filename;
        materials = material::parse(material_filename);
        break;
      };
      case O: {
        object o;
        ss >> o.name;

        if (objects.empty()) {
          o.v_offset = 0;
          o.vn_offset = 0;
        } else {
          const object &prev = objects.back();
          o.v_offset = prev.vertices.size();
          o.vn_offset = prev.vertex_normals.size();
        }

        if (!materials.empty()) {
          o.materials = materials;
        }

        objects.push_back(parse(file, filename, line_number, o));
        break;
      }
      default: { break; }
      }
    } catch (std::out_of_range) {
      // ignore any unsupported definitions
    }
  }

  return objects;
}

void to_json(nlohmann::json &j, const face_group &fg) {
  j = nlohmann::json{
      {"vertex_indices", fg.vertex_indices},
      {"material", fg.material},
  };
}

void to_json(nlohmann::json &j, const object &o) {
  std::vector<double> vertices;
  std::for_each(o.vertices.begin(), o.vertices.end(),
                [&vertices](const point::point &p) {
                  vertices.push_back(p.x);
                  vertices.push_back(p.y);
                  vertices.push_back(p.z);
                });

  j = nlohmann::json{
      {"name", o.name},
      {"vertices", vertices},
      {"faces", o.faces},
      {"normals", o.normals},
  };
}

void write(std::vector<object> objects, std::ostream &out) {
  out << nlohmann::json(objects) << std::endl;
}
}; // namespace object
