#include "material.h"
#include "parse.h"

#include <fstream>

namespace material {
material parse(const std::string &filename) {
  std::ifstream file = open(filename);
}
}; // namespace material