#include <fstream>
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

    // Vertex indices and vertex normal indices are unique per file, not per object.
    // To convert a per-file index to a per-object index, decrement the index by its offset.
    std::vector<point>::size_type v_offset;
    std::vector<point>::size_type vn_offset;
};

std::vector<object> parse(std::ifstream& file);

void to_json(nlohmann::json& j, const object& o);

point addPoints(const point& p1, const point& p2);
point normalize(const point& p);

#endif // INCLUDE_PARSE_H