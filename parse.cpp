#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "parse.h"


std::vector<object> parse(const char* filename) {
    std::ifstream file(filename);
    std::string line;
    unsigned int line_number = 0;
    std::stringstream ss;
    std::string definition;

    std::vector<object> objects;
    object o;

    while(std::getline(file, line)) {
        line_number++;
        ss = std::stringstream(line);
        ss >> definition;
        try {
            switch(definitions.at(definition)) {
                case O: {
                    o = object{};
                    ss >> o.name;
                    objects.push_back(o);
                }
                case V: {
                    double x, y, z;
                    ss >> x >> y >> z;
                    o.vertices.push_back(x);
                    o.vertices.push_back(y);
                    o.vertices.push_back(z);

                    point p = { .x = 0, .y = 0, .z = 0};
                    o.vertex_normals_aggregate.push_back(std::make_pair(p, 0));
                }
                case VN: {
                    point p;
                    ss >> p.x >> p.y >> p.z;
                    o.vertex_normals.push_back(p);
                }
                case F: {
                    std::string vn_indices;
                    std::string::size_type sz;

                    if (o.name.compare("Head_Head_male") == 0) {
                        std::cout << "vertex normal aggregates: " << o.vertex_normals_aggregate.size() << std::endl;
                        return objects;
                    }

                    while(ss >> vn_indices) {
                        try {
                            // vertex index
                            int vertex_index = std::stoi(vn_indices, &sz) - 1;
                            o.faces.push_back(vertex_index);
                            // vertex texture index (ignore)
                            vn_indices = vn_indices.substr(sz + 1);
                            std::stoi(vn_indices, &sz);
                            // vertex normal index
                            int vn_index = std::stoi(vn_indices.substr(sz + 1)) - 1;

                            std::pair<point, int> pair = o.vertex_normals_aggregate.at(vertex_index);
                            pair.first = addPoints(pair.first, o.vertex_normals.at(vn_index));
                            pair.second++;
                            // TODO: test if this line is necessary...
                            o.vertex_normals_aggregate[vertex_index] = pair;
                        } catch (std::invalid_argument e) {
                            std::cerr << filename << ", line " << line_number << ": " << e.what() << std::endl;
                        } catch (std::out_of_range e) {
                            std::cerr << filename << ", line " << line_number << ": " << e.what() << std::endl;
                        }
                    }
                }
            }
        } catch (std::out_of_range) {
            // ignore any unsupported definitions
        } 
    }
    return objects;
}

point addPoints(point const &p1, point const &p2) {
    return point{
        .x = p1.x + p2.x,
        .y = p1.y + p2.y,
        .z = p1.z + p2.z,
    };
}