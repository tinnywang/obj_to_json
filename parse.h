#include <map>
#include <string>
#include <utility>
#include <vector>

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
    std::vector<double> vertices;
    std::vector<point> vertex_normals;
    std::vector<int> faces;
    std::vector<std::pair<point, int>> vertex_normals_aggregate;
    std::vector<point> normals;
};

std::vector<object> parse(const char* filename);

point addPoints(point const &p1, point const &p2);

point normalize(point const &p);

#endif  // INCLUDE_PARSE_H
