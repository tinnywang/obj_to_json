#include "object.h"

#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: obj_to_json <file>" << std::endl;
    return 1;
  }

  object::write(object::parse(argv[1]), std::cout);
  return 0;
}
