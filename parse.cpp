#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "include/json.hpp"
#include "parse.h"

std::vector<object> parse(std::ifstream& file) {
    std::string line;
    unsigned int line_number = 0;
    std::stringstream ss;
    std::string definition;
    std::vector<object> objects;

    while(std::getline(file, line)) {
        line_number++;
        ss = std::stringstream(line);
        ss >> definition;
        try {
            switch(definitions.at(definition)) {
                case O: {
                    object o;
                    ss >> o.name;

                    if (objects.empty()) {
                      o.v_offset = 0;
                      o.vn_offset = 0;
                    } else {
                      const object& prev = objects.back();
                      o.v_offset = prev.vertices.size();
                      o.vn_offset = prev.vertex_normals.size();
                    }

                    objects.push_back(o);
                    break;
                }
                case V: {
                    object& o = objects.back();
                    point p;
                    ss >> p.x >> p.y >> p.z;
                    o.vertices.push_back(p);

                    p = { .x = 0, .y = 0, .z = 0 };
                    o.vertex_normals_aggregate.push_back(std::make_pair(p, 0));
                    break;
                }
                case VN: {
                    object& o = objects.back();
                    point p;
                    ss >> p.x >> p.y >> p.z;
                    o.vertex_normals.push_back(p);
                    break;
                }
                case F: {
                    object& o = objects.back();
                    std::string indices, index;
                    while(ss >> indices) {
                        try {
                            std::stringstream indices_ss(indices);

                            // vertex index
                            std::getline(indices_ss, index, '/');
                            int v_index = std::stoi(index) - o.v_offset - 1;
                            o.faces.push_back(v_index);

                            // vertex texture index (ignore)
                            std::getline(indices_ss, index, '/');

                            // vertex normal index
                            std::getline(indices_ss, index);
                            if (!index.empty()) {
                              int vn_index = std::stoi(index) - o.vn_offset - 1;

                              std::pair<point, int>& pair = o.vertex_normals_aggregate.at(v_index);
                              pair.first = addPoints(pair.first, o.vertex_normals.at(vn_index));
                              pair.second++;
                            }
                        } catch (std::invalid_argument e) {
                            std::cerr << "line " << line_number << ": " << e.what() << std::endl;
                        } catch (std::out_of_range e) {
                            std::cerr << "line " << line_number << ": " << e.what() << std::endl;
                        }
                    }
                }
                break;
            }
        } catch (std::out_of_range) {
            // ignore any unsupported definitions
        } 
    }

    for (auto objects_it = objects.begin(); objects_it != objects.end(); objects_it++) {
      for (auto normals_it = objects_it->vertex_normals_aggregate.begin(); normals_it != objects_it->vertex_normals_aggregate.end(); normals_it++) {
        std::pair<point, int>& pair = *normals_it;
        point normal = normalize({
          .x = pair.first.x / pair.second,
          .y = pair.first.y / pair.second,
          .z = pair.first.z / pair.second,
        });
        objects_it->normals.push_back(normal.x);
        objects_it->normals.push_back(normal.y);
        objects_it->normals.push_back(normal.z);
      }
    }

    return objects;
}

void to_json(nlohmann::json& j, const object& o) {
  std::vector<double> vertices;
  std::for_each(o.vertices.begin(), o.vertices.end(), [&vertices](const point& p) {
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

point addPoints(const point& p1, const point& p2) {
    return point{
        .x = p1.x + p2.x,
        .y = p1.y + p2.y,
        .z = p1.z + p2.z,
    };
}

point normalize(const point& p) {
  double magnitude = std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
  return point{
    .x = p.x / magnitude,
    .y = p.y / magnitude,
    .z = p.z / magnitude,
  };
}
