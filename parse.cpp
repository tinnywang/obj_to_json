#include <exception>
#include <fstream>
#include <string>

std::ifstream open(const std::string &filename) {
  std::ifstream file(filename);
  if (file.fail()) {
    throw std::invalid_argument("Failed to open file: " + filename);
  }
  return file;
}
