#include <iostream>
#include <vector>

#include "include/json.hpp"
#include "parse.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: obj_to_json <file>" << std::endl;
    return 1;
  }

  const char *filename = argv[1];
  std::ifstream file(filename);
  if (file.fail()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return 1;
  }

  std::vector<object> objects = parse(file);
  std::cout << nlohmann::json(objects) << std::endl;

  return 0;
}
