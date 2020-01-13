#include <map>
#include <string>
#include <utility>
#include <vector>

#include "include/json.hpp"

#ifndef INCLUDE_PARSE_H
#define INCLUDE_PARSE_H

enum definition {
    O,  // object
    V,  // vertex
    VN, // vertex normal
    F   // face
};

static std::map<const std::string, definition> definitions = {
    {"o", O},
    {"v", V},
    {"vn", VN},
    {"f", F}
};

struct point {
    double x;
    double y;
    double z;
};

struct object {
    std::string name;
    std::vector<point> vertices;
    std::vector<point> vertex_normals;
    std::vector<int> faces;
    std::vector<std::pair<point, int>> vertex_normals_aggregate;
    std::vector<double> normals;
};

std::vector<object> parse(const char* filename);

void to_json(nlohmann::json& j, const object& o);

point addPoints(point const &p1, point const &p2);
point normalize(point const &p);

#endif  // INCLUDE_PARSE_H
