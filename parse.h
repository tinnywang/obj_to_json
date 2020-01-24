#include <fstream>
#include <map>
#include <string>

#ifndef INCLUDE_PARSE_H
#define INCLUDE_PARSE_H

enum definition {
  O,  // object
  V,  // vertex
  VN, // vertex normal
  F   // face
};

static std::map<const std::string, definition> definitions = {
    {"o", O}, {"v", V}, {"vn", VN}, {"f", F}};

std::ifstream open(const std::string &filename);

#endif // INCLUDE_PARSE_H
