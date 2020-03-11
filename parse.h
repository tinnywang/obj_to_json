#include <fstream>
#include <map>
#include <string>

#ifndef INCLUDE_PARSE_H
#define INCLUDE_PARSE_H

namespace parse {
std::ifstream open(const std::string &filename);
double round(double d, double precision = 6);
} // namespace parse

#endif // INCLUDE_PARSE_H
